#ifndef GAME_H
#define GAME_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>
#include <ctype.h>

// --- Constants ---
#define STARTING_MONEY 40
// Add more constants here as needed (e.g., prices)

// --- Data Structures ---

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
} Inventory;

// --- Function Declarations (Prototypes) ---

// From game.c
void display_location(const char *location, Inventory *inventory, bool sweet_mode);
const char * process_command(char *command, const char *current_location, Inventory *inventory, bool sweet_mode);
bool has_item(Inventory *inventory, const char *item);
void add_item(Inventory *inventory, const char *item);
void remove_item(Inventory *inventory, const char *item);
void display_inventory(Inventory *inventory);
// From dialogues.c
const char *get_dialogue(const char *location, const char *key, bool sweet_mode);

// From utils.c
void clear_console();
void save_game(const char *location, Inventory *inventory, const char *filename);
bool load_game(char *location, Inventory *inventory, const char *filename);
char *trim_whitespace(char *str);
char *get_player_input();

#endif // GAME_H