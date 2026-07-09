#ifndef OBSERVER_H
#define OBSERVER_H

#include <time.h>

typedef struct
{
    time_t timestamp;

    long vmrss_kb;
    long vmsize_kb;
    long vmswap_kb;

    int threads;
    int fdcount;

} Snapshot;

int read_status(pid_t pid, Snapshot *snap);
int count_fds(pid_t pid);
void print_snapshot(const Snapshot *snap);

#endif
