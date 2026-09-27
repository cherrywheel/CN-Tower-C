#include "game.h"
#include "dialogues.h"
#include "ui.h"

// Все локации игры (для проверки сохранений и меню отладки)
static const char *locations[] = {
    "base", "alex_rivers", "Patrick", "entrance", "ticket_booth", "security",
    "elevator", "lookout", "glass_floor", "edgewalk_registration",
    "edgewalk_preparation", "edgewalk", "gift_shop", "information_booth",
    "worker", "open_box", "caught_stealing", "storage_room",
    "just_a_chill_guy", "corner", "quadrobics_base", "alex_rivers_quadrobics",
    "scare_alex", "phone_found", "roof",
    NULL
};

static const char *valid_items[] = {
    "ticket", "mask", "edgewalk_ticket", "postcards", "souvenir", "bible", "alex_phone",
    NULL
};

// Свой генератор случайных чисел: rand() на разных системах даёт разные
// последовательности, а с этим один и тот же seed везде перемешивает одинаково
static uint32_t random_state = 1;

void game_seed(uint32_t seed) {
    random_state = seed;
}

// Случайное число от 0 до n - 1 (LCG из стандарта C)
static int random_below(int n) {
    random_state = random_state * 1103515245u + 12345u;
    return (int)((random_state >> 16) & 0x7fff) % n;
}

// Вывести одну реплику локации
static void say(const char *location, const char *key, bool sweet_mode) {
    printf("%s\n", get_dialogue(location, key, sweet_mode));
}

// Строка "Hints: ..." нужна только в обычном режиме, в TUI подсказки внизу экрана
static void hint(const char *location, const char *key, bool sweet_mode) {
    if (!ui_active()) say(location, key, sweet_mode);
}

static bool is(const char *a, const char *b) {
    return strcmp(a, b) == 0;
}

static void set_location(char *location, const char *new_location) {
    snprintf(location, LOCATION_SIZE, "%s", new_location);
}

bool is_valid_location(const char *location) {
    for (int i = 0; locations[i] != NULL; i++) {
        if (is(locations[i], location)) return true;
    }
    return false;
}

void new_game(char *location, Inventory *inventory) {
    set_location(location, "base");
    memset(inventory, 0, sizeof(*inventory));
    inventory->money = STARTING_MONEY;
}

// Списать деньги, если их хватает
static bool pay(Inventory *inventory, int price) {
    if (inventory->money < price) {
        printf("Not enough money.\n");
        return false;
    }
    inventory->money -= price;
    return true;
}

// Вторая встреча с Алексом: выбрать два способа поддержать
static void support_alex(Inventory *inventory, bool sweet_mode) {
    const char *loc = "alex_rivers";
    const char *options[] = {
        get_dialogue(loc, "alex_support1", sweet_mode),
        get_dialogue(loc, "alex_support2", sweet_mode),
        get_dialogue(loc, "alex_support3", sweet_mode),
        get_dialogue(loc, "alex_support4", sweet_mode),
        get_dialogue(loc, "alex_support5", sweet_mode),
    };
    const char *best_support = options[2];
    int num_options = (int)(sizeof(options) / sizeof(options[0]));
    const char *choices[2] = {NULL, NULL};
    int num_choices = 0;

    // Перемешать варианты (Фишер-Йетс)
    for (int i = num_options - 1; i > 0; i--) {
        int j = random_below(i + 1);
        const char *tmp = options[i];
        options[i] = options[j];
        options[j] = tmp;
    }

    say(loc, "alex_choose", sweet_mode);
    for (int i = 0; i < num_options; i++) {
        printf("%d. %s\n", i + 1, options[i]);
    }

    while (num_choices < 2) {
        printf("Enter choice %d: ", num_choices + 1);
        char *input = get_player_input();
        if (input == NULL) {
            break; // Ввод закончился
        }
        int choice = 0;
        if (sscanf(input, "%d", &choice) == 1 && choice >= 1 && choice <= num_options) {
            choices[num_choices++] = options[choice - 1];
        } else {
            printf("Invalid choice. Pick a number from the list.\n");
        }
        free(input);
    }

    bool best_chosen = false;
    printf("You say:\n");
    for (int i = 0; i < num_choices; i++) {
        printf("- %s\n", choices[i]);
        if (choices[i] == best_support) best_chosen = true;
    }

    if (best_chosen) {
        say(loc, "alex_thanks", sweet_mode);
        inventory->money += 40;
        add_item(inventory, "mask");
        add_item(inventory, "ticket");
        inventory->alex_rewarded = true;
    } else {
        say(loc, "alex_thanks2", sweet_mode);
    }
}

