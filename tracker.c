#include "tracker.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


static int find_entry(
    const LeakTracker *tracker,
    const char *key)
{
    for (int i = 0; i < tracker->count; i++)
    {
        if (strcmp(
                tracker->entry[i].key,
                key) == 0)
        {
            return i;
        }
    }

    return -1;
}


void tracker_init(
    LeakTracker *tracker,
    const MappingList *baseline)
{
    memset(
        tracker,
        0,
        sizeof(LeakTracker));

    for (int i = 0;
         i < baseline->count && i < TRACKER_MAX;
         i++)
    {
        const Mapping *m =
            &baseline->mappings[i];

        LeakEntry *e =
            &tracker->entry[tracker->count++];

        snprintf(
            e->key,
            sizeof(e->key),
            "%016lx-%016lx",
            m->start,
            m->end);

        strncpy(
            e->name,
            m->name,
            sizeof(e->name) - 1);

        e->start_rss_kb =
            m->rss_kb;

        e->current_rss_kb =
            m->rss_kb;

        e->peak_rss_kb =
            m->rss_kb;

        e->growth_kb = 0;

        e->updates = 0;
    }
}


void tracker_update(
    LeakTracker *tracker,
    const MappingList *current)
{
    for (int i = 0; i < current->count; i++)
    {
        const Mapping *m =
            &current->mappings[i];

        char key[256];

        snprintf(
            key,
            sizeof(key),
            "%016lx-%016lx",
            m->start,
            m->end);

        int index =
            find_entry(
                tracker,
                key);

        if (index < 0)
            continue;

        LeakEntry *e =
            &tracker->entry[index];

        e->current_rss_kb =
            m->rss_kb;

        if (m->rss_kb > e->peak_rss_kb)
            e->peak_rss_kb =
                m->rss_kb;

        if (m->rss_kb >= e->start_rss_kb)
            e->growth_kb =
                m->rss_kb -
                e->start_rss_kb;
        else
            e->growth_kb = 0;

        e->updates++;
    }
}

static int compare_growth(
    const void *a,
    const void *b)
{
    const LeakEntry *ea =
        (const LeakEntry *)a;

    const LeakEntry *eb =
        (const LeakEntry *)b;

    if (ea->growth_kb < eb->growth_kb)
        return 1;

    if (ea->growth_kb > eb->growth_kb)
        return -1;

    return 0;
}
void tracker_print_top(
    const LeakTracker *tracker,
    int limit)
{
    printf("DEBUG: tracker count = %d\n", tracker->count);

    if (tracker->count < 0 ||
        tracker->count > TRACKER_MAX)
    {
        printf("DEBUG: INVALID TRACKER COUNT\n");
        return;
    }

    LeakEntry copy[TRACKER_MAX];

    memcpy(
        copy,
        tracker->entry,
        sizeof(LeakEntry) * tracker->count);

    int count = tracker->count;

    if (count > TRACKER_MAX)
    {
        count = TRACKER_MAX;
    }

    memcpy(
        copy,
        tracker->entry,
        sizeof(LeakEntry) * count);

qsort(
    copy,
    count,
        sizeof(LeakEntry),
        compare_growth);

    printf("\n");
    printf("========== LEAK CANDIDATES ==========\n");

    int shown = 0;

    for (int i = 0;
         i < count && shown < limit;
         i++)
    {
        if (copy[i].growth_kb == 0)
            continue;

        printf(
            "%2d  +%4lu MB\n",
            shown + 1,
            copy[i].growth_kb / 1024);

        printf(
            "    %s\n",
            copy[i].key);

        printf(
            "    %s\n",
            copy[i].name);

        printf(
            "    Start   : %4lu MB\n",
            copy[i].start_rss_kb / 1024);

        printf(
            "    Current : %4lu MB\n",
            copy[i].current_rss_kb / 1024);

        printf(
            "    Peak    : %4lu MB\n",
            copy[i].peak_rss_kb / 1024);

        printf(
            "    Updates : %u\n\n",
            copy[i].updates);

        shown++;
    }

    if (shown == 0)
    {
        printf("No growing mappings detected yet.\n");
    }

    printf("=====================================\n");
}
