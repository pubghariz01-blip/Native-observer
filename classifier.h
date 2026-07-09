#ifndef CLASSIFIER_H
#define CLASSIFIER_H

#include "mapping.h"

typedef enum
{
    CAT_SCUDO_PRIMARY = 0,
    CAT_SCUDO_SECONDARY,
    CAT_NATIVE_HEAP,
    CAT_ANON,
    CAT_LIBRARIES,
    CAT_APK,
    CAT_APEX,
    CAT_ART,
    CAT_JIT,
    CAT_STACK,
    CAT_ASHMEM,
    CAT_MEMFD,
    CAT_DMABUF,
    CAT_ION,
    CAT_KGSL,
    CAT_EGL,
    CAT_GL,
    CAT_VULKAN,
    CAT_BINDER,
    CAT_CURSOR,
    CAT_DEVICE,
    CAT_OTHER,

    CATEGORY_COUNT

} MemoryCategory;


typedef struct
{
    const char *name;

    long rss_kb;
    long pss_kb;
    long size_kb;

    int mappings;

} CategoryStat;


typedef struct
{
    CategoryStat category[CATEGORY_COUNT];

} CategorySummary;


void classify_mappings(
    const MappingList *list,
    CategorySummary *summary);


void print_category_summary(
    const CategorySummary *summary);

#endif
