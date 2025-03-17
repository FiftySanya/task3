#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <signal.h>
#include <unistd.h>
#include <sys/resource.h>  // For setrlimit()

volatile sig_atomic_t cpu_limit_hit = 0;
volatile long long accumulator = 0;

void cpu_limit_handler(int sig) {
    cpu_limit_hit = 1;
    write(STDERR_FILENO, "Error: CPU time limit exceeded\n", 31);
}

void setup_cpu_limit(int seconds) {
    struct rlimit rl;
    
    getrlimit(RLIMIT_CPU, &rl);
    
    rl.rlim_cur = seconds;
    rl.rlim_max = seconds + 1;
    
    if (setrlimit(RLIMIT_CPU, &rl) != 0) {
        perror("Failed to set CPU limit");
        exit(1);
    }
    
    struct sigaction sa;
    sa.sa_handler = cpu_limit_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    
    if (sigaction(SIGXCPU, &sa, NULL) == -1) {
        perror("Failed to set SIGXCPU handler");
        exit(1);
    }
}

void generate_unique(int *arr, int size, int max) {
    if (cpu_limit_hit) return;
    
    for (int i = 0; i < size && !cpu_limit_hit; i++) {
        int num;
        do {
            if (cpu_limit_hit) return;
            
            num = (rand() % max) + 1;
            int duplicate = 0;
            
            for (int j = 0; j < i; j++) {
                if (arr[j] == num) {
                    duplicate = 1;
                    break;
                }
            }
            
            if (!duplicate) {
                arr[i] = num;
                
                for (int k = 0; k < 100000000 && !cpu_limit_hit; k++) {
                    accumulator += k;
                    
                    if (k % 1000000 == 0 && cpu_limit_hit) {
                        return;
                    }
                }
                break;
            }
        } while (!cpu_limit_hit);
    }
}

int main() {
    setup_cpu_limit(1);
    
    srand(time(NULL));
    int numbers_49[7], numbers_36[6];
    
    generate_unique(numbers_49, 7, 49);
    
    if (!cpu_limit_hit) {
        generate_unique(numbers_36, 6, 36);
    }
    
    if (!cpu_limit_hit) {
        printf("7 з 49: ");
        for (int i = 0; i < 7; i++) printf("%d ", numbers_49[i]);
        printf("\n6 з 36: ");
        for (int i = 0; i < 6; i++) printf("%d ", numbers_36[i]);
        printf("\n");
        printf("Accumulator: %lld\n", accumulator);
    } 
    else {
        fprintf(stderr, "Program terminated due to CPU time limit\n");
    }
    
    return cpu_limit_hit ? 1 : 0;
}