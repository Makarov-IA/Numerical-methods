#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "../utils/utils_interpolation.h"

int main(int argc, char *argv[]) {
    int set_number;
    int n_nodes;
    int n_eval;
    double (*func)(double);
    double a;
    double b;

    assert(argc >= 4);
    assert(sscanf(argv[1], "%d", &set_number) == 1);
    assert(sscanf(argv[2], "%d", &n_nodes) == 1);
    assert(sscanf(argv[3], "%d", &n_eval) == 1);

    func = select_function(set_number);
    if (!func) {
        fprintf(stderr, "Error: incorrect set_number\n");
        return 1;
    }

    //Хардкод выбора отрезка построения
    if (set_number == 5) {
        a = -1.0;
        b = 1.0;
    } else {
        a = 0.0;
        b = 1.0;
    }

    printf("Function %d: %s\n", set_number, select_function_name(set_number));
    printf("Interval [%.2f, %.2f], nodes=%d, eval_points=%d\n\n", a, b, n_nodes, n_eval);

    double *nodes = (double *)malloc(n_nodes * sizeof(double));
    char path[256];

    fill_uniform_nodes(nodes, n_nodes, a, b);
    print_table("Uniform nodes", func, nodes, n_nodes, n_eval, a, b);
    snprintf(path, sizeof path, "data_plot/set_%d_uniform.txt", set_number);
    write_table_data(path, func, nodes, n_nodes, n_eval, a, b);
    snprintf(path, sizeof path, "data_plot/set_%d_uniform_nodes.txt", set_number);
    write_nodes_data(path, func, nodes, n_nodes);

    fill_chebyshev_nodes(nodes, n_nodes, a, b);
    print_table("Chebyshev nodes", func, nodes, n_nodes, n_eval, a, b);
    snprintf(path, sizeof path, "data_plot/set_%d_chebyshev.txt", set_number);
    write_table_data(path, func, nodes, n_nodes, n_eval, a, b);
    snprintf(path, sizeof path, "data_plot/set_%d_chebyshev_nodes.txt", set_number);
    write_nodes_data(path, func, nodes, n_nodes);

    free(nodes);

    return 0;
}
