#include "mapping.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


static int compare_rss(
    const void *a,
    const void *b)
{
    const Mapping *ma = a;
    const Mapping *mb = b;

    if (mb->rss_kb > ma->rss_kb)
        return 1;

    if (mb->rss_kb < ma->rss_kb)
        return -1;

    return 0;
}


static void parse_value(
    char *line,
    Mapping *m)
{
    long value = 0;

    if (sscanf(line, "Size: %ld kB", &value) == 1)
        m->size_kb = value;

    else if (sscanf(line, "Rss: %ld kB", &value) == 1)
        m->rss_kb = value;

    else if (sscanf(line, "Pss: %ld kB", &value) == 1)
        m->pss_kb = value;

    else if (sscanf(line, "Shared_Clean: %ld kB", &value) == 1)
        m->shared_clean_kb = value;

    else if (sscanf(line, "Shared_Dirty: %ld kB", &value) == 1)
        m->shared_dirty_kb = value;

    else if (sscanf(line, "Private_Clean: %ld kB", &value) == 1)
        m->private_clean_kb = value;

    else if (sscanf(line, "Private_Dirty: %ld kB", &value) == 1)
        m->private_dirty_kb = value;

    else if (sscanf(line, "Anonymous: %ld kB", &value) == 1)
        m->anonymous_kb = value;

    else if (sscanf(line, "Swap: %ld kB", &value) == 1)
        m->swap_kb = value;
}


static int parse_header(
    char *line,
    Mapping *m)
{
    char pathname[256] = "";

    memset(m, 0, sizeof(Mapping));


    int fields = sscanf(
        line,
        "%lx-%lx %4s %lx %15s %lu %255[^\n]",
        &m->start,
        &m->end,
        m->permissions,
        &m->offset,
        m->device,
        &m->inode,
        pathname);


    if (fields < 6)
        return 0;


    if (fields == 7)
    {
        strncpy(
            m->name,
            pathname,
            sizeof(m->name)-1);
    }
    else
    {
        strcpy(
            m->name,
            "[anonymous]");
    }


    return 1;
}



int read_mappings(
    pid_t pid,
    MappingList *list)
{
    char path[64];

    snprintf(
        path,
        sizeof(path),
        "/proc/%d/smaps",
        pid);


    FILE *fp = fopen(path, "r");

    if (!fp)
        return -1;


    list->count = 0;


    char line[512];

    Mapping *current = NULL;



    while (fgets(line, sizeof(line), fp))
    {
        Mapping temp;


        if (parse_header(line, &temp))
        {
            if (current != NULL)
                list->count++;


            if (list->count >= MAX_MAPPINGS)
                break;


            list->mappings[list->count] = temp;

            current =
                &list->mappings[list->count];

            continue;
        }


        if (current != NULL)
            parse_value(line, current);
    }


    if (current != NULL &&
        list->count < MAX_MAPPINGS)
    {
        list->count++;
    }


    fclose(fp);


    qsort(
        list->mappings,
        list->count,
        sizeof(Mapping),
        compare_rss);


    return 0;
}



void print_largest_mappings(
    const MappingList *list,
    int limit)
{
    if (limit > list->count)
        limit = list->count;


    printf("\n========== Top Mappings ==========\n");


    for (int i = 0; i < limit; i++)
    {
        const Mapping *m =
            &list->mappings[i];


        printf(
            "%2d %6ld MB RSS  %lx-%lx  %s\n",
            i + 1,
            m->rss_kb / 1024,
            m->start,
            m->end,
            m->name);
    }


    printf("==================================\n");
}



static Mapping *find_previous(
    MappingList *list,
    unsigned long start,
    unsigned long end)
{
    for (int i = 0; i < list->count; i++)
    {
        if (list->mappings[i].start == start &&
            list->mappings[i].end == end)
        {
            return &list->mappings[i];
        }
    }

    return NULL;
}



void print_mapping_growth(
    MappingHistory *history,
    const MappingList *current)
{
    printf("\n========== Mapping Growth ==========\n");


    if (!history->valid)
    {
        history->previous = *current;
        history->valid = 1;

        printf("Baseline collected.\n");
        printf("====================================\n");

        return;
    }



    for (int i = 0; i < current->count; i++)
    {
        const Mapping *now =
            &current->mappings[i];


        Mapping *old =
            find_previous(
                &history->previous,
                now->start,
                now->end);


        if (old != NULL)
        {
            long diff =
                now->rss_kb - old->rss_kb;


            if (diff > 1024)
            {
                printf(
                    "%lx-%lx %s\n"
                    " Previous : %ld MB\n"
                    " Current  : %ld MB\n"
                    " Growth   : +%ld MB\n\n",
                    now->start,
                    now->end,
                    now->name,
                    old->rss_kb / 1024,
                    now->rss_kb / 1024,
                    diff / 1024);
            }
        }
    }


    history->previous = *current;


    printf("====================================\n");
}