// Вывести текущую локацию и запустить то, что происходит при входе.
// Некоторые локации - просто сцены, которые сами ведут дальше,
// а некоторые - концовки.
GameState enter_location(char *location, Inventory *inventory, bool sweet_mode) {
    printf("\n---\n");

    for (;;) {
        const char *next = NULL; // Автоматический переход в другую локацию
        bool ending = false;
        const char *loc = location;

        if (is(loc, "base")) {
            say(loc, "base_intro", sweet_mode);
            say(loc, "base_directions", sweet_mode);
            if (inventory->met_patrick) {
                say(loc, "base_patrick", sweet_mode);
            } else if (!inventory->met_alex) {
                say(loc, "base_alex", sweet_mode);
            }
            if (!inventory->worker_task && !inventory->met_patrick) {
                say(loc, "base_worker", sweet_mode);
            }
            say(loc, "base_what", sweet_mode);
            hint(loc, "base_hints", sweet_mode);
        } else if (is(loc, "alex_rivers")) {
            if (!inventory->met_alex) {
                say(loc, "alex_intro", sweet_mode);
                say(loc, "alex_greeting", sweet_mode);
                say(loc, "alex_watch1", sweet_mode);
                say(loc, "alex_talking", sweet_mode);
                for (int i = 0; i < 5; i++) {
                    if (sweet_mode) {
                        printf("...blah, blah, blah! (%d minutes)... they seem so passionate!\n", 5 - i);
                    } else {
                        printf("...blah, blah, blah! (%d minutes)\n", 5 - i);
                    }
                }
                say(loc, "alex_watch2", sweet_mode);
                say(loc, "alex_run", sweet_mode);
                say(loc, "alex_time_wasted", sweet_mode);
                say(loc, "alex_continue", sweet_mode);
                inventory->met_alex = true;
                if (!has_item(inventory, "ticket")) {
                    say(loc, "alex_no_ticket", sweet_mode);
                    next = "base";
                } else {
                    next = "security";
                }
            } else {
                say(loc, "alex_again", sweet_mode);
                say(loc, "alex_you_again", sweet_mode);
                if (!inventory->alex_rewarded) {
                    support_alex(inventory, sweet_mode);
                }
                say(loc, "alex_what", sweet_mode);
                hint(loc, "alex_hints", sweet_mode);
            }
        } else if (is(loc, "Patrick")) {
            say(loc, "patrick_intro", sweet_mode);
            say(loc, "patrick_greeting", sweet_mode);
            say(loc, "patrick_moves", sweet_mode);
            say(loc, "patrick_what", sweet_mode);
            hint(loc, "patrick_hints", sweet_mode);
        } else if (is(loc, "entrance")) {
            say(loc, "entrance_line", sweet_mode);
            say(loc, has_item(inventory, "ticket") ? "entrance_ticket" : "entrance_no_ticket", sweet_mode);
            say(loc, "entrance_what", sweet_mode);
            hint(loc, "entrance_hints", sweet_mode);
        } else if (is(loc, "ticket_booth")) {
            say(loc, "ticket_price", sweet_mode);
            say(loc, "ticket_what", sweet_mode);
            hint(loc, "ticket_hints", sweet_mode);
        } else if (is(loc, "security")) {
            say(loc, "security_check", sweet_mode);
            say(loc, has_item(inventory, "ticket") ? "security_pass" : "security_no_ticket", sweet_mode);
            say(loc, "security_what", sweet_mode);
            hint(loc, "security_hints", sweet_mode);
        } else if (is(loc, "elevator")) {
            say(loc, "elevator_close", sweet_mode);
            say(loc, "elevator_up", sweet_mode);
            say(loc, "elevator_pop", sweet_mode);
            say(loc, "elevator_ding", sweet_mode);
            next = "lookout";
        } else if (is(loc, "lookout")) {
            say(loc, "lookout_view", sweet_mode);
            say(loc, "lookout_see", sweet_mode);
            say(loc, "lookout_directions", sweet_mode);
            say(loc, "lookout_what", sweet_mode);
            hint(loc, "lookout_hints", sweet_mode);
        } else if (is(loc, "glass_floor")) {
            say(loc, "glass_scary", sweet_mode);
            say(loc, "glass_see", sweet_mode);
            say(loc, "glass_directions", sweet_mode);
            say(loc, "glass_what", sweet_mode);
            hint(loc, "glass_hints", sweet_mode);
        } else if (is(loc, "edgewalk_registration")) {
            say(loc, "edgewalk_desk", sweet_mode);
            say(loc, has_item(inventory, "edgewalk_ticket") ? "edgewalk_ticket" : "edgewalk_no_ticket", sweet_mode);
            say(loc, "edgewalk_what", sweet_mode);
            hint(loc, "edgewalk_hints", sweet_mode);
        } else if (is(loc, "edgewalk_preparation")) {
            say(loc, "edgewalk_prep", sweet_mode);
            say(loc, "edgewalk_nervous", sweet_mode);
            say(loc, "edgewalk_check", sweet_mode);
            next = "edgewalk";
        } else if (is(loc, "edgewalk")) {
            say(loc, "edgewalk_outside", sweet_mode);
            say(loc, "edgewalk_exciting", sweet_mode);
            say(loc, "edgewalk_win", sweet_mode);
            ending = true;
        } else if (is(loc, "gift_shop")) {
            say(loc, "gift_souvenirs", sweet_mode);
            if (!has_item(inventory, "mask")) {
                say(loc, "gift_mask", sweet_mode);
            }
            say(loc, "gift_what", sweet_mode);
            hint(loc, "gift_hints", sweet_mode);
        } else if (is(loc, "information_booth")) {
            say(loc, "info_brochures", sweet_mode);
            say(loc, "info_staff", sweet_mode);
            say(loc, "info_what", sweet_mode);
            hint(loc, "info_hints", sweet_mode);
        } else if (is(loc, "worker")) {
            say(loc, "worker_tired", sweet_mode);
            say(loc, "worker_help", sweet_mode);
            say(loc, "worker_start", sweet_mode);
            for (int i = 1; i <= 4; i++) {
                printf("You carry box %d to the storage room...\n", i);
            }
            say(loc, "worker_fifth", sweet_mode);
            say(loc, "worker_what", sweet_mode);
            hint(loc, "worker_hints", sweet_mode);
        } else if (is(loc, "open_box")) {
            say(loc, "box_contents", sweet_mode);
            say(loc, "box_what", sweet_mode);
            hint(loc, "box_hints", sweet_mode);
        } else if (is(loc, "caught_stealing")) {
            say(loc, "caught_seen", sweet_mode);
            say(loc, "caught_worker", sweet_mode);
            say(loc, "caught_security", sweet_mode);
            say(loc, "caught_what", sweet_mode);
            hint(loc, "caught_hints", sweet_mode);
        } else if (is(loc, "storage_room")) {
            say(loc, "storage_thanks", sweet_mode);
            if (!inventory->worker_task) { // Платят только один раз
                say(loc, "storage_reward", sweet_mode);
                say(loc, "storage_money", sweet_mode);
                inventory->money += 20;
                inventory->worker_task = true;
            }
            say(loc, "storage_what", sweet_mode);
            hint(loc, "storage_hints", sweet_mode);
        } else if (is(loc, "just_a_chill_guy")) {
            say(loc, "chill_laughing", sweet_mode);
            if (has_item(inventory, "mask") && has_item(inventory, "bible")) {
                say(loc, "chill_ready", sweet_mode);
                if (!ui_active()) {
                    printf("Hints: 'Go West', 'Use Mask', 'Use Bible', 'Ask About Corner', 'Back', 'Exit', 'Restart'.\n");
                }
            } else if (has_item(inventory, "bible") && !has_item(inventory, "mask")) {
                say(loc, "chill_quadrobists_gone", sweet_mode);
                say(loc, "chill_what", sweet_mode);
                hint(loc, "chill_hints1", sweet_mode);
            } else if (inventory->used_bible) {
                say(loc, "chill_scared_quadrobists", sweet_mode);
                say(loc, "chill_what", sweet_mode);
                hint(loc, "chill_hints2", sweet_mode);
            } else {
                say(loc, "chill_what", sweet_mode);
                hint(loc, "chill_hints3", sweet_mode);
            }
        } else if (is(loc, "corner")) {
            if (has_item(inventory, "mask") && inventory->used_mask) {
                say(loc, "corner_peek", sweet_mode);
                say(loc, "corner_back", sweet_mode);
                inventory->used_mask = false;
                next = "just_a_chill_guy";
            } else if (inventory->used_bible) {
                say(loc, "corner_empty", sweet_mode);
                next = "just_a_chill_guy";
            } else {
                say(loc, "corner_seen", sweet_mode);
                say(loc, "corner_join", sweet_mode);
                say(loc, "corner_quadrobist", sweet_mode);
                next = "quadrobics_base";
            }
        } else if (is(loc, "quadrobics_base")) {
            say(loc, "quadrobics_move", sweet_mode);
            hint(loc, "quadrobics_hints", sweet_mode);
        } else if (is(loc, "alex_rivers_quadrobics")) {
            printf("You approach Alex Rivers, moving like a quadrobist.\n");
            printf("Alex is startled, drops their phone, and their recording is ruined.\n");
            printf("You've ruined their day. (Bad Ending)\n");
            ending = true;
        } else if (is(loc, "scare_alex")) {
            say(loc, "scare_success", sweet_mode);
            say(loc, "scare_drop", sweet_mode);
            hint(loc, "scare_hints", sweet_mode);
        } else if (is(loc, "phone_found")) {
            say(loc, "phone_run", sweet_mode);
            say(loc, "phone_what", sweet_mode);
            hint(loc, "phone_hints", sweet_mode);
        } else if (is(loc, "roof")) {
            say(loc, "roof_jump", sweet_mode);
            printf("(Bad Ending)\n");
            ending = true;
        } else {
            printf("Invalid location: %s\n", loc);
            next = "base";
        }

        if (ending) {
            printf("---\n");
            return GAME_OVER;
        }
        if (next == NULL) {
            break;
        }
        set_location(location, next);
    }

    printf("---\n");
    return GAME_CONTINUE;
}

