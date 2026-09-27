#ifndef _WIN32
#define _POSIX_C_SOURCE 200809L // Для fileno, isatty и termios
#define _DARWIN_C_SOURCE        // На macOS без этого не видно struct winsize и TIOCGWINSZ
#endif

#include "ui.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#ifdef _WIN32
#include <windows.h>
#include <conio.h>
#include <io.h>
#ifndef ENABLE_VIRTUAL_TERMINAL_PROCESSING
#define ENABLE_VIRTUAL_TERMINAL_PROCESSING 0x0004
#endif
#else
#include <termios.h>
#include <unistd.h>
#include <sys/ioctl.h>
#endif

#define MAX_HINTS 32
#define HISTORY_SIZE 50
#define LINE_LEN 100
#define MIN_ROWS 10

// Коды клавиш помимо обычных символов
enum {
    KEY_UP = 1000,
    KEY_DOWN,
    KEY_LEFT,
    KEY_RIGHT,
    KEY_OTHER
};

static bool active = false;
static char status[256] = "";
static const char *hints[MAX_HINTS];
static int hint_count = 0;
static int hint_primary = 0;
static char history[HISTORY_SIZE][LINE_LEN];
static int history_count = 0;
static int screen_rows = 0;
static int screen_cols = 0;

#ifdef _WIN32
static HANDLE out_handle;
static HANDLE in_handle;
static DWORD saved_out_mode;
static DWORD saved_in_mode;
static bool in_mode_saved = false;
#else
static struct termios saved_termios;
#endif

bool ui_active(void) {
    return active;
}

void ui_set_status(const char *text) {
    snprintf(status, sizeof(status), "%s", text);
}

void ui_set_hints(const char **list, int count, int primary) {
    hint_count = count < MAX_HINTS ? count : MAX_HINTS;
    hint_primary = primary < hint_count ? primary : hint_count;
    for (int i = 0; i < hint_count; i++) {
        hints[i] = list[i];
    }
}

// --- Работа с терминалом ---

static void get_screen_size(int *rows, int *cols) {
    *rows = 24;
    *cols = 80;
#ifdef _WIN32
    CONSOLE_SCREEN_BUFFER_INFO info;
    if (GetConsoleScreenBufferInfo(out_handle, &info)) {
        *rows = info.srWindow.Bottom - info.srWindow.Top + 1;
        *cols = info.srWindow.Right - info.srWindow.Left + 1;
    }
#else
    struct winsize ws;
    if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws) == 0 && ws.ws_row > 0 && ws.ws_col > 0) {
        *rows = ws.ws_row;
        *cols = ws.ws_col;
    }
#endif
}

// Прочитать одну клавишу. -1 - ввод закончился.
static int read_key(void) {
#ifdef _WIN32
    int c = _getch();
    if (c == 0 || c == 0xE0) { // Стрелки и прочие особые клавиши
        switch (_getch()) {
            case 72: return KEY_UP;
            case 80: return KEY_DOWN;
            case 75: return KEY_LEFT;
            case 77: return KEY_RIGHT;
            default: return KEY_OTHER;
        }
    }
    return c;
#else
    unsigned char c;
    if (read(STDIN_FILENO, &c, 1) != 1) return -1;
    if (c != 27) return c;

    // Escape-последовательность вида ESC [ A
    unsigned char seq[2];
    if (read(STDIN_FILENO, &seq[0], 1) != 1) return -1;
    if (seq[0] != '[' && seq[0] != 'O') return KEY_OTHER;
    if (read(STDIN_FILENO, &seq[1], 1) != 1) return -1;
    switch (seq[1]) {
        case 'A': return KEY_UP;
        case 'B': return KEY_DOWN;
        case 'C': return KEY_RIGHT;
        case 'D': return KEY_LEFT;
    }
    // Дочитать хвост длинных последовательностей вроде ESC [ 3 ~
    while (seq[1] >= '0' && seq[1] <= '9') {
        if (read(STDIN_FILENO, &seq[1], 1) != 1) return -1;
    }
    return KEY_OTHER;
#endif
}

// Строки экрана: 1 - статус, 2..rows-3 - текст игры,
// rows-2 - действия в локации, rows-1 - общие команды, rows - ввод.
static void setup_screen(int rows, int cols) {
    screen_rows = rows;
    screen_cols = cols;
    printf("\x1b[%d;%dr", 2, rows - 3); // Прокручивается только текст игры
}

static void restore_terminal(void) {
#ifdef _WIN32
    SetConsoleMode(out_handle, saved_out_mode);
    if (in_mode_saved) SetConsoleMode(in_handle, saved_in_mode);
#else
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &saved_termios);
#endif
}

