#ifndef _CPU_STATS_H
#define _CPU_STATS_H

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

// ====== Tools to fetch low-level stats from the CPU and kernel ======

struct cpu_stats;

struct cpu_stats *cpu_stats_init();

void cpu_stats_begin(struct cpu_stats *stats);

struct cpu_stats_report cpu_stats_end(struct cpu_stats *stats);


// ====== Stats report ======

struct cpu_stats_report {
    double elapsed_real_time_secs;
    double elapsed_cpu_time_secs;
    uint64_t cpu_cycles;
    bool has_cpu_cycles;
    size_t average_on_n;
};

void print_cpu_stats_report(struct cpu_stats_report);

void println_cpu_stats_report(char *title, struct cpu_stats_report);

struct cpu_stats_report average_report(struct cpu_stats_report *array, size_t len);

#endif