static void display_help(bool sweet_mode) {
    say("any", "help1", sweet_mode);
    say("any", "help2", sweet_mode);
    say("any", "help3", sweet_mode);
    say("any", "help4", sweet_mode);
    say("any", "help5", sweet_mode);
}

// Прочитать строку в меню отладки (NULL, если ввод закончился)
static char *debug_input(const char *prompt) {
    printf("%s", prompt);
    return get_player_input();
}

// Скрытое меню отладки, как в Python-версии.
// Возвращает GAME_ENTER, если локация сменилась, иначе GAME_CONTINUE.
static GameState debug_menu(char *location, Inventory *inventory, bool *sweet_mode) {
    for (;;) {
        printf("\n--- Debug Menu ---\n");
        printf("1. Add Money\n");
        printf("2. Add Item\n");
        printf("3. Remove Item\n");
        printf("4. Set Location\n");
        printf("5. View Inventory\n");
        printf("6. Exit Debug Menu\n");
        printf("7. Toggle Sweet+ Mode\n");

        char *choice = debug_input("Enter choice: ");
        if (choice == NULL) {
            return GAME_CONTINUE;
        }

        if (is(choice, "1")) {
            char *amount = debug_input("Enter amount of money to add: ");
            int value = 0;
            if (amount != NULL && sscanf(amount, "%d", &value) == 1) {
                inventory->money += value;
                printf("Added $%d. Current money: $%d\n", value, inventory->money);
            } else {
                printf("Invalid amount.\n");
            }
            free(amount);
        } else if (is(choice, "2") || is(choice, "3")) {
            bool adding = is(choice, "2");
            printf("Available items:");
            for (int i = 0; valid_items[i] != NULL; i++) {
                printf("%s %s", i == 0 ? "" : ",", valid_items[i]);
            }
            printf("\n");
            char *item = debug_input(adding ? "Enter item name to add: " : "Enter item name to remove: ");
            bool known = false;
            for (int i = 0; item != NULL && valid_items[i] != NULL; i++) {
                if (is(item, valid_items[i])) known = true;
            }
            if (!known) {
                printf("Invalid item name.\n");
            } else if (adding) {
                add_item(inventory, item);
                printf("%s added to inventory.\n", item);
            } else {
                remove_item(inventory, item);
                printf("%s removed from inventory.\n", item);
            }
            free(item);
        } else if (is(choice, "4")) {
            printf("Locations:");
            for (int i = 0; locations[i] != NULL; i++) {
                printf("%s %s", i == 0 ? "" : ",", locations[i]);
            }
            printf("\n");
            char *new_location = debug_input("Enter the location to set: ");
            if (new_location != NULL && is(new_location, "patrick")) {
                new_location[0] = 'P'; // Ввод в нижнем регистре, а имя локации - нет
            }
            if (new_location != NULL && is_valid_location(new_location)) {
                set_location(location, new_location);
                free(new_location);
                free(choice);
                return GAME_ENTER;
            }
            printf("Invalid location.\n");
            free(new_location);
        } else if (is(choice, "5")) {
            display_inventory(inventory);
        } else if (is(choice, "6")) {
            printf("Exiting debug menu...\n");
            free(choice);
            return GAME_CONTINUE;
        } else if (is(choice, "7")) {
            *sweet_mode = !*sweet_mode;
            printf("Sweet+ Mode %s\n", *sweet_mode ? "enabled" : "disabled");
        } else {
            printf("Invalid choice.\n");
        }
        free(choice);
    }
}

