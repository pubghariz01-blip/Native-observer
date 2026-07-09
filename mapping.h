#ifndef MAPPING_H
#define MAPPING_H

#include <sys/types.h>

#define MAX_MAPPINGS 4096

typedef struct
{
    unsigned long start;
    unsigned long end;

    char permissions[5];

    unsigned long offset;

    char device[16];

    unsigned long inode;

    long size_kb;
    long rss_kb;
    long pss_kb;

    long shared_clean_kb;
    long shared_dirty_kb;

    long private_clean_kb;
    long private_dirty_kb;

    long anonymous_kb;
    long swap_kb;

    char name[256];

} Mapping;


typedef struct
{
    int count;

    Mapping mappings[MAX_MAPPINGS];

} MappingList;


typedef struct
{
    MappingList previous;

    int valid;

} MappingHistory;


int read_mappings(
    pid_t pid,
    MappingList *list);


void print_largest_mappings(
    const MappingList *list,
    int limit);


void print_mapping_growth(
    MappingHistory *history,
    const MappingList *current);


#endif
