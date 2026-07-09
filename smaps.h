#ifndef SMAPS_H
#define SMAPS_H

#include <sys/types.h>

typedef struct
{
    long scudo_rss_kb;
    long anon_rss_kb;
    long heap_rss_kb;
    long stack_rss_kb;
    long ashmem_rss_kb;
    long so_rss_kb;
    long other_rss_kb;

    int scudo_regions;
    int anon_regions;
    int heap_regions;
    int stack_regions;
    int ashmem_regions;
    int so_regions;
    int other_regions;

} SmapsInfo;

int read_smaps(pid_t pid, SmapsInfo *info);
void print_smaps(const SmapsInfo *info);

#endif
