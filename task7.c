#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

volatile sig_atomic_t file_size_exceeded = 0;

void sigxfsz_handler(int sig) {
    file_size_exceeded = 1;
    write(STDERR_FILENO, "Signal: File size limit exceeded\n", 33);
    _exit(1);
}

int main() {
    signal(SIGXFSZ, sigxfsz_handler);

    FILE *file = fopen("large_file.txt", "w");
    if (!file) {
        perror("Cannot open file");
        return 1;
    }

    for (int i = 0; i < 100000; i++) {
        if (fprintf(file, "Line %d\n", i) < 0) {
            perror("Write error");
            fclose(file);
            return 1;
        }
        if (file_size_exceeded) {
            fclose(file);
            return 1;
        }
    }

    fclose(file);
    return 0;
}