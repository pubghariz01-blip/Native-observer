#include "smaps.h"

#include <stdio.h>
#include <string.h>

int read_smaps(pid_t pid, SmapsInfo *info)
{
    char path[64];
    char line[512];
    FILE *fp;

    memset(info, 0, sizeof(*info));

    snprintf(path, sizeof(path), "/proc/%d/smaps", pid);

    fp = fopen(path, "r");
    if (fp == NULL)
        return -1;

    enum Category
    {
        OTHER,
        SCUDO,
        HEAP,
        STACK,
        ASHMEM,
        LIBRARY,
        ANON
    };

    enum Category current = OTHER;

    while (fgets(line, sizeof(line), fp))
    {
        if (strchr(line, '-') != NULL)
        {
            current = OTHER;

            if (strstr(line, "[anon:scudo:secondary]"))
            {
                current = SCUDO;
                info->scudo_regions++;
            }
            else if (strstr(line, "[heap]"))
            {
                current = HEAP;
                info->heap_regions++;
            }
            else if (strstr(line, "[stack"))
            {
                current = STACK;
                info->stack_regions++;
            }
            else if (strstr(line, "ashmem"))
            {
                current = ASHMEM;
                info->ashmem_regions++;
            }
            else if (strstr(line, ".so"))
            {
                current = LIBRARY;
                info->so_regions++;
            }
            else if (strstr(line, "[anon"))
            {
                current = ANON;
                info->anon_regions++;
            }

            continue;
        }

        long rss;

        if (sscanf(line, "Rss: %ld kB", &rss) == 1)
        {
            switch (current)
            {
                case SCUDO:
                    info->scudo_rss_kb += rss;
                    break;

                case HEAP:
                    info->heap_rss_kb += rss;
                    break;

                case STACK:
                    info->stack_rss_kb += rss;
                    break;

                case ASHMEM:
                    info->ashmem_rss_kb += rss;
                    break;

                case LIBRARY:
                    info->so_rss_kb += rss;
                    break;

                case ANON:
                    info->anon_rss_kb += rss;
                    break;

                default:
                    info->other_rss_kb += rss;
                    break;
            }
        }
    }

    fclose(fp);
    return 0;
}

void print_smaps(const SmapsInfo *info)
{
    printf("\n");
    printf("========== SMAPS ==========\n");
    printf("Scudo    : %5ld MB (%d)\n",
           info->scudo_rss_kb / 1024,
           info->scudo_regions);

    printf("Anon     : %5ld MB (%d)\n",
           info->anon_rss_kb / 1024,
           info->anon_regions);

    printf("Heap      : %5ld MB (%d)\n",
           info->heap_rss_kb / 1024,
           info->heap_regions);

    printf("Libraries : %5ld MB (%d)\n",
           info->so_rss_kb / 1024,
           info->so_regions);

    printf("Ashmem    : %5ld MB (%d)\n",
           info->ashmem_rss_kb / 1024,
           info->ashmem_regions);

    printf("Stack     : %5ld MB (%d)\n",
           info->stack_rss_kb / 1024,
           info->stack_regions);

    printf("Other     : %5ld MB\n",
           info->other_rss_kb / 1024);

    printf("===========================\n");
}
