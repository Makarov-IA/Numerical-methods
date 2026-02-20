#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "utils_interpolation.h"

#define EPS 1e-12

//Тестовый сет
static double func_1(double x) {
    return 1.0 / (1.0 + 25.0 * x * x);
}

static double func_2(double x) {
    return sin(2.0 * M_PI * x);
}

static double func_3(double x) {
    return exp(x);
}

static double func_4(double x) {
    double t = x * (1.0 - x);
    return t * t * exp(x);
}

//Выбор тестовой функции
double (*select_function(int set_number))(double) {
    switch (set_number) {
        case 1:
            return func_1;
        case 2:
            return func_2;
        case 3:
            return func_3;
        case 4:
            return func_4;
        default:
            return NULL;
    }
}

//Возвращает имя тест функции для принта в таблицу
const char *select_function_name(int set_number) {
    switch (set_number) {
        case 1:
            return "1/(1+25*x^2)";
        case 2:
            return "sin(2*pi*x)";
        case 3:
            return "exp(x)";
        case 4:
            return "x^2*(1-x)^2*exp(x)";
        case 5:
            return "Runge: 1/(1+25*x^2)";
        default:
            return "unknown";
    }
}

//равномерная сетка 
void fill_uniform_nodes(double *nodes, int n, double a, double b) {
    double h = (b - a) / (double)(n - 1);
    for (int i = 0; i < n; ++i) {
        nodes[i] = a + h * (double)i;
    }
}

//чебышевская сетка
void fill_chebyshev_nodes(double *nodes, int n, double a, double b) {
    for (int i = 0; i < n; ++i) {
        double t = cos(M_PI * (2.0 * (double)i + 1.0) / (2.0 * (double)n));
        nodes[i] = 0.5 * (a + b) + 0.5 * (b - a) * t;
    }
}

//Заполняем матрицу для решения через СЛАУ
void fill_vandermonde(double *matrix, const double *nodes, int n) {
    for (int i = 0; i < n; ++i) {
        double power = 1.0;
        for (int j = 0; j < n; ++j) {
            matrix[i * n + j] = power;
            power *= nodes[i];
        }
    }
}

//Простая решалка линейной системы Гауссом с выбором максимального по столбцу для численной стабильности
int solve_linear_system(double *matrix, double *rhs, double *solution, int n) {
    //Прямой ход
    for (int i = 0; i < n; ++i) {
        //Выбор максимального по столбцу
        int pivot = i;
        double max_val = fabs(matrix[i * n + i]);
        for (int r = i + 1; r < n; ++r) {
            double val = fabs(matrix[r * n + i]);
            if (val > max_val) {
                max_val = val;
                pivot = r;
            }
        }

        if (max_val < 1e-18) {
            return 1;
        }

        if (pivot != i) {
            for (int c = i; c < n; ++c) {
                double tmp = matrix[i * n + c];
                matrix[i * n + c] = matrix[pivot * n + c];
                matrix[pivot * n + c] = tmp;
            }
            {
                double tmp = rhs[i];
                rhs[i] = rhs[pivot];
                rhs[pivot] = tmp;
            }
        }

        for (int r = i + 1; r < n; ++r) {
            double factor = matrix[r * n + i] / matrix[i * n + i];
            matrix[r * n + i] = 0.0;
            for (int c = i + 1; c < n; ++c) {
                matrix[r * n + c] -= factor * matrix[i * n + c];
            }
            rhs[r] -= factor * rhs[i];
        }
    }

    //Обратный ход
    for (int i = n - 1; i >= 0; --i) {
        double sum = rhs[i];
        for (int c = i + 1; c < n; ++c) {
            sum -= matrix[i * n + c] * solution[c];
        }
        if (fabs(matrix[i * n + i]) < 1e-18) {
            return 1;
        }
        solution[i] = sum / matrix[i * n + i];
    }

    return 0;
}

//Вычисляем полином из коэффициентов
double eval_polynomial(const double *coeff, int n, double x) {
    double result = coeff[n - 1];
    for (int i = n - 2; i >= 0; --i) {
        result = result * x + coeff[i];
    }
    return result;
}

//Считаем коэффициента для Лагранжа
double eval_lagrange(const double *nodes, const double *values, int n, double x) {
    double result = 0.0;

    for (int i = 0; i < n; ++i) {
        double li = 1.0;
        for (int j = 0; j < n; ++j) {
            if (i == j) {
                continue;
            }
            li *= (x - nodes[j]) / (nodes[i] - nodes[j]);
        }
        result += values[i] * li;
    }

    return result;
}

//Пайплайн поиска коэффициентов через решение СЛАУ
int prepare_interpolation(double (*func)(double), const double *nodes, int n_nodes,
                                 double **values_out, double **coeff_out) {
    double *values = (double *)malloc(n_nodes * sizeof(double));
    double *matrix = (double *)malloc(n_nodes * n_nodes * sizeof(double));
    double *rhs = (double *)malloc(n_nodes * sizeof(double));
    double *coeff = (double *)malloc(n_nodes * sizeof(double));

    for (int i = 0; i < n_nodes; ++i) {
        values[i] = func(nodes[i]);
        rhs[i] = values[i];
    }

    fill_vandermonde(matrix, nodes, n_nodes);

    if (solve_linear_system(matrix, rhs, coeff, n_nodes) != 0) {
        fprintf(stderr, "Error: system is singular or ill-conditioned\n");
        free(values);
        free(matrix);
        free(rhs);
        free(coeff);
        return 1;
    }

    free(matrix);
    free(rhs);

    *values_out = values;
    *coeff_out = coeff;
    return 0;
}


int print_tables(const char *title, double (*func)(double),
                 const double *nodes, int n_nodes, int n_eval,
                 double a, double b, const char *table_path,
                 const char *nodes_path) {
    double *values = NULL;
    double *coeff = NULL;
    FILE *file = NULL;
    FILE *nodes_file = NULL;

    prepare_interpolation(func, nodes, n_nodes, &values, &coeff);

    file = fopen(table_path, "w");
    nodes_file = fopen(nodes_path, "w");

    printf("%s\n", title);
    printf("%12s %22s %22s %22s %22s\n", "x", "exact", "P_n", "L_(n-1)", "diff");

    
    double step = (b - a) / (double)(n_eval - 1);
    for (int i = 0; i < n_eval; ++i) {
        double x = a + step * (double)i;
        double exact = func(x);
        double approx_slae = eval_polynomial(coeff, n_nodes, x);
        double approx_lagrange = eval_lagrange(nodes, values, n_nodes, x);
        printf("%12.6f %22.12e %22.12e %22.12e %22.12e\n",
                x, exact, approx_slae, approx_lagrange, fabs(approx_slae - approx_lagrange));
        fprintf(file, "%.20f %.20f %.20f %.20f %.20f\n",
                x, exact, approx_slae, approx_lagrange, fabs(approx_slae - approx_lagrange));

    }

    for (int i = 0; i < n_nodes; ++i) {
        fprintf(nodes_file, "%.20f %.20f\n", nodes[i], values[i]);
    }
    
    fclose(nodes_file);
    fclose(file);
    free(values);
    free(coeff);
    return 0;
}
