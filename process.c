#include "process.h"

#include <string.h>

int collect_snapshot(
    pid_t pid,
    const char *package,
    ProcessSnapshot *snapshot)
{
    memset(
        snapshot,
        0,
        sizeof(ProcessSnapshot));

    /*
     * Basic process status
     */
    if (read_status(
            pid,
            &snapshot->status) != 0)
    {
        return -1;
    }

    /*
     * Android dumpsys meminfo
     */
    if (read_meminfo(
            package,
            &snapshot->meminfo) != 0)
    {
        return -1;
    }

    /*
     * smaps totals
     */
    if (read_smaps(
            pid,
            &snapshot->smaps) != 0)
    {
        return -1;
    }

    /*
     * Individual mappings
     */
    if (read_mappings(
            pid,
            &snapshot->mappings) != 0)
    {
        return -1;
    }

    /*
     * Build category statistics
     */
    classify_mappings(
        &snapshot->mappings,
        &snapshot->categories);

    return 0;
}