// Обработать команду игрока (уже в нижнем регистре и без лишних пробелов).
GameState process_command(const char *command, char *location, Inventory *inventory, bool *sweet_mode) {
    const char *loc = location;
    const bool sweet = *sweet_mode;
    const char *new_location = NULL;

    // --- Команды, которые работают везде ---
    if (is(command, "")) {
        return GAME_CONTINUE;
    } else if (is(command, "exit") || is(command, "quit")) {
        return GAME_EXIT;
    } else if (is(command, "restart")) {
        return GAME_RESTART;
    } else if (is(command, "help")) {
        display_help(sweet);
        return GAME_CONTINUE;
    } else if (is(command, "inventory")) {
        display_inventory(inventory);
        return GAME_CONTINUE;
    } else if (is(command, "look")) {
        return GAME_ENTER;
    } else if (is(command, "save")) {
        save_game(location, inventory, SAVE_FILE);
        return GAME_CONTINUE;
    } else if (is(command, "load")) {
        return load_game(location, inventory, SAVE_FILE) ? GAME_ENTER : GAME_CONTINUE;
    } else if (is(command, "debug")) {
        return debug_menu(location, inventory, sweet_mode);
    }

    // --- Команды локаций ---
    if (is(loc, "base")) {
        if (is(command, "go north")) {
            new_location = "entrance";
        } else if (is(command, "go east")) {
            new_location = "gift_shop";
        } else if (is(command, "go west")) {
            new_location = inventory->met_patrick ? "Patrick" : "alex_rivers";
        } else if (is(command, "go south")) {
            if (!inventory->worker_task && !inventory->met_patrick) {
                new_location = "worker";
            } else {
                printf("There's nothing to do there anymore.\n");
            }
        } else if (is(command, "look around")) {
            say(loc, "look_around_base", sweet);
        } else {
            say(loc, "invalid_command", sweet);
        }
    } else if (is(loc, "alex_rivers")) {
        if (is(command, "compliment alex")) {
            say(loc, "alex_compliment", sweet);
            say(loc, "alex_nothing", sweet);
            new_location = "base";
        } else if (is(command, "ignore") || is(command, "back")) {
            say(loc, "alex_ignore", sweet);
            new_location = "base";
        } else {
            say(loc, "invalid_command", sweet);
        }
    } else if (is(loc, "Patrick")) {
        if (is(command, "join")) {
            say(loc, "patrick_join1", sweet);
            say(loc, "patrick_join2", sweet);
            say(loc, "patrick_join3", sweet);
            say(loc, "patrick_join4", sweet);
            say(loc, "patrick_join5", sweet);
            return GAME_OVER;
        } else if (is(command, "decline")) {
            say(loc, "patrick_decline", sweet);
            new_location = "base";
        } else if (is(command, "back")) {
            new_location = "base";
        } else {
            say(loc, "invalid_command", sweet);
        }
    } else if (is(loc, "entrance")) {
        if (is(command, "go north")) {
            if (has_item(inventory, "ticket")) {
                new_location = "security";
            } else {
                say(loc, "entrance_no_ticket", sweet);
            }
        } else if (is(command, "go west")) {
            new_location = "ticket_booth";
        } else if (is(command, "back")) {
            new_location = "base";
        } else {
            say(loc, "invalid_command", sweet);
        }
    } else if (is(loc, "ticket_booth")) {
        if (is(command, "buy ticket")) {
            if (has_item(inventory, "ticket")) {
                printf("You already have a ticket.\n");
            } else if (pay(inventory, TICKET_PRICE)) {
                add_item(inventory, "ticket");
                printf("You bought a ticket for $%d.\n", TICKET_PRICE);
            }
        } else if (is(command, "back")) {
            new_location = "entrance";
        } else {
            say(loc, "invalid_command", sweet);
        }
    } else if (is(loc, "security")) {
        if (is(command, "go north")) {
            if (has_item(inventory, "ticket")) {
                new_location = "elevator";
            } else {
                say(loc, "security_no_ticket", sweet);
            }
        } else if (is(command, "back")) {
            new_location = "entrance";
        } else {
            say(loc, "invalid_command", sweet);
        }
    } else if (is(loc, "lookout")) {
        if (is(command, "go down")) {
            new_location = "glass_floor";
        } else if (is(command, "go east")) {
            new_location = "information_booth";
        } else if (is(command, "go back") || is(command, "back")) {
            say(loc, "lookout_down", sweet);
            new_location = "base";
        } else if (is(command, "look around")) {
            say(loc, "lookout_look", sweet);
        } else {
            say(loc, "invalid_command", sweet);
        }
    } else if (is(loc, "glass_floor")) {
        if (is(command, "go up") || is(command, "back")) {
            new_location = "lookout";
        } else if (is(command, "go west")) {
            new_location = "edgewalk_registration";
        } else if (is(command, "go east")) {
            new_location = "just_a_chill_guy";
        } else if (is(command, "look down")) {
            say(loc, "glass_look_down", sweet);
        } else {
            say(loc, "invalid_command", sweet);
        }
    } else if (is(loc, "edgewalk_registration")) {
        if (is(command, "buy ticket") || is(command, "buy edgewalk ticket")) {
            if (has_item(inventory, "edgewalk_ticket")) {
                printf("You already have an EdgeWalk ticket.\n");
            } else if (pay(inventory, EDGEWALK_PRICE)) {
                add_item(inventory, "edgewalk_ticket");
                printf("You bought an EdgeWalk ticket for $%d.\n", EDGEWALK_PRICE);
            }
        } else if (is(command, "go north")) {
            if (has_item(inventory, "edgewalk_ticket")) {
                new_location = "edgewalk_preparation";
            } else {
                say(loc, "edgewalk_no_ticket", sweet);
            }
        } else if (is(command, "back")) {
            new_location = "glass_floor";
        } else {
            say(loc, "invalid_command", sweet);
        }
    } else if (is(loc, "gift_shop")) {
        if (is(command, "buy postcards")) {
            if (pay(inventory, 5)) {
                add_item(inventory, "postcards");
                printf("You bought postcards for $5.\n");
            }
        } else if (is(command, "buy souvenir")) {
            if (pay(inventory, 15)) {
                add_item(inventory, "souvenir");
                printf("You bought a CN Tower souvenir for $15.\n");
            }
        } else if (is(command, "buy mask")) {
            if (has_item(inventory, "mask")) {
                printf("You already have a mask.\n");
            } else if (pay(inventory, 20)) {
                add_item(inventory, "mask");
                printf("You bought a mask for $20.\n");
            }
        } else if (is(command, "back")) {
            new_location = "base";
        } else {
            say(loc, "invalid_command", sweet);
        }
    } else if (is(loc, "information_booth")) {
        if (is(command, "ask about history")) {
            print_cn_tower_art();
            say(loc, "history1", sweet);
            say(loc, "history2", sweet);
            say(loc, "history3", sweet);
        } else if (is(command, "ask about building")) {
            print_cn_tower_art();
            say(loc, "building1", sweet);
            say(loc, "building2", sweet);
            say(loc, "building3", sweet);
        } else if (is(command, "return phone")) {
            if (has_item(inventory, "alex_phone") && !inventory->phone_returned) {
                say(loc, "info_phone1", sweet);
                say(loc, "info_phone2", sweet);
                remove_item(inventory, "alex_phone");
                inventory->money += PHONE_REWARD;
                inventory->phone_returned = true;
            } else {
                say(loc, "info_no_phone", sweet);
            }
        } else if (is(command, "back")) {
            new_location = "lookout";
        } else {
            say(loc, "invalid_command", sweet);
        }
    } else if (is(loc, "worker")) {
        if (is(command, "look inside") || is(command, "help worker")) {
            new_location = "open_box";
        } else if (is(command, "continue")) {
            new_location = "storage_room";
        } else if (is(command, "back")) {
            new_location = "base";
        } else {
            say(loc, "invalid_command", sweet);
        }
    } else if (is(loc, "open_box")) {
        if (is(command, "take nothing")) {
            printf("You decide to leave the box alone and continue helping the worker.\n");
            new_location = "storage_room";
        } else if (is(command, "take money")) {
            inventory->money += 40;
            printf("You discreetly take the money from the box.\n");
            new_location = "caught_stealing";
        } else if (is(command, "take book")) {
            add_item(inventory, "bible");
            printf("You take the book from the box. It's a Bible.\n");
            new_location = "storage_room";
        } else if (is(command, "take mask")) {
            add_item(inventory, "mask");
            printf("You take the mask from the box.\n");
            new_location = "storage_room";
        } else {
            say(loc, "invalid_command", sweet);
        }
    } else if (is(loc, "caught_stealing")) {
        if (is(command, "tell truth")) {
            printf("You confess to taking the money. The police let you go with a warning.\n");
            printf("Next day, you go to the CN Tower again, but missed Alex Rivers and a chance for a free ticket.\n");
            printf("You see a strange guy near the entrance.\n");
            inventory->met_alex = true;
            inventory->met_patrick = true;
            new_location = "base";
        } else if (is(command, "bribe")) {
            if (inventory->money >= 50) {
                inventory->money -= 50;
                printf("You offer the officer a bribe. They reluctantly accept.\n");
                printf("Officer: \"Alright, get back to the CN Tower. And don't let me catch you again.\"\n");
                new_location = "base";
            } else {
                printf("You don't have enough money to bribe the officer.\n");
            }
        } else if (is(command, "lie")) {
            printf("You try to lie your way out of it, but the police don't believe you.\n");
            printf("You're deported. No more CN Tower for you. (Bad Ending)\n");
            return GAME_OVER;
        } else {
            say(loc, "invalid_command", sweet);
        }
    } else if (is(loc, "storage_room")) {
        if (is(command, "back")) {
            new_location = "base";
        } else {
            say(loc, "invalid_command", sweet);
        }
    } else if (is(loc, "just_a_chill_guy")) {
        if (is(command, "use mask")) {
            if (has_item(inventory, "mask")) {
                inventory->used_mask = true;
                new_location = "corner";
            } else {
                printf("You don't have a mask.\n");
            }
        } else if (is(command, "use bible")) {
            if (has_item(inventory, "bible")) {
                printf("You wave the Bible around. The quadrobists scatter in fear!\n");
                inventory->used_bible = true;
            } else {
                printf("You don't have a Bible.\n");
            }
        } else if (is(command, "go forward")) {
            new_location = "corner";
        } else if (is(command, "ask about corner")) {
            printf("Just a Chill Guy: \"Just some quadrobists practicing. Nothing to worry about... unless you're scared.\"\n");
        } else if (is(command, "go west")) {
            if (has_item(inventory, "mask") && has_item(inventory, "bible")) {
                new_location = "scare_alex";
            } else {
                printf("Just a Chill Guy: \"You'll need a mask and a Bible to scare Alex.\"\n");
            }
        } else if (is(command, "back")) {
            new_location = "glass_floor";
        } else {
            say(loc, "invalid_command", sweet);
        }
    } else if (is(loc, "quadrobics_base")) {
        if (is(command, "go west")) {
            new_location = "alex_rivers_quadrobics";
        } else {
            say(loc, "invalid_command", sweet);
        }
    } else if (is(loc, "scare_alex")) {
        if (is(command, "take phone")) {
            printf("You grab Alex's phone. It's yours now!\n");
            add_item(inventory, "alex_phone");
            new_location = "phone_found";
        } else if (is(command, "leave phone")) {
            printf("You decide to leave the phone. What were you thinking? (Bad Ending)\n");
            return GAME_OVER;
        } else {
            say(loc, "invalid_command", sweet);
        }
    } else if (is(loc, "phone_found")) {
        if (is(command, "jump")) {
            new_location = "roof";
        } else if (is(command, "go back") || is(command, "back")) {
            new_location = "glass_floor";
        } else {
            say(loc, "invalid_command", sweet);
        }
    } else {
        say(loc, "invalid_command", sweet);
    }

    if (new_location != NULL) {
        set_location(location, new_location);
        return GAME_ENTER;
    }
    return GAME_CONTINUE;
}

