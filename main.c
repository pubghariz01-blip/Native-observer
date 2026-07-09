#include "process.h"
#include "mapping.h"
#include "dashboard.h"
#include "tracker.h"

#include <stdio.h>
#include <unistd.h>

int main(void)
{
    pid_t pid;

    char package[128];

    ProcessSnapshot baseline;
    ProcessSnapshot current;

    MappingHistory history = {0};

    LeakTracker tracker;

    printf("Enter PID to monitor: ");
    fflush(stdout);

    if (scanf("%d", &pid) != 1)
    {
        printf("Invalid PID.\n");
        return 1;
    }

    printf("Enter package name (example: sts.al): ");
    fflush(stdout);

    if (scanf("%127s", package) != 1)
    {
        printf("Invalid package.\n");
        return 1;
    }

    if (collect_snapshot(
            pid,
            package,
            &baseline) != 0)
    {
        printf("Unable to collect baseline.\n");
        return 1;
    }

    tracker_init(
        &tracker,
        &baseline.mappings);

    dashboard_init(
        pid,
        package,
        &baseline);

    while (1)
    {
        if (collect_snapshot(
                pid,
                package,
                &current) != 0)
        {
            printf("\nProcess ended.\n");
            break;
        }

        tracker_update(
            &tracker,
            &current.mappings);

        printf("DEBUG: before dashboard_update\n");
        fflush(stdout);

        dashboard_update(
            &current,
            &history);

        printf("DEBUG: after dashboard_update\n");
        fflush(stdout);

        printf("A\n");
        fflush(stdout);

        tracker_print_top(
            &tracker,
            10);

        printf("B\n");
        fflush(stdout);

        printf("DEBUG: after tracker_print_top\n");
        fflush(stdout);

        sleep(2);
    }

    return 0;
}
