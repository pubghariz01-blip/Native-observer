#include "observer.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <limits.h>
#include <unistd.h>

int count_fds(pid_t pid)
{
    char path[PATH_MAX];

    snprintf(path, sizeof(path), "/proc/%d/fd", pid);

    DIR *dir = opendir(path);

    if (dir == NULL)
        return -1;

    int count = 0;
    struct dirent *entry;

    while ((entry = readdir(dir)) != NULL)
    {
        if (strcmp(entry->d_name, ".") == 0)
            continue;

        if (strcmp(entry->d_name, "..") == 0)
            continue;

        count++;
    }

    closedir(dir);

    return count;
}

int read_status(pid_t pid, Snapshot *snap)
{
    char path[PATH_MAX];

    snprintf(path, sizeof(path), "/proc/%d/status", pid);

    FILE *fp = fopen(path, "r");

    if (fp == NULL)
        return -1;

    memset(snap, 0, sizeof(Snapshot));

    snap->timestamp = time(NULL);

    char line[512];

    while (fgets(line, sizeof(line), fp) != NULL)
    {
        if (sscanf(line, "VmRSS: %ld kB", &snap->vmrss_kb) == 1)
            continue;

        if (sscanf(line, "VmSize: %ld kB", &snap->vmsize_kb) == 1)
            continue;

        if (sscanf(line, "VmSwap: %ld kB", &snap->vmswap_kb) == 1)
            continue;

        if (sscanf(line, "Threads: %d", &snap->threads) == 1)
            continue;
    }

    fclose(fp);

    snap->fdcount = count_fds(pid);

    return 0;
}

void print_snapshot(const Snapshot *snap)
{
    char timebuf[32];

    struct tm *tm_info = localtime(&snap->timestamp);

    if (tm_info != NULL)
        strftime(timebuf, sizeof(timebuf), "%H:%M:%S", tm_info);
    else
        strcpy(timebuf, "??:??:??");

    printf("\n");
    printf("========== Snapshot ==========\n");
    printf("Time     : %s\n", timebuf);
    printf("VmRSS    : %ld MB\n", snap->vmrss_kb / 1024);
    printf("VmSize   : %ld MB\n", snap->vmsize_kb / 1024);
    printf("VmSwap   : %ld MB\n", snap->vmswap_kb / 1024);
    printf("Threads  : %d\n", snap->threads);
    printf("FDs      : %d\n", snap->fdcount);
    printf("==============================\n");

    fflush(stdout);
}
