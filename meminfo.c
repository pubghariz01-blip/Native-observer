#include "meminfo.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int read_meminfo(const char *package_name, MemInfo *info)
{
    char cmd[256];
    FILE *fp;
    char line[512];

    memset(info, 0, sizeof(MemInfo));

    snprintf(cmd, sizeof(cmd),
             "su -c \"dumpsys meminfo %s\"",
             package_name);

    fp = popen(cmd, "r");
    if (!fp)
        return -1;

    while (fgets(line, sizeof(line), fp))
    {
        long pss, rss, heap_size, heap_alloc, heap_free;

        if (sscanf(line,
                   " Native Heap %ld %*ld %*ld %*ld %ld %ld %ld %ld",
                   &pss,
                   &rss,
                   &heap_size,
                   &heap_alloc,
                   &heap_free) == 5)
        {
            info->native_pss_kb = pss;
            info->native_rss_kb = rss;
            info->heap_size_kb = heap_size;
            info->heap_alloc_kb = heap_alloc;
            info->heap_free_kb = heap_free;
            continue;
        }

        if (sscanf(line,
                   " Native Heap: %ld %ld",
                   &pss,
                   &rss) == 2)
        {
            info->native_pss_kb = pss;
            info->native_rss_kb = rss;
            continue;
        }

        if (sscanf(line,
                   " Graphics: %ld",
                   &pss) == 1)
        {
            info->graphics_kb = pss;
            continue;
        }

        if (sscanf(line,
                   " GL mtrack %ld",
                   &pss) == 1)
        {
            info->gl_kb = pss;
            continue;
        }

        if (sscanf(line,
                   " EGL mtrack %ld",
                   &pss) == 1)
        {
            info->egl_kb = pss;
            continue;
        }
    }

    pclose(fp);
    return 0;
}

void print_meminfo(const MemInfo *info)
{
    printf("\n");
    printf("====== Native Memory ======\n");

    printf("Native PSS : %ld MB\n", info->native_pss_kb / 1024);
    printf("Native RSS : %ld MB\n", info->native_rss_kb / 1024);

    printf("Heap Size  : %ld MB\n", info->heap_size_kb / 1024);
    printf("Heap Alloc : %ld MB\n", info->heap_alloc_kb / 1024);
    printf("Heap Free  : %ld MB\n", info->heap_free_kb / 1024);

    printf("Graphics   : %ld MB\n", info->graphics_kb / 1024);
    printf("GL mtrack  : %ld MB\n", info->gl_kb / 1024);
    printf("EGL mtrack : %ld MB\n", info->egl_kb / 1024);

    printf("===========================\n");
}
