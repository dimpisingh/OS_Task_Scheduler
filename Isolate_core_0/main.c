#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <unistd.h>
#include <time.h>
#include <sys/resource.h>
#include <sched.h>
#include <linux/sched.h>
#include <sys/syscall.h>

// Define your custom system call number for isolating CPU 0
#define SYS_ISOLATE_CPU 548

// Function to implement the custom system call to isolate CPU 0. 
long isolate_cpu_0() {
    //This system call is responsible for isolating CPU 0.
    return syscall(SYS_ISOLATE_CPU);
}

//func() is designed to perform a large number of simple multiplications to create a high computational load.
//It's used in the code to measure the execution time of this computationally intensive task.
long long func() {
    long long ans = 0;
    for (int i = 0; i < 100000; i++) {
        for (int j = 0; j < 1000000; j++) {
            int res = i * j;
            ans = ans + res;
        }
    }
    return ans;
}

//measureTime() is a utility function to measure the execution time of the func() function, which helps assess the CPU performance of the system or a specific process.
double measureTime() {
    clock_t start, end;
    double cpu_time_used;

    start = clock();
    func();
    end = clock();

    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    return cpu_time_used;
}

int main(int argc, char *argv[]) {
    // Invoke your system call here to isolate CPU 0
    long isolation_result = isolate_cpu_0();
    if (isolation_result != 0) {
        perror("Failed to isolate CPU 0");
        exit(EXIT_FAILURE);
    }

    //Determines the number of available CPUs dynamically, rather than assuming there are four CPUs.
    int num_cpus = sysconf(_SC_NPROCESSORS_CONF); // Get the number of available CPUs
    cpu_set_t mask;
    CPU_ZERO(&mask);
    CPU_SET(0, &mask); // Set CPU 0 in the mask

    int c1pid = fork();
    int c2pid;
    double cpu_time_used;

    if (!c1pid) {
        // Assign this process to core 0 and change priority to highest priority
        //sched_setaffinity and setpriority to Handle any potential failures.
        if (sched_setaffinity(0, sizeof(mask), &mask) == -1) {
            perror("sched_setaffinity");
            exit(EXIT_FAILURE);
        }
        if (setpriority(PRIO_PROCESS, getpid(), -20) == -1) {
            perror("setpriority");
            exit(EXIT_FAILURE);
        }

        cpu_time_used = measureTime();
        printf("Process 1 used %lf seconds\n", cpu_time_used);
    } else {
        c2pid = fork();
        if (!c2pid) {
            // Assign this process to core 0 and change priority to lowest priority
            //sched_setaffinity and setpriority to Handle any potential failures.
            if (sched_setaffinity(0, sizeof(mask), &mask) == -1) {
                perror("sched_setaffinity");
                exit(EXIT_FAILURE);
            }
            if (setpriority(PRIO_PROCESS, getpid(), 19) == -1) {
                perror("setpriority");
                exit(EXIT_FAILURE);
            }

            cpu_time_used = measureTime();
            printf("Process 2 used %lf seconds\n", cpu_time_used);
        }
    }

    return 0;
}