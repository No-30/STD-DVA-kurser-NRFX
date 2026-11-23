#ifndef INTERRUPT_GRID_H
#define INTERRUPT_GRID_H

void interrupt_grid_reset(void);
void interrupt_grid_handle_key(char key);
int interrupt_grid_is_finished(void);
int interrupt_grid_render(char *buffer, int buffer_length);

#endif
