#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "../utils/utils_interpolation.h"

#define MAX_NODES 25

int main(int argc, char *argv[]) {
    int set_number;
    int n_nodes;
    int n_eval;
    double (*func)(double);

    assert(argc >= 4);
    assert(sscanf(argv[1], "%d", &set_number) == 1);
    assert(sscanf(argv[2], "%d", &n_nodes) == 1);
    assert(sscanf(argv[3], "%d", &n_eval) == 1);

    if (n_nodes < 2 || n_nodes > MAX_NODES) {
        fprintf(stderr, "Error: n_nodes must be in [2, %d]\n", MAX_NODES);
        return 1;
    }

    if (n_eval < 1) {
        fprintf(stderr, "Error: n_eval must be >= 1\n");
        return 1;
    }

    func = select_function(set_number);
    if (!func) {
        fprintf(stderr, "Error: incorrect set_number\n");
        return 1;
    }

    printf("Function %d: %s\n", set_number, select_function_name(set_number));
    printf("Interval [0, 1], nodes=%d, eval_points=%d\n\n", n_nodes, n_eval);

    double *nodes = (double *)malloc(n_nodes * sizeof(double));
    if (!nodes) {
        fprintf(stderr, "Error: memory allocation failed\n");
        return 1;
    }

    fill_uniform_nodes(nodes, n_nodes);
    print_table("Uniform nodes", func, nodes, n_nodes, n_eval);

    fill_chebyshev_nodes(nodes, n_nodes);
    print_table("Chebyshev nodes", func, nodes, n_nodes, n_eval);

    free(nodes);

    return 0;
}
