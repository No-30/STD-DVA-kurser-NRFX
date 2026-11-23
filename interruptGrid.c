#include "interruptGrid.h"
#include <stdio.h>
#include <string.h>

#define GRID_SIZE 10

static int cursor_x;
static int cursor_y;
static int finished;

void interrupt_grid_reset(void)
{
    cursor_x = 0;
    cursor_y = 0;
    finished = 0;
}

void interrupt_grid_handle_key(char key)
{
    switch (key)
    {
        case 'i':
        case 'I':
            if (cursor_y < GRID_SIZE - 1) cursor_y++;
            break;
        case 'k':
        case 'K':
            if (cursor_y > 0) cursor_y--;
            break;
        case 'j':
        case 'J':
            if (cursor_x > 0) cursor_x--;
            break;
        case 'l':
        case 'L':
            if (cursor_x < GRID_SIZE - 1) cursor_x++;
            break;
        case 'q':
        case 'Q':
            finished = 1;
            break;
        default:
            break;
    }
}

int interrupt_grid_is_finished(void)
{
    return finished;
}

int interrupt_grid_render(char *buffer, int buffer_length)
{
    int used = 0;

    if (buffer == 0 || buffer_length < 2) return 0;

    used += snprintf(buffer + used, buffer_length - used,
                     "\033[H\033[2JPosition: X=%d Y=%d\r\n", cursor_x, cursor_y);

    for (int y = 0; y < GRID_SIZE && used < buffer_length - 2; y++)
    {
        for (int x = 0; x < GRID_SIZE && used < buffer_length - 2; x++)
        {
            used += snprintf(buffer + used, buffer_length - used,
                             "%c", (x == cursor_x && y == cursor_y) ? 'O' : '.');
        }
        used += snprintf(buffer + used, buffer_length - used, "\r\n");
    }

    return used;
}
