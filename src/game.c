#include "game.h"
#include "dialogues.h"
#include <stdarg.h>

// Forward declarations (needed because display_location calls process_command, and vice-versa)
void display_location(const char *location, Inventory *inventory, bool sweet_mode);

// Helper function to format strings (like Python's f-strings)
char *format_string(const char *format, ...) {
    va_list args;
    va_start(args, format);

    // Determine the size needed for the formatted string
    int size = vsnprintf(NULL, 0, format, args);
    if (size < 0) {
        va_end(args);
        return NULL; // Error
    }

    char *str = malloc(size + 1); // Allocate memory (+1 for null terminator)
    if (str == NULL) {
        va_end(args);
        return NULL; // Memory allocation failed
    }

    vsnprintf(str, size + 1, format, args); // Actually format the string
    va_end(args);

    return str;
}
const char* process_command(char *command, const char *current_location, Inventory *inventory, bool sweet_mode) {
    char *new_location = NULL; // Initialize to NULL

    if (strcmp(current_location, "base") == 0) {
        // --- BASE Location Commands ---
        if (strcmp(command, "go north") == 0) {
            new_location = "entrance";
        } else if (strcmp(command, "go east") == 0) {
            new_location = "gift_shop";
        } else if (strcmp(command, "go west") == 0) {
            if (!inventory->met_alex) {
                new_location = "alex_rivers";
            } else {
                new_location = "Patrick";
            }
        } else if (strcmp(command, "go south") == 0 && !inventory->worker_task && !inventory->met_patrick) {
            new_location = "worker";
        } else if (strcmp(command, "look around") == 0) {
            printf("%s\n", get_dialogue(current_location, "look_around_base", sweet_mode)); // Assuming a dialogue key
        } else if (strcmp(command, "help") == 0) {
			printf("%s\n", get_dialogue(current_location, "base_help1", sweet_mode)); // Assuming dialogue keys
            printf("%s\n", get_dialogue(current_location, "base_help2", sweet_mode));
            printf("%s\n", get_dialogue(current_location, "base_help3", sweet_mode));
            printf("%s\n", get_dialogue(current_location, "base_help4", sweet_mode));
            printf("%s\n", get_dialogue(current_location, "base_help5", sweet_mode));
        } else if (strcmp(command, "inventory") == 0) {
            display_inventory(inventory);
        } else if (strcmp(command, "save") == 0) {
            save_game(current_location, inventory, "../data/savegame.dat");
        } else if (strcmp(command, "load") == 0) {
            char *temp_location = malloc(50);
            if (!temp_location) {
                perror("Failed to allocate memory for location!");
                exit(EXIT_FAILURE);
            }
            if (load_game(temp_location, inventory, "../data/savegame.dat")) {
                new_location = temp_location; // Use the loaded location
            } else {
                free(temp_location);  //Free in error case.
            }
        }
         else {
            printf("%s\n", get_dialogue(current_location, "invalid_command", sweet_mode));
        }
    }
	 // --- ALEX_RIVERS Location Commands ---
	else if (strcmp(current_location, "alex_rivers") == 0)
	{
		if (!inventory->met_alex) {
                printf("%s\n", get_dialogue(current_location, "alex_intro", sweet_mode));
                printf("%s\n", get_dialogue(current_location, "alex_greeting", sweet_mode));
                printf("%s\n", get_dialogue(current_location, "alex_watch1", sweet_mode));
                printf("%s\n", get_dialogue(current_location, "alex_talking", sweet_mode));
                for (int i = 0; i < 5; i++) {
                   char dialogue_key[30];
                    snprintf(dialogue_key, sizeof(dialogue_key), "alex_blah", i);
                    char formatted_dialogue[200]; // Adjust size as needed

                    if (sweet_mode) {
                       snprintf(formatted_dialogue, sizeof(formatted_dialogue),  "...blah, blah, blah! (%d minutes)... they seem so passionate!", 5 - i);
                    }
                    else {
                       snprintf(formatted_dialogue, sizeof(formatted_dialogue), "...blah, blah, blah! (%d minutes)", 5 - i);
                    }

                   printf("%s\n", formatted_dialogue);

                }
                printf("%s\n", get_dialogue(current_location, "alex_watch2", sweet_mode));
                printf("%s\n", get_dialogue(current_location, "alex_run", sweet_mode));
                printf("%s\n", get_dialogue(current_location, "alex_time_wasted", sweet_mode));
                printf("%s\n", get_dialogue(current_location, "alex_continue", sweet_mode));
                inventory->met_alex = true;
                if (!has_item(inventory, "ticket")) {
                    printf("%s\n", get_dialogue(current_location, "alex_no_ticket", sweet_mode));
                }
                new_location = "base";
            }
			else{
                printf("%s\n", get_dialogue(current_location, "alex_again",sweet_mode));
                printf("%s\n", get_dialogue(current_location, "alex_you_again", sweet_mode));
                printf("%s\n", get_dialogue(current_location, "alex_what", sweet_mode));

                char *choices[2];
                char *support_options[] = {
                    get_dialogue(current_location, "alex_support1", sweet_mode),
                    get_dialogue(current_location, "alex_support2", sweet_mode),
                    get_dialogue(current_location, "alex_support3", sweet_mode),
                    get_dialogue(current_location, "alex_support4", sweet_mode),
                    get_dialogue(current_location, "alex_support5", sweet_mode),
                };

                char *best_support = get_dialogue(current_location, "alex_support3", sweet_mode);
                bool best_chosen = false;
                int num_options = sizeof(support_options) / sizeof(support_options[0]);
                for (int i = 0; i < num_options; i++) {
                    printf("%d. %s\n", i+1, support_options[i]);
                }

                for (int i = 0; i < 2; i++) {
                    int choice = 0;

                    while(1){
                        printf("Enter choice %d: ", i + 1);
                        char* input = get_player_input();

                        if(input == NULL){
                            printf("Invalid Input");
                            continue;
                        }

                        if (sscanf(input, "%d", &choice) == 1 && choice >= 1 && choice <= num_options){
                            choices[i] = support_options[choice-1];

                            if (strcmp(choices[i], best_support) == 0)
                                best_chosen = true;
                            free(input);
                            break;
                        }
                        else
                            printf("Invalid input. Pick a number from the list.\n");
                            free(input);
                    }
                }

                printf("You say:\n");
                for(int i=0; i<2; ++i) {
                    printf("- %s\n", choices[i]);
                }

                if (best_chosen) {
                    printf("%s\n", get_dialogue(current_location, "alex_thanks", sweet_mode));
                    inventory->money += 40;
                    add_item(inventory, "mask");
                    add_item(inventory, "ticket");
                } else {
                    printf("%s\n", get_dialogue(current_location, "alex_thanks2", sweet_mode));
                }
                printf("%s\n", get_dialogue(current_location, "alex_hints", sweet_mode));
                new_location = "base";

            }
	}
     // --- PATRICK Location Commands ---
    else if (strcmp(current_location, "Patrick") == 0) {
        printf("%s\n", get_dialogue(current_location, "patrick_intro", sweet_mode));
        printf("%s\n", get_dialogue(current_location, "patrick_greeting", sweet_mode));
        printf("%s\n", get_dialogue(current_location, "patrick_moves", sweet_mode));
        printf("%s\n", get_dialogue(current_location, "patrick_what", sweet_mode));
        printf("%s\n", get_dialogue(current_location, "patrick_hints", sweet_mode));

        if (strcmp(command, "join") == 0) {
            // ... (Join quadrobics club - bad ending)
             printf("You join Patrick's quadrobics club.\n");
             printf("You spend a year practicing, forgetting about the CN Tower.\n");
             printf("One day, you're mistaken for a stray cat and taken to a shelter.\n");
             printf("You're adopted and live a comfy but meaningless life.\n");
             printf("You become useless. (Bad Ending)\n");
             new_location = "exit";
        } else if (strcmp(command, "decline") == 0) {
            printf("%s\n", get_dialogue(current_location, "patrick_decline", sweet_mode));
            new_location = "base";
        } else if (strcmp(command, "back") == 0) {
            new_location = "base";
        } else if (strcmp(command, "exit") == 0) {
            return "exit";
        } else if(strcmp(command, "restart") == 0){
            return "restart";
        }
        else {
             printf("%s\n", get_dialogue(current_location, "invalid_command", sweet_mode));
        }
    }
    // --- ENTRANCE Location Commands ---
    else if (strcmp(current_location, "entrance") == 0)
    {
        printf("%s\n", get_dialogue(current_location, "entrance_line", sweet_mode));
        if (has_item(inventory, "ticket")) {
            printf("%s\n", get_dialogue(current_location, "entrance_ticket", sweet_mode));
        } else {
            printf("%s\n", get_dialogue(current_location, "entrance_no_ticket", sweet_mode));
        }
        printf("%s\n", get_dialogue(current_location, "entrance_what", sweet_mode));
        printf("%s\n", get_dialogue(current_location, "entrance_hints", sweet_mode));


        if (strcmp(command, "go north") == 0 && has_item(inventory, "ticket")) {
            new_location = "security";
        } else if (strcmp(command, "go west") == 0) {
            new_location = "ticket_booth";
        } else if (strcmp(command, "back") == 0) {
            new_location = "base";
        } else if (strcmp(command, "inventory") == 0) {
            display_inventory(inventory);
        }
        else if (strcmp(command, "exit") == 0) {
            return "exit"; // Let main handle exiting
        }
        else if (strcmp(command, "restart") == 0) {
            return "restart"; // Let main handle restarting.
        }

        else {
            printf("Invalid command or action not allowed. Try 'Help'.\n");
        }
    }
	// --- TICKET_BOOTH Location Commands ---
    else if (strcmp(current_location, "ticket_booth") == 0) {
        printf("%s\n", get_dialogue(current_location, "ticket_price", sweet_mode));
        printf("%s\n", get_dialogue(current_location, "ticket_what", sweet_mode));
        printf("%s\n", get_dialogue(current_location, "ticket_hints", sweet_mode));

        if (strcmp(command, "buy ticket") == 0) {
            if (inventory->money >= 40) {
                inventory->money -= 40;
                add_item(inventory, "ticket");
                printf("You bought a ticket for $40.\n");
            } else {
                printf("Not enough money to buy a ticket.\n");
            }
        } else if (strcmp(command, "back") == 0) {
            new_location = "entrance";
        }
         else if (strcmp(command, "inventory") == 0) {
            display_inventory(inventory);
        }
        else if (strcmp(command, "exit") == 0)
        {
            return "exit";
        }
        else if (strcmp(command, "restart") == 0)
        {
            return "restart";
        }
        else {
            printf("Invalid command. Try 'Help'.\n");
        }
    }
    // --- SECURITY Location Commands ---
    else if(strcmp(current_location, "security") == 0){
        printf("%s\n", get_dialogue(current_location, "security_check", sweet_mode));
        if(has_item(inventory,"ticket")){
             printf("%s\n", get_dialogue(current_location, "security_pass", sweet_mode));
        }
        else{
             printf("%s\n", get_dialogue(current_location, "security_no_ticket", sweet_mode));
        }
        printf("%s\n", get_dialogue(current_location, "security_what", sweet_mode));

        printf("%s\n", get_dialogue(current_location, "security_hints", sweet_mode));

        if (strcmp(command, "go north") == 0 && has_item(inventory, "ticket"))
        {
            new_location = "elevator";
        }
        else if (strcmp(command,"back") == 0)
        {
            new_location = "entrance";
        }
        else if (strcmp(command, "inventory") == 0)
        {
            display_inventory(inventory);
        }
        else if (strcmp(command, "exit") == 0)
        {
            return "exit";
        }
        else if (strcmp(command, "restart") == 0)
        {
            return "restart";
        }
        else
        {
             printf("Invalid command or action not allowed. Try 'Help'.\n");
        }
    }
    // --- ELEVATOR Location Commands ---
    else if (strcmp(current_location, "elevator") == 0) {
        // Elevator is mostly descriptive.  No commands here, just transitions.
        printf("%s\n", get_dialogue(current_location, "elevator_close", sweet_mode));
        printf("%s\n", get_dialogue(current_location, "elevator_up", sweet_mode));
        printf("%s\n", get_dialogue(current_location, "elevator_pop", sweet_mode));
        printf("%s\n", get_dialogue(current_location, "elevator_ding", sweet_mode));
        new_location = "lookout"; // Automatically go to the LookOut level
    }
    // --- LOOKOUT Location Commands ---
    else if (strcmp(current_location, "lookout") == 0) {
        printf("%s\n", get_dialogue(current_location, "lookout_view", sweet_mode));
        printf("%s\n", get_dialogue(current_location, "lookout_see", sweet_mode));
        printf("%s\n", get_dialogue(current_location, "lookout_directions", sweet_mode));
        printf("%s\n", get_dialogue(current_location, "lookout_what", sweet_mode));
        printf("%s\n", get_dialogue(current_location, "lookout_hints", sweet_mode));


        if (strcmp(command, "go down") == 0) {
            new_location = "glass_floor";
        } else if (strcmp(command, "go east") == 0) {
            new_location = "information_booth";
        } else if (strcmp(command, "look around") == 0) {
            printf("You take in the magnificent view of Toronto and take some pictures.\n");
        } else if (strcmp(command, "inventory") == 0){
            display_inventory(inventory);
        }
         else if (strcmp(command, "exit") == 0)
        {
            return "exit";
        }
        else if (strcmp(command, "restart") == 0)
        {
            return "restart";
        }
        else {
            printf("Invalid command. Try 'Help'.\n");
        }
    }
    // --- GLASS_FLOOR Location Commands ---
    else if (strcmp(current_location, "glass_floor") == 0) {
        printf("%s\n", get_dialogue(current_location, "glass_scary", sweet_mode));
        printf("%s\n", get_dialogue(current_location, "glass_see", sweet_mode));
        printf("%s\n", get_dialogue(current_location, "glass_directions", sweet_mode));
        printf("%s\n", get_dialogue(current_location, "glass_what", sweet_mode));
        printf("%s\n", get_dialogue(current_location, "glass_hints", sweet_mode));


        if (strcmp(command, "go up") == 0) {
            new_location = "lookout";
        } else if (strcmp(command, "go west") == 0) {
            new_location = "edgewalk_registration";
        } else if (strcmp(command, "look down") == 0) {
            printf("It's a long way down!  You feel a bit dizzy.\n");
        }
         else if (strcmp(command, "inventory") == 0){
            display_inventory(inventory);
        }
         else if (strcmp(command, "exit") == 0)
        {
            return "exit";
        }
        else if (strcmp(command, "restart") == 0)
        {
            return "restart";
        }
        else {
            printf("Invalid command. Try 'Help'.\n");
        }
    }
    // --- EDGEWALK_REGISTRATION Location Commands ---
    else if (strcmp(current_location, "edgewalk_registration") == 0) {
        printf("%s\n", get_dialogue(current_location, "edgewalk_desk", sweet_mode));
        if (has_item(inventory, "edgewalk_ticket")) {
            printf("%s\n", get_dialogue(current_location, "edgewalk_ticket", sweet_mode));
        } else {
            printf("%s\n", get_dialogue(current_location, "edgewalk_no_ticket", sweet_mode));
        }

        printf("%s\n", get_dialogue(current_location, "edgewalk_what", sweet_mode));
        printf("%s\n", get_dialogue(current_location, "edgewalk_hints", sweet_mode));

        if (strcmp(command, "buy ticket") == 0) {
            if (inventory->money >= 195) {
                inventory->money -= 195;
                add_item(inventory, "edgewalk_ticket");
                printf("You bought an EdgeWalk ticket for $195.\n");
            } else {
                printf("Not enough money to buy an EdgeWalk ticket.\n");
            }
        } else if (strcmp(command, "go north") == 0 && has_item(inventory, "edgewalk_ticket")) {
            new_location = "edgewalk_preparation";
        } else if (strcmp(command, "back") == 0) {
            new_location = "glass_floor";
        }
        else if (strcmp(command, "inventory") == 0){
            display_inventory(inventory);
        }
         else if (strcmp(command, "exit") == 0)
        {
            return "exit";
        }
        else if (strcmp(command, "restart") == 0)
        {
            return "restart";
        }
        else {
            printf("Invalid command or action not allowed. Try 'Help'.\n");
        }
    }
    // --- EDGEWALK_PREPARATION Location Commands ---
    else if (strcmp(current_location, "edgewalk_preparation") == 0) {
        printf("%s\n", get_dialogue(current_location, "edgewalk_prep", sweet_mode));
        printf("%s\n", get_dialogue(current_location, "edgewalk_nervous", sweet_mode));
        printf("%s\n", get_dialogue(current_location, "edgewalk_check", sweet_mode));

        new_location = "edgewalk"; // Automatically transition to the EdgeWalk
    }
    // --- EDGEWALK Location Commands ---
    else if (strcmp(current_location, "edgewalk") == 0) {
        printf("%s\n", get_dialogue(current_location, "edgewalk_outside", sweet_mode));
        printf("%s\n", get_dialogue(current_location, "edgewalk_exciting", sweet_mode));
        printf("%s\n", get_dialogue(current_location, "edgewalk_win", sweet_mode));
        new_location = "exit"; // Winning condition!
    }
    // --- GIFT_SHOP Location Commands ---
    else if (strcmp(current_location, "gift_shop") == 0) {
        printf("%s\n", get_dialogue(current_location, "gift_souvenirs", sweet_mode));
        if (!has_item(inventory, "mask")) {
            printf("%s\n", get_dialogue(current_location, "gift_mask", sweet_mode));
        }
        printf("%s\n", get_dialogue(current_location, "gift_what", sweet_mode));
        printf("%s\n", get_dialogue(current_location, "gift_hints", sweet_mode));

        if (strcmp(command, "buy postcards") == 0) {
            if (inventory->money >= 5) {
                inventory->money -= 5;
                add_item(inventory, "postcards");
                printf("You bought postcards for $5.\n");
            } else {
                printf("Not enough money to buy postcards.\n");
            }
        } else if (strcmp(command, "buy souvenir") == 0) {
            if (inventory->money >= 15) {
                inventory->money -= 15;
                add_item(inventory, "souvenir");
                printf("You bought a CN Tower souvenir for $15.\n");
            } else {
                printf("Not enough money to buy a souvenir.\n");
            }
        } else if (strcmp(command, "buy mask") == 0) {
            if (inventory->money >= 20) {
                inventory->money -= 20;
                add_item(inventory, "mask");
                printf("You bought a mask for $20.\n");
            } else {
                printf("Not enough money to buy a mask.\n");
            }
        } else if (strcmp(command, "back") == 0) {
            new_location = "base";
        }
        else if (strcmp(command, "inventory") == 0){
            display_inventory(inventory);
        }
         else if (strcmp(command, "exit") == 0)
        {
            return "exit";
        }
        else if (strcmp(command, "restart") == 0)
        {
            return "restart";
        }
        else {
            printf("Invalid command. Try 'Help'.\n");
        }
    }
	 // --- INFORMATION_BOOTH Location Commands ---
    else if (strcmp(current_location, "information_booth") == 0) {
        printf("%s\n", get_dialogue(current_location, "info_brochures", sweet_mode));
        printf("%s\n", get_dialogue(current_location, "info_staff", sweet_mode));
        printf("%s\n", get_dialogue(current_location, "info_what", sweet_mode));
		printf("%s\n", get_dialogue(current_location, "info_hints", sweet_mode));

        if (strcmp(command, "ask about history") == 0) {
             printf("%s\n", get_dialogue(current_location, "history1", sweet_mode));
             printf("%s\n", get_dialogue(current_location, "history2", sweet_mode));
             printf("%s\n", get_dialogue(current_location, "history3", sweet_mode));
        } else if (strcmp(command, "ask about building") == 0) {
            printf("%s\n", get_dialogue(current_location, "building1", sweet_mode));
            printf("%s\n", get_dialogue(current_location, "building2", sweet_mode));
            printf("%s\n", get_dialogue(current_location, "building3", sweet_mode));
        } else if (strcmp(command, "back") == 0) {
            new_location = "lookout";
        }
        else if (strcmp(command, "inventory") == 0){
            display_inventory(inventory);
        }
         else if (strcmp(command, "exit") == 0)
        {
            return "exit";
        }
        else if (strcmp(command, "restart") == 0)
        {
            return "restart";
        }
        else {
            printf("Invalid command. Try 'Help'.\n");
        }
    }
	// --- WORKER Location Commands ---
    else if (strcmp(current_location, "worker") == 0) {
        printf("%s\n", get_dialogue(current_location, "worker_tired", sweet_mode));
        printf("%s\n", get_dialogue(current_location, "worker_help", sweet_mode));
        printf("%s\n", get_dialogue(current_location, "worker_start", sweet_mode));

        for (int i = 1; i <= 4; i++) {
            char dialogue_key[30];
            snprintf(dialogue_key, sizeof(dialogue_key), "worker_carry", i);
            char formatted_dialogue[200];

            if(sweet_mode){
                snprintf(formatted_dialogue, sizeof(formatted_dialogue), "You carry box %d to the storage room...", i);
            }
            else{
                snprintf(formatted_dialogue, sizeof(formatted_dialogue), "You carry box %d to the storage room...", i);
            }

            printf("%s\n", formatted_dialogue);

        }
        printf("%s\n", get_dialogue(current_location, "worker_fifth", sweet_mode));
        printf("%s\n", get_dialogue(current_location, "worker_what", sweet_mode));
        printf("%s\n", get_dialogue(current_location, "worker_hints", sweet_mode));


        if (strcmp(command, "look inside") == 0) {
            new_location = "open_box";
        } else if (strcmp(command, "continue") == 0) {
            new_location = "storage_room";
        }
        else if (strcmp(command, "inventory") == 0){
            display_inventory(inventory);
        }
         else if (strcmp(command, "exit") == 0)
        {
            return "exit";
        }
        else if (strcmp(command, "restart") == 0)
        {
            return "restart";
        }
        else {
            printf("Invalid command. Try 'Help'.\n");
        }
    }
	 // --- OPEN_BOX Location Commands ---
    else if (strcmp(current_location, "open_box") == 0) {
        printf("%s\n", get_dialogue(current_location, "box_contents", sweet_mode));
        printf("%s\n", get_dialogue(current_location, "box_what", sweet_mode));
        printf("%s\n", get_dialogue(current_location, "box_hints", sweet_mode));


        if (strcmp(command, "take nothing") == 0) {
            printf("You decide to leave the box alone and continue helping the worker.\n");
            new_location = "storage_room";
        } else if (strcmp(command, "take money") == 0) {
            inventory->money += 40;
            printf("You discreetly take the money from the box.\n");
            new_location = "caught_stealing";
        } else if (strcmp(command, "take book") == 0) {
            add_item(inventory, "bible");
            printf("You take the book from the box. It's a Bible.\n");
            new_location = "storage_room";
        } else if (strcmp(command, "take mask") == 0) {
            add_item(inventory, "mask");
            printf("You take the mask from the box.\n");
            new_location = "storage_room";
        }
        else if (strcmp(command, "inventory") == 0){
            display_inventory(inventory);
        }
         else if (strcmp(command, "exit") == 0)
        {
            return "exit";
        }
        else if (strcmp(command, "restart") == 0)
        {
            return "restart";
        }
        else {
            printf("Invalid command. Try 'Help'.\n");
        }
    }
	// --- CAUGHT_STEALING Location Commands ---
    else if (strcmp(current_location, "caught_stealing") == 0) {
        printf("%s\n", get_dialogue(current_location, "caught_seen", sweet_mode));
        printf("%s\n", get_dialogue(current_location, "caught_worker", sweet_mode));
        printf("%s\n", get_dialogue(current_location, "caught_security", sweet_mode));
        printf("%s\n", get_dialogue(current_location, "caught_what", sweet_mode));
        printf("%s\n", get_dialogue(current_location, "caught_hints", sweet_mode));

        if (strcmp(command, "tell truth") == 0) {
            printf("You confess to taking the money. The police are surprisingly understanding.\n");
            printf("They let you go with a warning, but you feel a bit guilty.\n");
             inventory->met_alex = true;
             inventory->met_patrick = true;
            printf("Next day, you go to the CN Tower again, but missed Alex Rivers and a chance for a free ticket.\n");
            printf("You see a strange guy near the entrance.\n");
            new_location = "base";
        } else if (strcmp(command, "bribe") == 0) {
            if (inventory->money >= 50) {
                inventory->money -= 50;
                printf("You offer the officer a bribe.  They reluctantly accept.\n");
                printf("Officer: \"Alright, get back to the CN Tower.  And don't let me catch you again.\"\n");
                new_location = "base";
            } else {
                printf("You don't have enough money to bribe the officer.\n");
            }
        } else if (strcmp(command, "lie") == 0) {
            printf("You try to lie your way out of it, but the police don't believe you.\n");
            printf("You're deported.  No more CN Tower for you. (Bad Ending)\n");
            new_location = "exit";  // Game over
        }
        else if (strcmp(command, "inventory") == 0){
            display_inventory(inventory);
        }
         else if (strcmp(command, "exit") == 0)
        {
            return "exit";
        }
        else if (strcmp(command, "restart") == 0)
        {
            return "restart";
        }
        else {
            printf("Invalid command. Try 'Help'.\n");
        }
    }
    // --- STORAGE_ROOM Location Commands ---
    else if (strcmp(current_location, "storage_room") == 0) {
         printf("%s\n", get_dialogue(current_location, "storage_thanks", sweet_mode));
         printf("%s\n", get_dialogue(current_location, "storage_reward", sweet_mode));
         printf("%s\n", get_dialogue(current_location, "storage_money", sweet_mode));
        inventory->money += 20;
        inventory->worker_task = true;
        printf("%s\n", get_dialogue(current_location, "storage_what", sweet_mode));
        printf("%s\n", get_dialogue(current_location, "storage_hints", sweet_mode));

        if (strcmp(command, "back") == 0) {
            new_location = "base";
        }
        else if (strcmp(command, "inventory") == 0){
            display_inventory(inventory);
        }
        else if (strcmp(command, "exit") == 0)
        {
            return "exit";
        }
        else if (strcmp(command, "restart") == 0)
        {
            return "restart";
        }
        else {
            printf("Invalid command. Try 'Help'.\n");
        }
    }
    // --- JUST_A_CHILL_GUY Location Commands ---
    else if (strcmp(current_location, "just_a_chill_guy") == 0) {
        printf("%s\n", get_dialogue(current_location, "chill_laughing", sweet_mode));
        if (has_item(inventory, "mask") && has_item(inventory, "bible")) {
            printf("%s\n", get_dialogue(current_location, "chill_ready", sweet_mode));
            printf("Hints: 'Go West', 'Back', 'Exit', 'Restart'.\n");
        } else if (has_item(inventory, "bible") && !has_item(inventory, "mask")) {
            printf("%s\n", get_dialogue(current_location, "chill_quadrobists_gone", sweet_mode));
            printf("%s\n", get_dialogue(current_location, "chill_what", sweet_mode));
            printf("%s\n", get_dialogue(current_location, "chill_hints1", sweet_mode));
        } else if (inventory->used_bible) {
            printf("%s\n", get_dialogue(current_location, "chill_scared_quadrobists", sweet_mode));
            printf("%s\n", get_dialogue(current_location, "chill_what", sweet_mode));
            printf("%s\n", get_dialogue(current_location, "chill_hints2", sweet_mode));
        }
        else {
             printf("%s\n", get_dialogue(current_location, "chill_what", sweet_mode));
             printf("%s\n", get_dialogue(current_location, "chill_hints3", sweet_mode));
        }


        if (strcmp(command, "use mask") == 0) {
            if (has_item(inventory, "mask")) {
                inventory->used_mask = true;
                new_location = "corner";
            } else {
                printf("You don't have a mask.\n");
            }
        } else if (strcmp(command, "use bible") == 0) {
            if (has_item(inventory, "bible")) {
                printf("You wave the Bible around. The quadrobists scatter in fear!\n");
                remove_item(inventory, "bible"); // Remove the Bible after use
                inventory->used_bible = true; // Mark as used
            } else {
                printf("You don't have a Bible.\n");
            }
        } else if (strcmp(command, "go forward") == 0) {
            new_location = "corner";
        } else if (strcmp(command, "ask about corner") == 0) {
            printf("Just a Chill Guy: \"Just some quadrobists practicing.  Nothing to worry about... unless you're scared.\"\n");
        } else if (strcmp(command, "go west") == 0 && has_item(inventory, "mask") && has_item(inventory, "bible")) {
            new_location = "scare_alex";  // Go to scare Alex
        }
        else if (strcmp(command, "back") == 0) {
            new_location = "glass_floor";
        }
         else if (strcmp(command, "inventory") == 0){
            display_inventory(inventory);
        }
         else if (strcmp(command, "exit") == 0)
        {
            return "exit";
        }
        else if (strcmp(command, "restart") == 0)
        {
            return "restart";
        }
        else {
            printf("Invalid command. Try 'Help'.\n");
        }
    }
     // --- CORNER Location Commands ---
    else if (strcmp(current_location, "corner") == 0) {
        if (has_item(inventory, "mask") && inventory->used_mask) {
            printf("%s\n", get_dialogue(current_location, "corner_peek", sweet_mode));
            printf("%s\n", get_dialogue(current_location, "corner_back", sweet_mode));
            new_location = "just_a_chill_guy";
            inventory->used_mask = false;
        } else {
            printf("%s\n", get_dialogue(current_location, "corner_seen", sweet_mode));
            printf("%s\n", get_dialogue(current_location, "corner_join", sweet_mode));
            printf("%s\n", get_dialogue(current_location, "corner_quadrobist", sweet_mode));

            new_location = "quadrobics_base";
        }
    }
    // --- QUADROBICS_BASE Location Commands ---
    else if (strcmp(current_location, "quadrobics_base") == 0) {
         printf("%s\n", get_dialogue(current_location, "quadrobics_move", sweet_mode));
         printf("%s\n", get_dialogue(current_location, "quadrobics_hints", sweet_mode));
        if(strcmp(command, "go west") == 0){
            new_location = "alex_rivers_quadrobics";
        }
        else if (strcmp(command, "inventory") == 0){
            display_inventory(inventory);
        }
        else if (strcmp(command, "exit") == 0)
        {
            return "exit";
        }
        else if (strcmp(command, "restart") == 0)
        {
            return "restart";
        }
        else {
             printf("Invalid command. Try 'Help'.\n");
        }
    }
    // --- ALEX_RIVERS_QUADROBICS Location Commands ---
    else if (strcmp(current_location, "alex_rivers_quadrobics") == 0) {
        printf("You approach Alex Rivers, moving like a quadrobist.\n");
        printf("Alex is startled, drops their phone, and their recording is ruined.\n");
        printf("You've ruined their day. (Bad Ending)\n");
        new_location = "exit"; // Bad ending
    }
	// --- SCARE_ALEX Location Commands ---
    else if (strcmp(current_location, "scare_alex") == 0) {
        printf("%s\n", get_dialogue(current_location, "scare_success", sweet_mode));
        printf("%s\n", get_dialogue(current_location, "scare_drop", sweet_mode));
        printf("%s\n", get_dialogue(current_location, "scare_hints", sweet_mode));
        if (strcmp(command, "take phone") == 0) {
            printf("You grab Alex's phone. It's yours now!\n");
            add_item(inventory, "alex_phone");
            new_location = "phone_found";
        } else if (strcmp(command, "leave phone") == 0) {
            printf("You decide to leave the phone.  What were you thinking? (Bad Ending)\n");
            new_location = "exit";  // Bad ending
        }
        else if (strcmp(command, "inventory") == 0){
            display_inventory(inventory);
        }
        else if (strcmp(command, "exit") == 0)
        {
            return "exit";
        }
        else if (strcmp(command, "restart") == 0)
        {
            return "restart";
        }
        else {
             printf("Invalid command. Try 'Help'.\n");
        }
    }
    // --- PHONE_FOUND Location Commands ---
    else if (strcmp(current_location, "phone_found") == 0) {
        printf("%s\n", get_dialogue(current_location, "phone_run", sweet_mode));
        printf("%s\n", get_dialogue(current_location, "phone_what", sweet_mode));
        printf("%s\n", get_dialogue(current_location, "phone_hints", sweet_mode));

        if (strcmp(command, "jump") == 0) {
            printf("%s\n", get_dialogue(current_location, "roof_jump", sweet_mode));
            new_location = "roof";
        } else if (strcmp(command, "go back") == 0) {
            new_location = "glass_floor";
        }
        else if (strcmp(command, "inventory") == 0){
            display_inventory(inventory);
        }
        else if (strcmp(command, "exit") == 0)
        {
            return "exit";
        }
        else if (strcmp(command, "restart") == 0)
        {
            return "restart";
        }
        else {
             printf("Invalid command. Try 'Help'.\n");
        }
    }
	 // --- ROOF Location Commands ---
    else if (strcmp(current_location, "roof") == 0) {
        // This is a final state.  No commands are possible.
        return "exit";
    }

    if (new_location != NULL) {
        free(command); // Free 'command' here, *after* all string comparisons
    }
    return new_location;
}


