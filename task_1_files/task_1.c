#include <stdio.h>
#include <math.h>
#include <assert.h>
#include <float.h>

#define RESET   "\033[0m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define BLUE    "\033[34m"
#define DEBUG_COLOR    "\033[1;35m"

//Description
//we know taht d(e^x)/dx = e^x
//lets check if it is so in C 

int main(int argc, char *argv[]) {

    int n; 
    double x0;
    double h;
    double theoretical_der_at_x0;
    double real_derivative;
    double last_diff = DBL_MAX;
    double current_diff;

    assert(argc >= 3);
    assert(sscanf(argv[1], "%d", &n) != 0);
    assert(sscanf(argv[2], "%lf", &x0) != 0);
    theoretical_der_at_x0 = exp(x0);
    
    printf("Theoretical   Calculation   Difference    Δx\n");
    for (int i=0; i<n; i++) {
        h = 1/pow(10, i);
        real_derivative = (exp(x0+h) - exp(x0))/h;
        current_diff = fabs(theoretical_der_at_x0-real_derivative);
        printf(GREEN "%.6e " BLUE " %.6e " DEBUG_COLOR " %3.e ", 
        theoretical_der_at_x0, real_derivative, current_diff);
        if (current_diff > last_diff)
            printf(RED "   ↑ " RESET);
        else if (current_diff < last_diff)
            printf(GREEN "   ↓ " RESET);
        else 
            printf(BLUE "   = " RESET);
        last_diff = current_diff;
        printf("   %.3e\n", h);
        }
    
    return 0;
}
