#ifndef PROCESS_H
#define PROCESS_H

#include <sys/types.h>

#include "observer.h"
#include "meminfo.h"
#include "smaps.h"
#include "mapping.h"
#include "classifier.h"

typedef struct
{
    Snapshot status;

    MemInfo meminfo;

    SmapsInfo smaps;

    MappingList mappings;

    CategorySummary categories;

} ProcessSnapshot;


int collect_snapshot(
    pid_t pid,
    const char *package,
    ProcessSnapshot *snapshot);

#endif
