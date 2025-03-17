#include <stdio.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>

int main() {
    int fd;
    int i = 0;
    while (1) {
        char filename[20];
        sprintf(filename, "file%d", i);
        fd = open(filename, O_CREAT | O_WRONLY, 0644);
        if (fd == -1) {
            printf("Failed to open file %s: %s\n", filename, strerror(errno));
            break;
        }
        printf("Opened file %s, fd=%d\n", filename, fd);
        i++;
    }
    return 0;
}