// Display the current location and available actions
void display_location(const char *location, Inventory *inventory, bool sweet_mode) {
    printf("\n---\n");

    if (strcmp(location, "base") == 0) {
        printf("%s\n", get_dialogue(location, "base_intro", sweet_mode));
        printf("%s\n", get_dialogue(location, "base_directions", sweet_mode));
        if (!inventory->met_alex) {
            printf("%s\n", get_dialogue(location, "base_alex", sweet_mode));
        }
        if (!inventory->worker_task && !inventory->met_patrick) {
           printf("%s\n", get_dialogue(location, "base_worker", sweet_mode));
        }
        if(inventory->met_patrick){
             printf("%s\n", get_dialogue(location, "base_patrick", sweet_mode));
        }
        printf("%s\n", get_dialogue(location,"base_what", sweet_mode));
        printf("%s\n", get_dialogue(location, "base_hints", sweet_mode));

    } else if (strcmp(location, "alex_rivers") == 0) {
        if (!inventory->met_alex) { // First time meeting Alex
           //This is handled in process_command
        }
        else {
           //This is handled in process_command.
        }
    }
    else if (strcmp(location, "Patrick") == 0) {
       //This is handled in process_command
    } else if (strcmp(location, "entrance") == 0) {
       //This is handled in process_command
    }
    else if (strcmp(location, "ticket_booth") == 0) {
        //This is handled in process_command
    }
    else if(strcmp(location, "security") == 0){
        //This is handled in process_command
    }
    else if (strcmp(location, "elevator") == 0) {
       //This is handled in process_command

    } else if (strcmp(location, "lookout") == 0) {
        //This is handled in process_command
    }
    else if(strcmp(location, "glass_floor") == 0){
       //This is handled in process_command
    }
    else if(strcmp(location, "edgewalk_registration") == 0){
        //This is handled in process_command
    }
    else if(strcmp(location, "edgewalk_preparation") == 0){
       //This is handled in process_command
    }
    else if(strcmp(location, "edgewalk") == 0){
        //This is handled in process_command
    }
    else if(strcmp(location, "gift_shop") == 0){
       //This is handled in process_command
    }
    else if(strcmp(location, "information_booth") == 0){
        //This is handled in process_command
    }
     else if (strcmp(location, "worker") == 0) {
       //This is handled in process_command
    }
    else if (strcmp(location, "open_box") == 0) {
       //This is handled in process_command
    }
	else if (strcmp(location, "caught_stealing") == 0) {
        //This is handled in process_command
    }
    else if (strcmp(location, "storage_room") == 0) {
       //This is handled in process_command
    }
    else if (strcmp(location, "just_a_chill_guy") == 0) {
        //This is handled in process_command
    }
    else if (strcmp(location, "corner") == 0) {
       //This is handled in process_command
    }
    else if (strcmp(location, "quadrobics_base") == 0) {
        //This is handled in process_command
    }
	else if (strcmp(location, "alex_rivers_quadrobics") == 0) {
        //This is handled in process_command
    }
    else if (strcmp(location, "scare_alex") == 0) {
        //This is handled in process_command
    }
	else if (strcmp(location, "phone_found") == 0) {
        //This is handled in process_command
    }
    else if (strcmp(location, "roof") == 0) {
       //This is handled in process_command
    }
     else {
        printf("Invalid location: %s\n", location); // Debugging output
    }

    printf("---\n");
}

