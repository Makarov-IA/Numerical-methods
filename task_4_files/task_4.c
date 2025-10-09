#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>
#include "../utils/utils_schema.h" 

#define BG_YELLOW  "\033[43m"
#define BG_WHITE   "\033[47m"
#define BG_MAGENTA "\033[45m"
#define BLACK   "\033[30m"
#define RESET   "\033[0m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define BLUE    "\033[34m"
#define BRIGHT_BLUE    "\033[94m"

int main(int argc, char *argv[]) {

    int schema;
    double a, h, starting_point, y, real_solution;
    FILE* files[4];
    
    double A[4] = {1,5,10,100};
    double H[5] = {pow(10,-1),pow(10,-2),pow(10,-3),pow(10,-4),pow(10,-5)};
    double answers[20];
    char path[256];
    
    starting_point = 1;

    assert(argc==2);
    assert(sscanf(argv[1], "%d", &schema)==1);

    for (int file=0; file<4; file++){
        snprintf(path, sizeof path, "data_graph/tech_graph_%d_%d.txt", schema, file);
        files[file] = fopen(path, "w");
    }

    printf(">>> Number of schema: %d <<<\n\n", schema);
    for (int i=0; i<4; i++) {
        a = A[i];
        real_solution = exp(-a);
        printf("A: " BG_YELLOW "%.0lf" RESET " Real_solution " BG_YELLOW "%.3e" RESET "\n", a, real_solution);
        for (int j=0; j<5; j++) {
            h = H[j];
            y = starting_point;

            switch (schema) {
            case 1: 
                y = solve_by_schema_1(1, a, h, 1);
                break;
            case 2:
                y = solve_by_schema_2(1, a, h, 1);
                break;
            case 3:
                y = solve_by_schema_3(1, a, h, 1);
                break;
            case 4:
                y = solve_by_schema_4(1, a, h, 1);
                break;
            case 5:
                y = solve_by_schema_5(1, a, h, 1);
                break;
            case 6:
                y = solve_by_schema_6(1, a, h, 1);
                break;
            }

            answers[j+i*5] = y;
            printf("Answer: %.6e Error_normed: " BG_MAGENTA "%.6e" RESET " H: %.0e\n", y, \
                fabs((y-real_solution)/(real_solution)), h);
            fprintf(files[i], "%.lf %.lf \n", -log(h), -log(fabs(y-real_solution)));
        }
        printf("\n");
    }
    printf("---------------------\n\n");
    for (int file=0; file < 4; file++) {
        fclose(files[file]);
    }

    return 0;
}
