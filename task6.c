#include <stdio.h>
#include <stdlib.h>
#include <sys/resource.h>

void set_stack_limit(size_t size_kb) {
    struct rlimit rl;

    rl.rlim_cur = size_kb * 1024;  // Поточний ліміт (м'яке обмеження)
    rl.rlim_max = size_kb * 1024;  // Максимальний ліміт (жорстке обмеження)

    if (setrlimit(RLIMIT_STACK, &rl) != 0) {
        perror("setrlimit failed");
        exit(EXIT_FAILURE);
    }
}

void recursive(int n) {
    char array[1000];

    // Заповнюємо масив, щоб уникнути оптимізацій
    for (int i = 0; i < 1000; i++) {
        array[i] = (char)(n % 256);
    }

    printf("Depth: %d, array[0] = %d, array[999] = %d\n", n, array[0], array[999]);

    recursive(n + 1);
}

int main() {
    set_stack_limit(24);  // Встановлюємо стек до 24 KB
    recursive(1);
    return 0;
}
