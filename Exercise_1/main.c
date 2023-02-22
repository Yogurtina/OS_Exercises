#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h> // for O_* constants
#include <sys/shm.h> // for share memory
#include <sys/mman.h> // for memory map
#include <sys/wait.h> // for wait

#define PROCESSES 4
#define N 1000000LL

// this function calculates the sum between 2 numbers start and end, sum = start + start+1 +... + end
long long int calculate(int start, int end) {
    long long int sum = 0;

    for (int i = start + 1; i <= end; i++) {
        sum += i;
    }

    return sum;
}

long long int *SUMS; // array in which we save the different sums of the processes.
                     // it is a long long int type, so we avoid the integer overflow.

int main() {

    // here we create a new mapping in the virtual address space of the process.
    SUMS = mmap(NULL, PROCESSES * sizeof(long long int), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);

    pid_t pid_code = fork(); //we create the child process

    // int rank is a flag in order to secure that we don't have race conditions between the child-process.
    // it can be 0, 1, 2, 3, depending in which child-process we are.
    int rank = 0;

    if (pid_code == 0) {

        pid_t pid_code_c = fork();
        pid_t pid_code_cc;
        pid_t pid_code_ccc;


        if (pid_code_c == 0) {
            rank = 0;
            pid_code_ccc = fork();
            if (pid_code_ccc == 0){
            }
            else {
                pid_code_cc = fork();
                if (pid_code_cc == 0) {
                    rank = 2;
                }
                else {
                    rank = 3;
                }
            }
        }
        else {
            rank = 1;
        }
        // for every child process we calculate the sum and we save it in the array we defined earlier 
        SUMS[rank] = calculate(rank * N / PROCESSES, (rank + 1) * N / PROCESSES);

        // we wait for every child process to end.
        waitpid(pid_code_c, NULL, 0); 
        waitpid(pid_code_cc, NULL, 0);
        waitpid(pid_code_ccc, NULL, 0);


        exit(0);
    }
    else {
        waitpid(pid_code, NULL, 0);

        long long int sum = 0;
        for (int i = 0; i < PROCESSES; i++) {
            sum += SUMS[i];
        }
        long long int check = (N*(N + 1))/2;

        printf("SUMS: %ld CHECK: %ld", sum, check);
    }
}