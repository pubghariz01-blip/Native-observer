#ifndef TRACKER_H
#define TRACKER_H

#include "mapping.h"

#define TRACKER_MAX 4096

typedef struct
{
    char key[256];

    char name[256];

    unsigned long start_rss_kb;

    unsigned long current_rss_kb;

    unsigned long peak_rss_kb;

    unsigned long growth_kb;

    unsigned int updates;

} LeakEntry;


typedef struct
{
    LeakEntry entry[TRACKER_MAX];

    int count;

} LeakTracker;


void tracker_init(
    LeakTracker *tracker,
    const MappingList *baseline);

void tracker_update(
    LeakTracker *tracker,
    const MappingList *current);

void tracker_print_top(
    const LeakTracker *tracker,
    int limit);

#endif
