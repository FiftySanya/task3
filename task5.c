#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>

#define BUFFER_SIZE 1024

int main(int argc, char *argv[]) {
    int src_fd, dst_fd;
    char buffer[BUFFER_SIZE];
    ssize_t bytes;

    if (argc != 3) {
        printf("Program need two arguments\n");
        return 1;
    }

    if (access(argv[1], R_OK) != 0) {
        printf("Cannot open file %s for reading\n", argv[1]);
        return 1;
    }

    src_fd = open(argv[1], O_RDONLY);
    if (src_fd < 0) {
        perror("Open source file");
        return 1;
    }

    dst_fd = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (dst_fd < 0) {
        printf("Cannot open file %s for writing\n", argv[2]);
        close(src_fd);
        return 1;
    }

    while ((bytes = read(src_fd, buffer, BUFFER_SIZE)) > 0) {
        if (write(dst_fd, buffer, bytes) < 0) {
            perror("Write error - file size limit reached");
            break;
        }
    }

    close(src_fd);
    close(dst_fd);
    return 0;
}