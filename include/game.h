#ifndef GAME_H
#define GAME_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>
#include <ctype.h>

// --- Константы ---
#define STARTING_MONEY 40
#define TICKET_PRICE 40
#define EDGEWALK_PRICE 195
#define PHONE_REWARD 100
#define MIN_AGE 16

#define LOCATION_SIZE 50
#define INPUT_SIZE 100

#define SAVE_FILE data_file("savegame.dat")
#define AGE_FILE data_file("age.dat")

// --- Структуры данных ---

typedef struct {
    int money;
    bool ticket;
    bool mask;
    bool edgewalk_ticket;
    bool postcards;
    bool souvenir;
    bool bible;
    bool alex_phone;
    bool met_alex;
    bool met_patrick;
    bool worker_task;
    bool used_mask;
    bool used_bible;
    bool alex_rewarded;
    bool phone_returned;
} Inventory;

// Что делать главному циклу после команды или смены локации.
typedef enum {
    GAME_CONTINUE, // Остаться на месте и ждать следующую команду
    GAME_ENTER,    // (Пере)войти в текущую локацию: вывести её и запустить события
    GAME_OVER,     // Достигнута концовка
    GAME_RESTART,
    GAME_EXIT
} GameState;

// --- Объявления функций (прототипы) ---

// Из game.c
void new_game(char *location, Inventory *inventory);
GameState enter_location(char *location, Inventory *inventory, bool sweet_mode);
GameState process_command(const char *command, char *location, Inventory *inventory, bool *sweet_mode);
bool is_valid_location(const char *location);
void format_status(char *buf, size_t size, const char *location, Inventory *inventory, bool sweet_mode);
extern const char *global_commands[];
int available_commands(const char *location, Inventory *inventory, const char **out, int max);
bool has_item(Inventory *inventory, const char *item);
void add_item(Inventory *inventory, const char *item);
void remove_item(Inventory *inventory, const char *item);
void display_inventory(Inventory *inventory);

// Из dialogues.c
const char *get_dialogue(const char *location, const char *key, bool sweet_mode);
void print_cn_tower_art(void);

// Из utils.c
const char *data_file(const char *name);
void save_game(const char *location, Inventory *inventory, const char *filename);
bool load_game(char *location, Inventory *inventory, const char *filename);
int load_age(const char *filename);
void save_age(int age, const char *filename);
char *trim_whitespace(char *str);
char *get_player_input(void);

#endif // GAME_H