// Название локации для строки статуса
static const char *location_title(const char *location) {
    static const char *titles[][2] = {
        {"base", "Base of the CN Tower"}, {"alex_rivers", "Alex Rivers"},
        {"Patrick", "Patrick"}, {"entrance", "Entrance"},
        {"ticket_booth", "Ticket Booth"}, {"security", "Security Check"},
        {"elevator", "Elevator"}, {"lookout", "LookOut Level"},
        {"glass_floor", "Glass Floor"}, {"edgewalk_registration", "EdgeWalk Desk"},
        {"edgewalk_preparation", "EdgeWalk Prep"}, {"edgewalk", "EdgeWalk"},
        {"gift_shop", "Gift Shop"}, {"information_booth", "Info Booth"},
        {"worker", "Worker"}, {"open_box", "Open Box"},
        {"caught_stealing", "Caught!"}, {"storage_room", "Storage Room"},
        {"just_a_chill_guy", "Just a Chill Guy"}, {"corner", "Corner"},
        {"quadrobics_base", "Quadrobics"}, {"alex_rivers_quadrobics", "Alex Rivers"},
        {"scare_alex", "Alex Rivers"}, {"phone_found", "Roof Edge"}, {"roof", "Roof"},
    };
    for (size_t i = 0; i < sizeof(titles) / sizeof(titles[0]); i++) {
        if (is(titles[i][0], location)) return titles[i][1];
    }
    return location;
}