// Inventory management functions (implementation)
bool has_item(Inventory *inventory, const char *item) {
    if (strcmp(item, "ticket") == 0) return inventory->ticket;
    if (strcmp(item, "mask") == 0) return inventory->mask;
    if (strcmp(item, "edgewalk_ticket") == 0) return inventory->edgewalk_ticket;
    if (strcmp(item, "postcards") == 0) return inventory->postcards;
    if (strcmp(item, "souvenir") == 0) return inventory->souvenir;
    if (strcmp(item, "bible") == 0) return inventory->bible;
    if (strcmp(item, "alex_phone") == 0) return inventory->alex_phone;
    return false; // Item not found
}

void add_item(Inventory *inventory, const char *item) {
    if (strcmp(item, "ticket") == 0) inventory->ticket = true;
    else if (strcmp(item, "mask") == 0) inventory->mask = true;
    else if (strcmp(item, "edgewalk_ticket") == 0) inventory->edgewalk_ticket = true;
    else if (strcmp(item, "postcards") == 0) inventory->postcards = true;
    else if (strcmp(item, "souvenir") == 0) inventory->souvenir = true;
    else if (strcmp(item, "bible") == 0) inventory->bible = true;
    else if (strcmp(item, "alex_phone") == 0) inventory->alex_phone = true;
    // No else needed, just ignore invalid items
}

void remove_item(Inventory *inventory, const char *item) {
    if (strcmp(item, "ticket") == 0) inventory->ticket = false;
    else if (strcmp(item, "mask") == 0) inventory->mask = false;
    else if (strcmp(item, "edgewalk_ticket") == 0) inventory->edgewalk_ticket = false;
    else if (strcmp(item, "postcards") == 0) inventory->postcards = false;
    else if (strcmp(item, "souvenir") == 0) inventory->souvenir = false;
    else if (strcmp(item, "bible") == 0) inventory->bible = false;
    else if (strcmp(item, "alex_phone") == 0) inventory->alex_phone = false;
}
void display_inventory(Inventory *inventory) {
	printf("Inventory: Money=$%d, ticket=%s, mask=%s, edgewalk_ticket=%s, postcards=%s, souvenir=%s, bible=%s, alex_phone=%s\n",
		   inventory->money,
		   inventory->ticket ? "true" : "false",
		   inventory->mask ? "true" : "false",
		   inventory->edgewalk_ticket ? "true" : "false",
		   inventory->postcards ? "true" : "false",
		   inventory->souvenir ? "true" : "false",
		   inventory->bible ? "true" : "false",
		   inventory->alex_phone ? "true" : "false");
}