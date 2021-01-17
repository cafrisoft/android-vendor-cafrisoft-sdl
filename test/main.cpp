#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <limits.h>

int main(int argc, char* argv[]) {

    system("source /system/etc/python/conf/config_python.sh");

    while (true) {
        sleep(INT_MAX);
    }
    return 0;
}

