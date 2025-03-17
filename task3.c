#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main() {
    FILE *file;
    int roll;

    srand(time(NULL));

    file = fopen("dice_rolls.txt", "w");
    if (file == NULL) {
        perror("Cannot open file");
        return 1;
    }

    for (int i = 0; i < 1000; i++) {
        roll = (rand() % 6) + 1;
        if (fprintf(file, "%d\n", roll) < 0) {
            perror("Write error - file size limit reached");
            break;
        }
    }

    fclose(file);
    return 0;
}