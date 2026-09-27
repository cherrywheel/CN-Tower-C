#include "game.h"
#include "ui.h"

#define MAX_COMMANDS 32

// Проверка возраста, как в Python-версии: спросить один раз и запомнить
static bool check_age(void) {
    int age = load_age(AGE_FILE);
    if (age >= MIN_AGE) {
        printf("Welcome back! Your age (%d) was loaded automatically.\n", age);
        return true;
    }

    for (;;) {
        printf("Enter your age: ");
        char *input = get_player_input();
        if (input == NULL) {
            return false;
        }
        bool valid = sscanf(input, "%d", &age) == 1;
        free(input);
        if (!valid) {
            printf("Invalid input. Please enter a number.\n");
            continue;
        }
        if (age < MIN_AGE) {
            printf("Sorry, you must be %d or older to play this game.\n", MIN_AGE);
            return false;
        }
        save_age(age, AGE_FILE);
        return true;
    }
}

int main(int argc, char **argv) {
    char location[LOCATION_SIZE];
    Inventory inventory;
    bool sweet_mode = false;
    bool allow_tui = getenv("CN_TOWER_PLAIN") == NULL;

    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "--plain") == 0) {
            allow_tui = false; // Обычный построчный режим без TUI
        }
    }

    const char *seed = getenv("CN_TOWER_SEED"); // Чтобы варианты у Алекса повторялись (для тестов)
    game_seed(seed != NULL ? (uint32_t)strtoul(seed, NULL, 10) : (uint32_t)time(NULL));

    ui_init(allow_tui);
    ui_set_status("CN Tower");
    printf("=== CN Tower ===\n");
    if (!check_age()) {
        ui_shutdown();
        return 0;
    }

    new_game(location, &inventory);
    printf("Welcome to the CN Tower Experience Simulator!\n");
    printf("Type 'Help' for commands.\n");

    GameState state = GAME_ENTER;
    while (state != GAME_EXIT) {
        if (state == GAME_ENTER) {
            state = enter_location(location, &inventory, sweet_mode);
            continue;
        }
        if (state == GAME_RESTART) {
            printf("Restarting the game...\n");
            new_game(location, &inventory);
            state = GAME_ENTER;
            continue;
        }
        if (state == GAME_OVER) {
            printf("Game over. Type 'Restart' to play again, 'Load' to load your save or 'Exit' to quit.\n");
        }

        // Статус и подсказки для TUI
        char status[256];
        const char *commands[MAX_COMMANDS];
        int count = 0;
        int primary = 0;
        if (state == GAME_OVER) {
            commands[count++] = "Restart";
            commands[count++] = "Load";
            commands[count++] = "Exit";
            primary = count;
        } else {
            count = primary = available_commands(location, &inventory, commands, MAX_COMMANDS);
            for (int i = 0; global_commands[i] != NULL && count < MAX_COMMANDS; i++) {
                commands[count++] = global_commands[i];
            }
        }
        format_status(status, sizeof(status), location, &inventory, sweet_mode);
        ui_set_status(status);
        ui_set_hints(commands, count, primary);

        printf("> ");
        char *command = get_player_input();
        if (command == NULL) {
            break; // Ввод закончился
        }

        if (state == GAME_OVER) {
            if (strcmp(command, "restart") == 0) {
                state = GAME_RESTART;
            } else if (strcmp(command, "exit") == 0 || strcmp(command, "quit") == 0) {
                state = GAME_EXIT;
            } else if (strcmp(command, "load") == 0 && load_game(location, &inventory, SAVE_FILE)) {
                state = GAME_ENTER;
            }
        } else {
            state = process_command(command, location, &inventory, &sweet_mode);
        }
        free(command);
    }

    ui_shutdown();
    printf("Thanks for playing!\n");
    return 0;
}
