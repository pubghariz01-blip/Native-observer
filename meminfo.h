#ifndef MEMINFO_H
#define MEMINFO_H

typedef struct
{
    long native_pss_kb;
    long native_rss_kb;
    long heap_size_kb;
    long heap_alloc_kb;
    long heap_free_kb;

    long graphics_kb;
    long gl_kb;
    long egl_kb;

} MemInfo;

int read_meminfo(const char *package_name, MemInfo *info);
void print_meminfo(const MemInfo *info);

#endif
