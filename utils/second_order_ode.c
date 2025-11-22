#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "second_order_ode.h"

double right_part_1 (double x) {
    (void) x;
    return 1;
}

double right_part_2 (double x) {
    return (pow(M_PI,2)+1)*cos(M_PI*x);    
}

double theoretical_solution_1 (double x) {
    (void) x;
    return 1;
}

double theoretical_solution_2 (double x) {
    return cos(M_PI * x);
}
