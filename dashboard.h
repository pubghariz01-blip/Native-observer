#ifndef DASHBOARD_H
#define DASHBOARD_H

#include "process.h"
#include "mapping.h"

void dashboard_init(
    pid_t pid,
    const char *package,
    const ProcessSnapshot *baseline);

void dashboard_update(
    const ProcessSnapshot *current,
    MappingHistory *history);

#endif
