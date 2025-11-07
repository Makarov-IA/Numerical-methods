#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>
#include "../utils/utils_Runge.h" 

#define min(a, b) ((a) < (b) ? (a) : (b))
#define max(a, b) ((a) < (b) ? (b) : (a))

int main(int argc, char *argv[]) {

    double xR, x, y, H_MIN, H_MAX, thr_error, h, y_big_step, h_old, \
    y_small_step, x_big_step, x_small_step, estimated_error, y_small_step_inner, \
    x_small_step_inner, max_diff, diff;
    int s, K, exiting_code, is_step_made, num_of_function, is_fixed_h;
    double (*f)(double, double, double *);
    double (*f_sol)(double, double *);
    double H[3] = {0, 0, 0};
    double *additional_args_to_function;
    char path[256];
    FILE* file;
    //parsing args
    assert(argc>=5);
    assert(sscanf(argv[1], "%d", &num_of_function)==1);
    assert(sscanf(argv[2], "%lf", &x)==1);
    assert(sscanf(argv[3], "%lf", &xR)==1);
    assert(sscanf(argv[4], "%lf", &h)==1);
    assert(sscanf(argv[5], "%d", &is_fixed_h)==1);
    if (argc-6 > 0)
        additional_args_to_function = (double*)malloc((argc-6)*sizeof(double));
    for (int i=0; i<argc-6; i++){
        assert(sscanf(argv[i+6], "%lf", &additional_args_to_function[i])==1);
    }
    //if h is not fixed make the plot of the solution
    if (!is_fixed_h)
        snprintf(path, sizeof path, "data_graph/%d.txt", num_of_function);
    //else make the plot to prove the 4-th order of approximation
    else snprintf(path, sizeof path, "data_graph/%d_prove.txt", num_of_function);

    file = fopen(path, "w");
    exiting_code = 0;
    s = 4;
    K = 32; //use this K for the 4-th order
    thr_error = 1e-4;
    H_MIN = 1e-10;
    H_MAX = xR-x;
    max_diff = 0;
    H[0] = h/8; H[1] = h/4; H[2] = h/2; 

    switch (num_of_function) {
        case 1: 
            f = test_function_1;
            f_sol = test_function_sol_1;
            break;
        case 2: 
            f = test_function_2;
            f_sol = test_function_sol_2;
            break;
        case 3: 
            f = test_function_3;
            f_sol = test_function_sol_3;
            break;
    }

    //initial point
    y = f_sol(x, additional_args_to_function);
    //the case to get h_path
    if (!is_fixed_h) {
        fprintf(file, "%lf %lf %lf %lf \n", x, y, f_sol(x, additional_args_to_function), 0.0);
        while (x < xR) {
            if (x + h > xR) {
                h = xR - x;
            }
            if (h > H_MAX) {
                printf("Stop solving cause h is bigger than H_MAX\n");
                exiting_code = 1;
                break;
            }
            if (h < H_MIN) {
                printf("Stop solving cause h is smaller than H_MIN\n");
                exiting_code = 2;
                break;
            }
            y_big_step = y;
            y_small_step = y;
            x_big_step = x;
            x_small_step = x;
            
            //two step path
            make_step(&x_small_step, &y_small_step, h/2, f, additional_args_to_function);
            //first inner point to report
            y_small_step_inner = y_small_step; 
            x_small_step_inner = x_small_step; 
            make_step(&x_small_step, &y_small_step, h/2, f, additional_args_to_function);
            
            //one step path
            make_step(&x_big_step, &y_big_step, h, f, additional_args_to_function);

            estimated_error = (y_big_step - y_small_step)/(pow(2, s) - 1);
            h_old = h/2; //the size of small steps
            is_step_made = get_step(estimated_error, thr_error, K, &h, y_small_step, &y, &x);
            if (is_step_made) {
                //print two points from small steps
                fprintf(file, "%.3lf %.15lf %.15lf %.6e \n", x_small_step_inner, y_small_step_inner, f_sol(x_small_step_inner, additional_args_to_function), h_old);
                fprintf(file, "%.3lf %.15lf %.15lf %.6e \n", x, y, f_sol(x, additional_args_to_function), h_old);
            }
        }
    }
    //the case to prove 4-th order
    else {
        for (int i=0; i < 3; i++) {
            h = H[i];
            x_small_step = x; y_small_step = y;
            while (x_small_step < xR) {
                if (x_small_step + h > xR) {
                    h = xR - x_small_step;
                }
                make_step(&x_small_step, &y_small_step, h, f, additional_args_to_function);
                diff = fabs(y_small_step-f_sol(x_small_step, additional_args_to_function));
                max_diff = max(diff, max_diff);
            }
            if (fabs(max_diff) <1e-12) {
                printf("Our solution exactly matches the real %d_func!\n", num_of_function);
                exiting_code = 3;
                return exiting_code;
            }
            fprintf(file, "%.15lf %.15lf \n", -log(H[i]), -log(max_diff));
        }
    }
    return exiting_code;
}
