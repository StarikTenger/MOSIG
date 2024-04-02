#include "cpu_stats.h"
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define ANSI_RED "\e[0;31m"
#define ANSI_GREEN "\e[0;32m"
#define ANSI_YELLOW "\e[0;33m"
#define ANSI_BLUE "\e[0;34m"
#define ANSI_PURPLE "\e[0;35m"
#define ANSI_CYAN "\e[0;36m"
#define ANSI_RESET "\e[0m"

typedef struct timespec timespec;

timespec time_diff(timespec start, timespec end) {
    timespec temp;
    if ((end.tv_nsec - start.tv_nsec) < 0) {
        temp.tv_sec = end.tv_sec - start.tv_sec - 1;
        temp.tv_nsec = 1000000000 + end.tv_nsec - start.tv_nsec;
    } else {
        temp.tv_sec = end.tv_sec - start.tv_sec;
        temp.tv_nsec = end.tv_nsec - start.tv_nsec;
    }
    return temp;
}

double time_to_secs(timespec t) {
    return (double)t.tv_sec + 1e-9 * t.tv_nsec;
}

void print_cpu_stats_report(struct cpu_stats_report r) {
    printf("{");
    if (r.has_cpu_cycles) {
        // See https://stackoverflow.com/questions/9225567/how-to-portably-print-a-int64-t-type-in-c :)
        printf(ANSI_PURPLE "cpu_reference_cycles: %" PRId64 ANSI_RESET ", ", r.cpu_cycles);
    }
    printf( "elapsed_cpu_time: %.6f s" ", ", r.elapsed_cpu_time_secs / 1000);
    printf( "elapsed_real_time: %.6f s", r.elapsed_real_time_secs / 1000);
    printf("}");
    if (r.average_on_n > 1) {
        printf(" (average on %zd samples)", r.average_on_n);
    }
}

void println_cpu_stats_report(char *title, struct cpu_stats_report r) {
    printf("%s: ", title);
    print_cpu_stats_report(r);
    printf("\n");
}

struct cpu_stats_report average_report(struct cpu_stats_report *array, size_t len) {
    struct cpu_stats_report res = {
        .cpu_cycles = 0,
        .elapsed_cpu_time_secs = 0.0,
        .elapsed_real_time_secs = 0.0,
        .has_cpu_cycles = true,
        .average_on_n = len,
    };
    // Exclude the first experiment because it's more likely to be an outlier,
    // but only if len >= 3
    size_t i0 = len >= 3 ? 1 : 0;
    for (size_t i = i0; i < len; i++) {
        struct cpu_stats_report r = array[i];
        res.cpu_cycles += r.cpu_cycles;
        res.elapsed_cpu_time_secs += r.elapsed_cpu_time_secs;
        res.elapsed_real_time_secs += r.elapsed_real_time_secs;
        res.has_cpu_cycles &= r.has_cpu_cycles;
    }
    size_t n = len >= 3 ? len - 1 : len;
    res.cpu_cycles /= n;
    res.elapsed_cpu_time_secs /= n;
    res.elapsed_real_time_secs /= n;
    return res;
}

#ifdef __linux__

// ON LINUX: Use perf_event_open to count cpu cycles for the current process, more reliably than _rdtsc()

#include <linux/perf_event.h>
#include <sys/ioctl.h>
#include <sys/syscall.h>
#include <sys/types.h>
#include <unistd.h>

struct cpu_stats {
    timespec realtime_begin;
    timespec cputime_begin;
    int perf_event_fd;
};

int perf_event_open(struct perf_event_attr *attr, pid_t pid, int cpu, int group_fd, unsigned long flags) {
    return syscall(SYS_perf_event_open, attr, pid, cpu, group_fd, flags);
}

struct perf_event_attr *init_perf_event_attr() {
    // initialize attrs to zeros (default values)
    struct perf_event_attr *attr = malloc(sizeof(struct perf_event_attr));
    memset(attr, 0, sizeof(struct perf_event_attr));

    // set the size (so that the kernel can do some version checking for non-default params, this is requested by the docs)
    attr->size = sizeof(struct perf_event_attr);
    return attr;
}

struct cpu_stats *cpu_stats_init() {
    struct cpu_stats *stats = malloc(sizeof(struct cpu_stats));
    struct perf_event_attr *attr = init_perf_event_attr();
    // set some parameters
    attr->type = PERF_TYPE_HARDWARE;
    attr->config = PERF_COUNT_HW_REF_CPU_CYCLES; // not subject to frequency scaling issues, see https://stackoverflow.com/questions/43356721/papi-what-does-clock-reference-cycles-mean
    attr->exclude_kernel = 1; // disable kernel profiling so that it works even with kernel.perf_event_paranoid == 2, see https://stackoverflow.com/questions/51911368/what-restriction-is-perf-event-paranoid-1-actually-putting-on-x86-perf