void format_status(char *buf, size_t size, const char *location, Inventory *inventory, bool sweet_mode) {
    size_t len = 0;
    bool first_item = true;

    snprintf(buf, size, "CN Tower | %s | $%d", location_title(location), inventory->money);
    for (int i = 0; valid_items[i] != NULL; i++) {
        if (has_item(inventory, valid_items[i])) {
            len = strlen(buf);
            snprintf(buf + len, size - len, "%s%s", first_item ? " | Items: " : ", ", valid_items[i]);
            first_item = false;
        }
    }
    if (sweet_mode) {
        len = strlen(buf);
        snprintf(buf + len, size - len, " | Sweet+");
    }
}

// Команды, которые работают в любой локации
const char *global_commands[] = {
    "Look", "Inventory", "Help", "Save", "Load", "Debug", "Restart", "Exit", NULL
};

// Действия, которые сейчас имеют смысл в локации: для подсказок и автодополнения по Tab
int available_commands(const char *location, Inventory *inventory, const char **out, int max) {
    const char *list[16];
    int n = 0;
    const char *loc = location;

#define ADD(cmd) (list[n++] = (cmd))
    if (is(loc, "base")) {
        ADD("Go North"); ADD("Go East"); ADD("Go West");
        if (!inventory->worker_task && !inventory->met_patrick) ADD("Go South");
        ADD("Look Around");
    } else if (is(loc, "alex_rivers")) {
        ADD("Compliment Alex"); ADD("Ignore");
    } else if (is(loc, "Patrick")) {
        ADD("Join"); ADD("Decline"); ADD("Back");
    } else if (is(loc, "entrance")) {
        if (has_item(inventory, "ticket")) ADD("Go North");
        ADD("Go West"); ADD("Back");
    } else if (is(loc, "ticket_booth")) {
        if (!has_item(inventory, "ticket")) ADD("Buy Ticket");
        ADD("Back");
    } else if (is(loc, "security")) {
        if (has_item(inventory, "ticket")) ADD("Go North");
        ADD("Back");
    } else if (is(loc, "lookout")) {
        ADD("Go Down"); ADD("Go East"); ADD("Go Back"); ADD("Look Around");
    } else if (is(loc, "glass_floor")) {
        ADD("Go Up"); ADD("Go West"); ADD("Go East"); ADD("Look Down");
    } else if (is(loc, "edgewalk_registration")) {
        if (has_item(inventory, "edgewalk_ticket")) ADD("Go North");
        else ADD("Buy Ticket");
        ADD("Back");
    } else if (is(loc, "gift_shop")) {
        ADD("Buy Postcards"); ADD("Buy Souvenir");
        if (!has_item(inventory, "mask")) ADD("Buy Mask");
        ADD("Back");
    } else if (is(loc, "information_booth")) {
        ADD("Ask About History"); ADD("Ask About Building");
        if (has_item(inventory, "alex_phone") && !inventory->phone_returned) ADD("Return Phone");
        ADD("Back");
    } else if (is(loc, "worker")) {
        ADD("Look Inside"); ADD("Continue"); ADD("Back");
    } else if (is(loc, "open_box")) {
        ADD("Take Nothing"); ADD("Take Money"); ADD("Take Book"); ADD("Take Mask");
    } else if (is(loc, "caught_stealing")) {
        ADD("Tell Truth"); ADD("Bribe"); ADD("Lie");
    } else if (is(loc, "storage_room")) {
        ADD("Back");
    } else if (is(loc, "just_a_chill_guy")) {
        if (has_item(inventory, "mask") && has_item(inventory, "bible")) ADD("Go West");
        if (has_item(inventory, "mask")) ADD("Use Mask");
        if (has_item(inventory, "bible")) ADD("Use Bible");
        ADD("Go Forward"); ADD("Ask About Corner"); ADD("Back");
    } else if (is(loc, "quadrobics_base")) {
        ADD("Go West");
    } else if (is(loc, "scare_alex")) {
        ADD("Take Phone"); ADD("Leave Phone");
    } else if (is(loc, "phone_found")) {
        ADD("Jump"); ADD("Go Back");
    }
#undef ADD

    if (n > max) n = max;
    for (int i = 0; i < n; i++) out[i] = list[i];
    return n;
}

