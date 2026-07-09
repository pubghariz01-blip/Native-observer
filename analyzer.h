#ifndef ANALYZER_H
#define ANALYZER_H

#include "process.h"

typedef struct
{
    int initialized;

    ProcessSnapshot last;

} Analyzer;

void analyzer_init(Analyzer *a);

void analyzer_print_delta(
    Analyzer *a,
    const ProcessSnapshot *current);

#endif