    // pid == 0 and cpu == -1: measures the calling process on any CPU
    unsigned long flags = 0;
    int fd = perf_event_open(attr, 0, -1, -1, flags);
    if (fd == -1) {
        fprintf(stderr, ANSI_YELLOW "Error opening perf_event PERF_COUNT_HW_REF_CPU_CYCLES. If that's a permission problem, try: sudo sysctl -w kernel.perf_event_paranoid=0\n" ANSI_RESET);
    }

    stats->perf_event_fd = fd;
    return stats;
}

void cpu_stats_begin(struct cpu_stats *stats) {
    // measure t0
    clock_gettime(CLOCK_MONOTONIC, &stats->realtime_begin);
    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &stats->cputime_begin);

    // start counting perf event
    if (stats->perf_event_fd != -1) {
        ioctl(stats->perf_event_fd, PERF_EVENT_IOC_RESET, 0);
        ioctl(stats->perf_event_fd, PERF_EVENT_IOC_ENABLE, 0);
    }
}

struct cpu_stats_report cpu_stats_end(struct cpu_stats *stats) {
    uint64_t cycles_count = 0;
    bool has_cpu_cycles = false;
    if (stats->perf_event_fd != -1) {
        // stop counting perf event and read count
        ioctl(stats->perf_event_fd, PERF_EVENT_IOC_DISABLE, 0);
        int res = read(stats->perf_event_fd, &cycles_count, sizeof(cycles_count));
        if (res <= 0) {
            fprintf(stderr, ANSI_RED "Reading perf_event failed with return code %d\n" ANSI_RED, res);
        } else {
            has_cpu_cycles = true;
        }
    }

    // measure t1 and compute diff
    timespec cputime_end, realtime_end;
    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &cputime_end);
    clock_gettime(CLOCK_MONOTONIC, &realtime_end);
    double cputime_diff_secs = time_to_secs(time_diff(stats->cputime_begin, cputime_end));
    double realtime_diff_secs = time_to_secs(time_diff(stats->realtime_begin, realtime_end));

    // return struct with all results
    struct cpu_stats_report res = {
        .has_cpu_cycles = has_cpu_cycles,
        .cpu_cycles = cycles_count,
        .elapsed_cpu_time_secs = cputime_diff_secs,
        .elapsed_real_time_secs = realtime_diff_secs,
        .average_on_n = 1,
    };
    return res;
}

#else

#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

struct cpu_stats {
    LARGE_INTEGER realtime_begin;
    LARGE_INTEGER cputime_begin;
};

struct cpu_stats *cpu_stats_init() {
    struct cpu_stats *stats = malloc(sizeof(struct cpu_stats));
    return stats;
}

void cpu_stats_begin(struct cpu_stats *stats) {
    // measure t0
    QueryPerformanceCounter(&stats->realtime_begin);
    FILETIME creationTime, exitTime, kernelTime, userTime;
    GetProcessTimes(GetCurrentProcess(), &creationTime, &exitTime, &kernelTime, &userTime);
    stats->cputime_begin.QuadPart = ((ULONGLONG)kernelTime.dwHighDateTime << 32) | kernelTime.dwLowDateTime;
    stats->cputime_begin.QuadPart += ((ULONGLONG)userTime.dwHighDateTime << 32) | userTime.dwLowDateTime;
}

struct cpu_stats_report cpu_stats_end(struct cpu_stats *stats) {
    LARGE_INTEGER cputime_end, realtime_end;
    QueryPerformanceCounter(&realtime_end);
    FILETIME creationTime, exitTime, kernelTime, userTime;
    GetProcessTimes(GetCurrentProcess(), &creationTime, &exitTime, &kernelTime, &userTime);
    cputime_end.QuadPart = ((ULONGLONG)kernelTime.dwHighDateTime << 32) | kernelTime.dwLowDateTime;
    cputime_end.QuadPart += ((ULONGLONG)userTime.dwHighDateTime << 32) | userTime.dwLowDateTime;

    double elapsed_cpu_time_secs = (cputime_end.QuadPart - stats->cputime_begin.QuadPart) / (double)CLOCKS_PER_SEC;
    double elapsed_real_time_secs = (realtime_end.QuadPart - stats->realtime_begin.QuadPart) / (double)CLOCKS_PER_SEC;

    struct cpu_stats_report res = {
        .elapsed_real_time_secs = elapsed_real_time_secs,
        .elapsed_cpu_time_secs = elapsed_cpu_time_secs,
        .cpu_cycles = 0,
        .has_cpu_cycles = false,
        .average_on_n = 1,
    };

    return res;
}


#endif