void ui_init(bool allow_tui) {
    if (!allow_tui) return;

#ifdef _WIN32
    if (!_isatty(_fileno(stdin)) || !_isatty(_fileno(stdout))) return;
    out_handle = GetStdHandle(STD_OUTPUT_HANDLE);
    if (!GetConsoleMode(out_handle, &saved_out_mode)) return;
    if (!SetConsoleMode(out_handle, saved_out_mode | ENABLE_VIRTUAL_TERMINAL_PROCESSING)) {
        return; // Старая консоль без ANSI - остаёмся в обычном режиме
    }
    // Ctrl+C приходит как обычная клавиша, чтобы выйти и вернуть экран
    in_handle = GetStdHandle(STD_INPUT_HANDLE);
    if (GetConsoleMode(in_handle, &saved_in_mode)) {
        in_mode_saved = true;
        SetConsoleMode(in_handle, saved_in_mode & ~(DWORD)ENABLE_PROCESSED_INPUT);
    }
#else
    if (!isatty(STDIN_FILENO) || !isatty(STDOUT_FILENO)) return;
    const char *term = getenv("TERM");
    if (term == NULL || strcmp(term, "dumb") == 0) return;
    if (tcgetattr(STDIN_FILENO, &saved_termios) != 0) return;
    struct termios raw = saved_termios;
    raw.c_lflag &= ~(tcflag_t)(ICANON | ECHO | ISIG | IEXTEN);
    raw.c_cc[VMIN] = 1;
    raw.c_cc[VTIME] = 0;
    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw) != 0) return;
#endif

    int rows, cols;
    get_screen_size(&rows, &cols);
    if (rows < MIN_ROWS) { // Слишком маленькое окно - остаёмся в обычном режиме
        restore_terminal();
        return;
    }

    active = true;
    atexit(ui_shutdown);
    printf("\x1b[?1049h\x1b[2J"); // Отдельный экран, как у полноэкранных программ
    setup_screen(rows, cols);
    printf("\x1b[2;1H");
    fflush(stdout);
}

void ui_shutdown(void) {
    if (!active) return;
    active = false;
    printf("\x1b[r\x1b[?1049l"); // Вернуть обычный экран
    fflush(stdout);
    restore_terminal();
}

// --- Подсказки и дополнение ---

static bool starts_with_ci(const char *text, const char *prefix) {
    for (; *prefix != '\0'; text++, prefix++) {
        if (tolower((unsigned char)*text) != tolower((unsigned char)*prefix)) return false;
    }
    return true;
}

// Хвост первой подсказки, которая начинается с набранного текста
static const char *find_ghost(const char *buf) {
    size_t len = strlen(buf);
    if (len == 0) return "";
    for (int i = 0; i < hint_count; i++) {
        if (strlen(hints[i]) > len && starts_with_ci(hints[i], buf)) {
            return hints[i] + len;
        }
    }
    return "";
}

// Вывести text, но не больше width символов. Возвращает, сколько выведено.
static int put_clipped(const char *text, int width) {
    int len = (int)strlen(text);
    if (len > width) len = width;
    if (len > 0) printf("%.*s", len, text);
    return len;
}

// Одна строка подсказок. Не влезающие команды не рвутся, а заменяются на "...".
static void draw_hint_row(int row, const char *label, int from, int to, const char *color) {
    int cols = screen_cols;
    printf("\x1b[%d;1H\x1b[2K", row);
    if (from >= to) return;
    int used = put_clipped(label, cols);
    for (int i = from; i < to; i++) {
        int need = (int)strlen(hints[i]) + (i > from ? 2 : 0);
        if (used + need > cols - 4 && i < to - 1) { // Место под "..." в конце
            used += put_clipped(" ...", cols - used);
            break;
        }
        if (i > from) used += put_clipped("  ", cols - used);
        printf("%s", color);
        used += put_clipped(hints[i], cols - used);
        printf("\x1b[0m");
    }
}

