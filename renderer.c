#include "renderer.h"

#include <stdio.h>

void renderer_begin_frame(void)
{
    /*
     * Move cursor to the upper-left corner.
     */
    printf("\033[H");

    /*
     * Clear everything from the cursor to the end of the screen.
     */
    printf("\033[J");
}

void renderer_end_frame(void)
{
    fflush(stdout);
}

void renderer_clear(void)
{
    /*
     * Clear the entire screen and move the cursor home.
     */
    printf("\033[2J");
    printf("\033[H");
}

void renderer_home(void)
{
    printf("\033[H");
}

void renderer_separator(void)
{
    printf("============================================================\n");
}

void renderer_title(const char *title)
{
    renderer_separator();
    printf("%28s\n", title);
    renderer_separator();
}
