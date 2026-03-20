#include "../utils/utils_task11.h"

int main(int argc, char *argv[]) {
    const char *path = argc >= 2 ? argv[1] : "examples/set_1.txt";
    return task11_run(path);
}
