#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>
#include "utils_schema.h"

double solve_by_schema_1(double starting_point, double a, double step_size, double length_of_segment) {
    
    double y = starting_point;
    double number_of_steps = length_of_segment/step_size;

    for (int i=0; i<number_of_steps; i++) {
        y = -step_size*a*y + y;
    }

    return y;
}

double solve_by_schema_2(double starting_point, double a, double step_size, double length_of_segment) {
    
    double y = starting_point;
    double number_of_steps = length_of_segment/step_size;

    for (int i=0; i<number_of_steps; i++) {
        y = y/(1+step_size*a);
    }

    return y;
}

double solve_by_schema_3(double starting_point, double a, double step_size, double length_of_segment) {
    
    double y = starting_point;
    double number_of_steps = length_of_segment/step_size;

    for (int i=0; i<number_of_steps; i++) {
        y = y*(2-a*step_size)/(2+a*step_size);
    }

    return y;
}

double solve_by_schema_4(double starting_point, double a, double step_size, double length_of_segment) {
    
    double y;
    double y_0 = starting_point;
    double y_1 = starting_point - a*step_size;
    double number_of_steps = length_of_segment/step_size;

    for (int i=0; i<number_of_steps-1; i++) {
        y = -2*step_size*a*y_1 + y_0;
        y_0 = y_1;
        y_1 = y;
    }

    return y;
}

double solve_by_schema_5(double starting_point, double a, double step_size, double length_of_segment) {
    
    double y;
    double y_0 = starting_point;
    double y_1 = starting_point - a*step_size;
    double number_of_steps = length_of_segment/step_size;

    for (int i=0; i<number_of_steps-1; i++) {
        y = (2*y_1 - 0.5*y_0)/(1.5 + step_size*a);
        y_0 = y_1;
        y_1 = y;
    }

    return y;
}

double solve_by_schema_6(double starting_point, double a, double step_size, double length_of_segment) {
    
    double y;
    double y_0 = starting_point;
    double y_1 = starting_point - a*step_size;
    double number_of_steps = length_of_segment/step_size;

    for (int i=0; i<number_of_steps-1; i++) {
        y = (2*a*step_size - 3.0)*y_0 + 4*y_1;
        y_0 = y_1;
        y_1 = y;
    }

    return y;
}
