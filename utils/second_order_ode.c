#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "second_order_ode.h"

double right_part_1 (double x) {
    (void) x;
    return 1;
}

double p_part_1 (double x) {
    (void) x;
    return 1;
}

double theoretical_solution_1 (double x) {
    (void) x;
    return 1;
}

double right_part_2 (double x) {
    return (pow(M_PI,2)+1)*cos(M_PI*x);    
}


double p_part_2 (double x) {
    (void) x;
    return 1;
}

double theoretical_solution_2 (double x) {
    return cos(M_PI * x);
}

double right_part_4 (double x) {
    return M_PI*M_PI * cos(M_PI * x) 
         + 2.0 * M_PI*M_PI * cos(2.0 * M_PI * x) 
         + (1.0 + x*x) * (cos(M_PI * x) + 0.5 * cos(2.0 * M_PI * x));
}

double p_part_4 (double x) {
    return 1 + pow(x,2);
}

double theoretical_solution_4 (double x) {
    return cos(M_PI*x) + 0.5*cos(2*M_PI*x);
}

double right_part_3 (double x) {
    return (pow(M_PI,2)+1)*cos(M_PI*x);    
}


double p_part_3 (double x) {
    (void) x;
    return 0;
}

//Одно из решений
double theoretical_solution_3 (double x) {
    return (pow(M_PI,2) +1)/(pow(M_PI,2))*cos(M_PI * x);
}

double right_part_5 (double x) {
    return (-2.0 + 8.0*x - 0.5*pow(x,2) - 6.0*pow(x,3) -2.5*pow(x,4) + pow(x,5))*exp(x);
}


double p_part_5 (double x) {
    (void) x;
    return 0.5 + x;
}

double theoretical_solution_5 (double x) {
    return pow(x,2) * pow((1-x),2) * exp(x);
}

double right_part_6 (double x) {
    return (-2.0 + 8.0*x - 0.5*pow(x,2) - 6.0*pow(x,3) -2.5*pow(x,4) + pow(x,5))*exp(x) + (-x+0.5)*(pow(x,2) * pow((1-x),2) * exp(x)); 
}


double p_part_6 (double x) {
    (void) x;
    return 1;
}

double theoretical_solution_6 (double x) {
    return pow(x,2) * pow((1-x),2) * exp(x);
}
