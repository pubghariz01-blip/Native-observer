#include "analyzer.h"

#include <stdio.h>
#include <string.h>

static long delta(long current, long previous)
{
    return current - previous;
}

void analyzer_init(Analyzer *a)
{
    memset(a, 0, sizeof(*a));
}

void analyzer_print_delta(
    Analyzer *a,
    const ProcessSnapshot *current)
{
    if (!a->initialized)
    {
        a->last = *current;
        a->initialized = 1;

        printf("\n========== Growth ==========\n");
        printf("Baseline established.\n");
        printf("============================\n");

        return;
    }

    printf("\n========== Growth ==========\n");

    printf("VmRSS        %+6ld MB\n",
           delta(current->status.vmrss_kb,
                 a->last.status.vmrss_kb) / 1024);

    printf("Native Heap  %+6ld MB\n",
           delta(current->meminfo.heap_alloc_kb,
                 a->last.meminfo.heap_alloc_kb) / 1024);

    printf("Anonymous    %+6ld MB\n",
           delta(current->smaps.anon_rss_kb,
                 a->last.smaps.anon_rss_kb) / 1024);

    printf("Scudo        %+6ld MB\n",
           delta(current->smaps.scudo_rss_kb,
                 a->last.smaps.scudo_rss_kb) / 1024);

    printf("Graphics     %+6ld MB\n",
           delta(current->meminfo.graphics_kb,
                 a->last.meminfo.graphics_kb) / 1024);

    printf("============================\n");

    a->last = *current;
}
