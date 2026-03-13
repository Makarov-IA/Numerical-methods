#include <assert.h>
#include "../utils/utils_task10.h"

int main(int argc, char *argv[]) {
    assert(argc == 2);
    return task10_run_from_file(argv[1]);
}
