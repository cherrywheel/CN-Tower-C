#ifndef UI_H
#define UI_H

#include <stdbool.h>

// Текстовый интерфейс (TUI) на ANSI-последовательностях:
// сверху строка статуса, в середине текст игры, внизу подсказки и ввод.
// Если stdin/stdout не терминал, всё работает в обычном построчном режиме.

void ui_init(bool allow_tui);
void ui_shutdown(void);
bool ui_active(void);

// Строка статуса (локация, деньги, предметы)
void ui_set_status(const char *status);

// Команды для подсказок и автодополнения. Действуют на одно чтение строки.
// Первые primary - действия в локации, остальные - общие команды.
void ui_set_hints(const char **hints, int count, int primary);

// Прочитать строку с клавиатуры в TUI. false - ввод закончился.
bool ui_read_line(char *buf, int size);

#endif // UI_H
