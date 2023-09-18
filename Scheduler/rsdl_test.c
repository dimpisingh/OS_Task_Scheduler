#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <unistd.h>
#include <sys/resource.h>
#include <sched.h>
#include <sys/syscall.h> // For syscall

#define SYS_ISOLATE_CPU 548
#define SCHED_RSDL 7

struct result {
    int sum;
    double time;
};

int compute(int itr) {
    int sum = 0;
    for (int i = 0; i < itr; i++) {
        for (int j = 0; j < itr; j++) {
            sum += 1;
        }
    }
    return sum;
}

// Calculates the sum of compute results and measures the time taken.
struct result do_calc(int itr) {
    struct result ans;
    int sum;
    time_t start, end;
    double cpu_time_used;

    start = time(NULL);
    sum = compute(itr);
    end = time(NULL);
    cpu_time_used = difftime(end, start);

    ans.sum = sum;
    ans.time = cpu_time_used;

    return ans;
}

int main(int argc, char *argv[]) {
    if (argc < 3) {
        printf("Usage: ./rsdl_test <num_procs> <num_iterations>\n");
        return 0;
    }

    srand(time(NULL));

    int num_proc = atoi(argv[1]);
    int num_itr = atoi(argv[2]);
    int parent_pid = getpid();

    // Invoke your system call here to isolate CPU 0
    // YOUR CODE STARTS HERE (Implement SYS_ISOLATE_CPU)
    syscall(SYS_ISOLATE_CPU);
    // YOUR CODE ENDS HERE


    //num_cpus, mask, and size are used to set up CPU affinity for processes.
    int num_cpus = 4;
    cpu_set_t *mask = CPU_ALLOC(num_cpus);
    size_t size = CPU_ALLOC_SIZE(num_cpus);

    //CPU 0 is isolated, and the main process is assigned to it.
    CPU_ZERO_S(size, mask);
    CPU_SET_S(0, size, mask);
    sched_setaffinity(parent_pid, size, mask);

    //The priority of the main process is set to a high value using setpriority to give it higher priority than other processes.
    setpriority(PRIO_PROCESS, parent_pid, -20);

    struct sched_param param = {
        .sched_priority = 0
    };
    sched_setscheduler(parent_pid, SCHED_RSDL, &param);

    printf("<pid: %d, sched_class: %d, nice: %3d>\n", parent_pid, sched_getscheduler(parent_pid), getpriority(PRIO_PROCESS, parent_pid));

    int wstatus[num_proc];
    int pids[num_proc];

    //Each child process sets its priority to a random value and calculates the result of the compute function using do_calc
    for (int i = 0; i < num_proc; i++) {
        int pid = fork();
        int child_nice = rand() % 40 - 20;

        if (pid != 0) {
            pids[i] = pid;
        }

        if (pid == 0) {
            int child = getpid();
            setpriority(PRIO_PROCESS, child, child_nice);

            struct result res = do_calc(num_itr);
            printf("<pid: %d, sched_class: %d, nice: %3d, do_calc(%d): %d, time: %5.2f>\n", child, sched_getscheduler(child), getpriority(PRIO_PROCESS, child), num_itr, res.sum, res.time);

            return 0;
        }
    }

    for (int i = 0; i < num_proc; i++) {
        waitpid(pids[i], &wstatus[i], 0);
    }

    CPU_FREE(mask);
    return 0;
}