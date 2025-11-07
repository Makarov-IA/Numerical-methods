#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>
#include "../utils/utils_Runge.h" 

int main(int argc, char *argv[]) {

    double xR, x, y, H_MIN, H_MAX, thr_error, h, y_big_step, \
    y_small_step, x_big_step, x_small_step, estimated_error;
    int s, K, exiting_code, is_step_made, num_of_function;
    double (*f)(double, double);
    double (*f_sol)(double);
    char path[256];
    FILE* file;
    //Parsing args
    assert(argc==6);
    assert(sscanf(argv[1], "%d", &num_of_function)==1);
    assert(sscanf(argv[2], "%lf", &x)==1);
    assert(sscanf(argv[3], "%lf", &y)==1);
    assert(sscanf(argv[4], "%lf", &xR)==1);
    assert(sscanf(argv[5], "%lf", &h)==1);

    snprintf(path, sizeof path, "data_graph/number_of_function_%d.txt", num_of_function);
    file = fopen(path, "w");
    exiting_code = 0;
    s = 4;
    K = 32; //Use this K for the 4-th order
    thr_error = 1e-4;
    H_MIN = 1e-10;
    H_MAX = 50.0;

    switch (num_of_function) {
        case 1: 
            f = test_function_1;
            f_sol = test_function_sol_1;
            break;
        case 2: 
            f = test_function_2;
            f_sol = test_function_sol_2;
            break;
    }

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
        make_step(&x_small_step, &y_small_step, h/2, f);
        make_step(&x_small_step, &y_small_step, h/2, f);
        //one step path
        make_step(&x_big_step, &y_big_step, h, f);

        estimated_error = (y_big_step - y_small_step)/(pow(2, s) - 1);
        is_step_made = get_step(estimated_error, thr_error, K, &h, y_small_step, &y, &x);
        if (is_step_made)
            fprintf(file, "%lf %lf %lf %lf \n", x, y, f_sol(x), h);
    }
    return exiting_code;
}
