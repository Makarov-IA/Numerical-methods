#include <assert.h>
#include <stdio.h>
#include "../utils/utils_task10.h"

int main(int argc, char *argv[]) {
    int set_number = 1;
    int n_nodes = 0;
    double a = 0.0;
    double b = 0.0;
    unsigned int seed = 42U;

    assert(argc >= 4);
    assert(sscanf(argv[1], "%d", &n_nodes) == 1);
    assert(sscanf(argv[2], "%lf", &a) == 1);
    assert(sscanf(argv[3], "%lf", &b) == 1);
    if (argc >= 5) {
        assert(sscanf(argv[4], "%u", &seed) == 1);
    }
    if (argc >= 6) {
        assert(sscanf(argv[5], "%d", &set_number) == 1);
    }

    return task10_run_for_set(set_number, n_nodes, a, b, seed);
}