// Работа с инвентарём
static bool *item_flag(Inventory *inventory, const char *item) {
    if (is(item, "ticket")) return &inventory->ticket;
    if (is(item, "mask")) return &inventory->mask;
    if (is(item, "edgewalk_ticket")) return &inventory->edgewalk_ticket;
    if (is(item, "postcards")) return &inventory->postcards;
    if (is(item, "souvenir")) return &inventory->souvenir;
    if (is(item, "bible")) return &inventory->bible;
    if (is(item, "alex_phone")) return &inventory->alex_phone;
    return NULL; // Нет такого предмета
}

bool has_item(Inventory *inventory, const char *item) {
    bool *flag = item_flag(inventory, item);
    return flag != NULL && *flag;
}

void add_item(Inventory *inventory, const char *item) {
    bool *flag = item_flag(inventory, item);
    if (flag != NULL) *flag = true;
}

void remove_item(Inventory *inventory, const char *item) {
    bool *flag = item_flag(inventory, item);
    if (flag != NULL) *flag = false;
}

void display_inventory(Inventory *inventory) {
    printf("Inventory: Money=$%d", inventory->money);
    for (int i = 0; valid_items[i] != NULL; i++) {
        if (has_item(inventory, valid_items[i])) {
            printf(", %s", valid_items[i]);
        }
    }
    printf("\n");
}
