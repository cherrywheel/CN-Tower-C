#include "game.h"

int main() {
    char location[50] = "base"; // Start at the base.  Use a fixed-size buffer.
    Inventory inventory = {STARTING_MONEY, false, false, false, false, false, false, false, false, false, false, false, false};
    bool sweet_mode = false;
    char *command;

    // Try to load a saved game; if it fails, start a new game.
    if (!load_game(location, &inventory, "../data/savegame.dat")) {
        // Initialize if the game starts (no save file or load failed).
        inventory.money = STARTING_MONEY;
    }

    printf("Welcome to the CN Tower Experience Simulator!\n");
    printf("Type 'Help' for commands.\n");

    while (true) { // Main game loop
        clear_console();
        display_location(location, &inventory, sweet_mode);
        command = get_player_input();

        if (command == NULL) {
            printf("Exiting.\n");
            break;  // Exit if input fails (e.g., EOF)
        }

        const char *new_location = process_command(command, location, &inventory, sweet_mode);

        if (new_location == NULL) {
            // Command was handled, but location didn't change.
            free(command);
            continue; // Go to the next loop iteration
        }
        else if (strcmp(new_location, "exit") == 0) {
            printf("Thanks for playing!\n");
            free(command);
            break; // Exit the game
        } else if (strcmp(new_location, "restart") == 0) {
            printf("Restarting the game...\n");
            free(command);
            strcpy(location, "base");  // Reset to the starting location
            inventory = (Inventory){STARTING_MONEY, false, false, false, false, false, false, false, false, false, false, false, false}; // Reset inventory
            continue; // Skip to the next loop iteration
        } else {
            // Location changed.

            // Check if the new_location was dynamically allocated (by load_game)
            if (strcmp(location, new_location) != 0) { // Only need to check if different
                //This will only be true if it was loaded.
                if(new_location != location){
                    strcpy(location, new_location); // Update location
                    free(new_location); // Free the allocated memory
                }
				else{
                    strcpy(location, new_location); // Update location
                }
            }

        }
         free(command);
    }
    return 0;
}