#include "dashboard.h"
#include "renderer.h"
#include "classifier.h"

#include <stdio.h>
#include <string.h>
#include <time.h>

static ProcessSnapshot g_baseline;

static pid_t g_pid;

static char g_package[128];

static char g_started[64];

static int g_initialized = 0;


static void print_header(void)
{
    renderer_separator();
    printf("                  NATIVE OBSERVER\n");
    renderer_separator();

    printf("PID      : %d\n", g_pid);
    printf("Package  : %s\n", g_package);
    printf("Started  : %s\n", g_started);
    printf("Refresh  : 2 seconds\n");

    renderer_separator();
    printf("\n");
}


static void print_baseline(void)
{
    printf("================ BASELINE =================\n\n");

    print_snapshot(
        &g_baseline.status);

    print_largest_mappings(
        &g_baseline.mappings,
        10);

    print_category_summary(
        &g_baseline.categories);

    printf("\n");
}


static void print_live(
    const ProcessSnapshot *snapshot,
    MappingHistory *history)
{
    printf("================ LIVE =====================\n\n");

    print_snapshot(
        &snapshot->status);

    print_largest_mappings(
        &snapshot->mappings,
        10);

    print_category_summary(
        &snapshot->categories);

    print_mapping_growth(
        history,
        &snapshot->mappings);

    printf("\n");
}


void dashboard_init(
    pid_t pid,
    const char *package,
    const ProcessSnapshot *baseline)
{
    time_t now;

    struct tm *tm_info;

    memset(
        &g_baseline,
        0,
        sizeof(ProcessSnapshot));

    memcpy(
        &g_baseline,
        baseline,
        sizeof(ProcessSnapshot));

    g_pid = pid;

    strncpy(
        g_package,
        package,
        sizeof(g_package) - 1);

    g_package[sizeof(g_package) - 1] = '\0';

    now = time(NULL);

    tm_info = localtime(&now);

    strftime(
        g_started,
        sizeof(g_started),
        "%H:%M:%S",
        tm_info);

    g_initialized = 1;

    renderer_clear();

    print_header();

    print_baseline();

    renderer_end_frame();
}


void dashboard_update(
    const ProcessSnapshot *snapshot,
    MappingHistory *history)
{
    if (!g_initialized)
    {
        return;
    }

    renderer_begin_frame();

    print_header();

    print_baseline();

    print_live(
        snapshot,
        history);

    renderer_end_frame();
}