static void draw_chrome(const char *buf) {
    int cols = screen_cols;
    int rows = screen_rows;

    // Статус
    printf("\x1b[1;1H\x1b[2K\x1b[7m ");
    int used = 1 + put_clipped(status, cols - 1);
    for (; used < cols; used++) putchar(' ');
    printf("\x1b[0m");

    // Подсказки: действия в локации и общие команды
    draw_hint_row(rows - 2, "Actions: ", 0, hint_primary, "\x1b[1;36m");
    draw_hint_row(rows - 1, "Also:    ", hint_primary, hint_count, "\x1b[2m");
    if (hint_count == 0) { // Вопрос без вариантов команд (возраст, выбор, меню отладки)
        printf("\x1b[%d;1H\x1b[2m", rows - 2);
        put_clipped("Type your answer and press Enter.", cols);
        printf("\x1b[0m");
    }

    // Ввод с серым продолжением подсказки
    const char *ghost = find_ghost(buf);
    int width = cols - 3; // "> " и место под курсор
    int len = (int)strlen(buf);
    const char *visible = len > width ? buf + (len - width) : buf; // Длинная строка - видно конец
    printf("\x1b[%d;1H\x1b[2K\x1b[1m> \x1b[0m", rows);
    int shown = put_clipped(visible, width);
    printf("\x1b[90m");
    put_clipped(ghost, width - shown);
    printf("\x1b[0m\x1b[%d;%dH", rows, 3 + shown);
    fflush(stdout);
}

bool ui_read_line(char *buf, int size) {
    char line[LINE_LEN] = "";
    int len = 0;
    int history_pos = history_count;
    char tab_prefix[LINE_LEN] = "";
    int tab_index = -1; // -1 - дополнение не начато
    bool resized = false;

    if (size > LINE_LEN) size = LINE_LEN;

    printf("\x1b" "7"); // Запомнить, где остановился текст игры
    for (;;) {
        int rows, cols;
        get_screen_size(&rows, &cols);
        if (rows != screen_rows || cols != screen_cols) {
            printf("\x1b[2J");
            setup_screen(rows, cols);
            resized = true;
        }
        draw_chrome(line);

        int key = read_key();
        if (key == -1 || key == 3 || key == 26 || (key == 4 && len == 0)) {
            // Конец ввода, Ctrl+C, Ctrl+Z или Ctrl+D на пустой строке
            printf("\x1b" "8");
            ui_set_hints(NULL, 0, 0);
            return false;
        }

        if (key == '\t') {
            if (tab_index < 0) {
                snprintf(tab_prefix, sizeof(tab_prefix), "%s", line);
                tab_index = 0;
            }
            // Перебор подсказок, подходящих к тому, что было набрано до Tab
            int matches = 0;
            for (int i = 0; i < hint_count; i++) {
                if (starts_with_ci(hints[i], tab_prefix)) matches++;
            }
            if (matches > 0) {
                int wanted = tab_index % matches;
                for (int i = 0, n = 0; i < hint_count; i++) {
                    if (!starts_with_ci(hints[i], tab_prefix)) continue;
                    if (n++ == wanted) {
                        snprintf(line, sizeof(line), "%s", hints[i]);
                        len = (int)strlen(line);
                        break;
                    }
                }
                tab_index++;
            }
            continue;
        }
        tab_index = -1;

        if (key == '\r' || key == '\n') {
            break;
        } else if (key == KEY_RIGHT) {
            // Принять серую подсказку
            const char *ghost = find_ghost(line);
            snprintf(line + len, sizeof(line) - (size_t)len, "%s", ghost);
            len = (int)strlen(line);
        } else if (key == KEY_UP || key == KEY_DOWN) {
            if (key == KEY_UP && history_pos > 0) history_pos--;
            if (key == KEY_DOWN && history_pos < history_count) history_pos++;
            if (history_pos < history_count) {
                snprintf(line, sizeof(line), "%s", history[history_pos]);
            } else {
                line[0] = '\0';
            }
            len = (int)strlen(line);
        } else if (key == 127 || key == 8) {
            if (len > 0) line[--len] = '\0';
        } else if (key == 21) { // Ctrl+U - стереть строку
            len = 0;
            line[0] = '\0';
        } else if (key >= 32 && key < 127 && len < size - 1) {
            line[len++] = (char)key;
            line[len] = '\0';
        }
    }

    // Вернуться в текст игры и повторить там введённое
    if (resized) {
        printf("\x1b[%d;1H", screen_rows - 3);
    } else {
        printf("\x1b" "8");
    }
    printf("%s\n", line);
    fflush(stdout);

    if (len > 0 && (history_count == 0 || strcmp(history[history_count - 1], line) != 0)) {
        if (history_count == HISTORY_SIZE) {
            memmove(history[0], history[1], sizeof(history[0]) * (HISTORY_SIZE - 1));
            history_count--;
        }
        snprintf(history[history_count++], LINE_LEN, "%s", line);
    }

    ui_set_hints(NULL, 0, 0); // Подсказки действуют на одно чтение
    snprintf(buf, (size_t)size, "%s", line);
    return true;
}
