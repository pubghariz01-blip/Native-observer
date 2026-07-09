#include "classifier.h"

#include <stdio.h>
#include <string.h>


static const char *category_names[CATEGORY_COUNT] =
{
    "Scudo Primary",
    "Scudo Secondary",
    "Native Heap",
    "Anonymous",
    "Native Libraries",
    "APK",
    "APEX",
    "ART",
    "JIT",
    "Stack",
    "Ashmem",
    "memfd",
    "DMA-BUF",
    "ION",
    "KGSL",
    "EGL",
    "OpenGL",
    "Vulkan",
    "Binder",
    "CursorWindow",
    "Device",
    "Other"
};


static MemoryCategory detect_category(
    const Mapping *m)
{
    const char *n = m->name;

    if (strstr(n, "scudo:primary"))
        return CAT_SCUDO_PRIMARY;

    if (strstr(n, "scudo:secondary"))
        return CAT_SCUDO_SECONDARY;

    if (strstr(n, "[heap]"))
        return CAT_NATIVE_HEAP;

    if (strstr(n, "[anon"))
        return CAT_ANON;

    if (strstr(n, ".so"))
        return CAT_LIBRARIES;

    if (strstr(n, ".apk"))
        return CAT_APK;

    if (strstr(n, "/apex/"))
        return CAT_APEX;

    if (strstr(n, "boot.art"))
        return CAT_ART;

    if (strstr(n, "jit"))
        return CAT_JIT;

    if (strstr(n, "[stack"))
        return CAT_STACK;

    if (strstr(n, "ashmem"))
        return CAT_ASHMEM;

    if (strstr(n, "memfd"))
        return CAT_MEMFD;

    if (strstr(n, "dmabuf"))
        return CAT_DMABUF;

    if (strstr(n, "ion"))
        return CAT_ION;

    if (strstr(n, "kgsl"))
        return CAT_KGSL;

    if (strstr(n, "egl"))
        return CAT_EGL;

    if (strstr(n, "libEGL"))
        return CAT_EGL;

    if (strstr(n, "gles"))
        return CAT_GL;

    if (strstr(n, "libGLES"))
        return CAT_GL;

    if (strstr(n, "vulkan"))
        return CAT_VULKAN;

    if (strstr(n, "binder"))
        return CAT_BINDER;

    if (strstr(n, "CursorWindow"))
        return CAT_CURSOR;

    if (strncmp(n, "/dev/", 5) == 0)
        return CAT_DEVICE;

    return CAT_OTHER;
}



void classify_mappings(
    const MappingList *list,
    CategorySummary *summary)
{
    memset(
        summary,
        0,
        sizeof(CategorySummary));


    for (int i = 0; i < CATEGORY_COUNT; i++)
    {
        summary->category[i].name =
            category_names[i];
    }


    for (int i = 0; i < list->count; i++)
    {
        const Mapping *m =
            &list->mappings[i];


        MemoryCategory cat =
            detect_category(m);


        CategoryStat *s =
            &summary->category[cat];


        s->rss_kb += m->rss_kb;
        s->pss_kb += m->pss_kb;
        s->size_kb += m->size_kb;
        s->mappings++;
    }
}



void print_category_summary(
    const CategorySummary *summary)
{
    printf("\n========== Category Summary ==========\n");


    for (int i = 0; i < CATEGORY_COUNT; i++)
    {
        const CategoryStat *s =
            &summary->category[i];


        if (s->mappings == 0)
            continue;


        printf(
            "%-18s "
            "RSS:%6ld MB  "
            "PSS:%6ld MB  "
            "Size:%6ld MB  "
            "Maps:%4d\n",

            s->name,

            s->rss_kb / 1024,
            s->pss_kb / 1024,
            s->size_kb / 1024,

            s->mappings);
    }


    printf("======================================\n");
}
