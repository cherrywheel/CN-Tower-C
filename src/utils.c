#define _WIN32
#include "game.h"

// Cross-platform clear screen
void clear_console() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

// Simplified save/load (no JSON, just a binary file)
void save_game(const char *location, Inventory *inventory, const char *filename) {
    FILE *file = fopen(filename, "wb");
    if (file == NULL) {
        perror("Error saving game");
        return;
    }

    // Save location (fixed-size buffer for simplicity)
    char loc_buffer[50];
    strncpy(loc_buffer, location, sizeof(loc_buffer) - 1);
    loc_buffer[sizeof(loc_buffer) - 1] = '\0'; // Ensure null termination
    fwrite(loc_buffer, sizeof(loc_buffer), 1, file);

    // Save inventory
    fwrite(inventory, sizeof(Inventory), 1, file);

    fclose(file);
    printf("Game saved.\n");
}

// Load game (and handle potential errors)
bool load_game(char *location, Inventory *inventory, const char *filename) {
    FILE *file = fopen(filename, "rb");
    if (file == NULL) {
        printf("No saved game found. Starting new game.\n");
        return false; // Indicate load failure
    }

    // Load location
    char loc_buffer[50];
    if (fread(loc_buffer, sizeof(loc_buffer), 1, file) != 1) {
        perror("Error loading location");
        fclose(file);
        return false;
    }
    strcpy(location, loc_buffer); // Copy back to location

    // Load inventory
    if (fread(inventory, sizeof(Inventory), 1, file) != 1) {
        perror("Error loading inventory");
        fclose(file);
        return false;
    }

    fclose(file);
    printf("Game loaded.\n");
    return true; // Indicate load success
}

// Helper function to trim leading/trailing whitespace from a string
char *trim_whitespace(char *str) {
    char *end;

    // Trim leading space
    while(isspace((unsigned char)*str)) str++;

    if(*str == 0)  // All spaces?
        return str;

    // Trim trailing space
    end = str + strlen(str) - 1;
    while(end > str && isspace((unsigned char)*end)) end--;

    // Write new null terminator character
    end[1] = '\0';

    return str;
}

// Get player input safely using fgets
char *get_player_input() {
    char *input = malloc(100); // Allocate memory for input
    if (input == NULL) {
        perror("Memory allocation failed");
        exit(EXIT_FAILURE); // Exit on allocation failure
    }

    if (fgets(input, 100, stdin) != NULL) {
        return trim_whitespace(input);
    } else {
        free(input); // Free allocated memory if fgets fails
        return NULL;  // Or handle the error as appropriate
    }
}