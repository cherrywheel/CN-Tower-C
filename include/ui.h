#ifndef UI_H
#define UI_H

#include <stdbool.h>

// text ui on ansi escape codes
// status on top then game text then hints and input at the bottom
// falls back to plain line by line mode when stdin or stdout isnt a terminal

void ui_init(bool allow_tui);
void ui_shutdown(void);
bool ui_active(void);

// status line with location money and items
void ui_set_status(const char *status);

// commands for hints and tab completion that last for one read
// the first primary ones are location actions and the rest are general commands
void ui_set_hints(const char **hints, int count, int primary);

// read a line from the keyboard in the tui and return false at end of input
bool ui_read_line(char *buf, int size);

#endif // UI_H
