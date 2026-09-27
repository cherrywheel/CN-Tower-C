#include "game.h"
#include "ui.h"

#include <sys/stat.h>

#ifndef S_ISDIR
#define S_ISDIR(mode) (((mode) & S_IFMT) == S_IFDIR)
#endif

#define SAVE_MAGIC "CNT2" // bump when the save layout changes

// data file path
// ../data when run from src and the current dir otherwise like a release build
const char *data_file(const char *name) {
    static char path[256];
    struct stat st;
    bool in_repo = stat("../data", &st) == 0 && S_ISDIR(st.st_mode);
    snprintf(path, sizeof(path), "%s%s", in_repo ? "../data/" : "", name);
    return path;
}

// simple save and load with a plain binary file no json
void save_game(const char *location, Inventory *inventory, const char *filename) {
    FILE *file = fopen(filename, "wb");
    if (file == NULL) {
        perror("Error saving game");
        return;
    }

    // location in a fixed size buffer to keep it simple
    char loc_buffer[LOCATION_SIZE] = {0};
    snprintf(loc_buffer, sizeof(loc_buffer), "%s", location);

    bool ok = fwrite(SAVE_MAGIC, 4, 1, file) == 1
           && fwrite(loc_buffer, sizeof(loc_buffer), 1, file) == 1
           && fwrite(inventory, sizeof(Inventory), 1, file) == 1;

    if (fclose(file) != 0) ok = false;
    printf(ok ? "Game saved.\n" : "Error saving game.\n");
}

// load the game and change nothing if it fails
bool load_game(char *location, Inventory *inventory, const char *filename) {
    FILE *file = fopen(filename, "rb");
    if (file == NULL) {
        printf("No saved game found.\n");
        return false;
    }

    char magic[4];
    char loc_buffer[LOCATION_SIZE];
    Inventory loaded;
    bool ok = fread(magic, sizeof(magic), 1, file) == 1
           && memcmp(magic, SAVE_MAGIC, sizeof(magic)) == 0
           && fread(loc_buffer, sizeof(loc_buffer), 1, file) == 1
           && fread(&loaded, sizeof(loaded), 1, file) == 1;
    fclose(file);

    if (ok) {
        loc_buffer[sizeof(loc_buffer) - 1] = '\0';
        ok = is_valid_location(loc_buffer);
    }
    if (!ok) {
        printf("The saved game is damaged or from an old version.\n");
        return false;
    }

    snprintf(location, LOCATION_SIZE, "%s", loc_buffer);
    *inventory = loaded;
    printf("Game loaded.\n");
    return true;
}

// saved age or -1 if there is none
int load_age(const char *filename) {
    FILE *file = fopen(filename, "r");
    int age = -1;
    if (file == NULL) {
        return -1;
    }
    if (fscanf(file, "%d", &age) != 1) {
        age = -1;
    }
    fclose(file);
    return age;
}

void save_age(int age, const char *filename) {
    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        return; // no big deal we just ask again next time
    }
    fprintf(file, "%d\n", age);
    fclose(file);
}

// trim whitespace on both ends
char *trim_whitespace(char *str) {
    char *end;

    // leading spaces
    while(isspace((unsigned char)*str)) str++;

    if(*str == 0)  // all spaces
        return str;

    // trailing spaces
    end = str + strlen(str) - 1;
    while(end > str && isspace((unsigned char)*end)) end--;

    // new end of string
    end[1] = '\0';

    return str;
}

// read a line from the player trimmed lowercased with single spaces
// returns a malloc'd string the caller frees or NULL at end of input
char *get_player_input(void) {
    char *input = malloc(INPUT_SIZE); // buffer for the input
    if (input == NULL) {
        perror("Memory allocation failed");
        exit(EXIT_FAILURE); // bail out if malloc fails
    }

    fflush(stdout);
    if (ui_active()) {
        if (!ui_read_line(input, INPUT_SIZE)) {
            free(input);
            return NULL;
        }
    } else {
        ui_set_hints(NULL, 0, 0); // hints only last for one read
        if (fgets(input, INPUT_SIZE, stdin) == NULL) {
            free(input); // free the buffer if fgets fails
            return NULL;
        }

        // drop the rest of a line thats too long
        if (strchr(input, '\n') == NULL) {
            int c;
            while ((c = getchar()) != '\n' && c != EOF) {}
        }
    }

    // write into the start of the buffer so it can still be freed
    char *src = trim_whitespace(input);
    char *dst = input;
    bool prev_space = false;
    for (; *src != '\0'; src++) {
        unsigned char c = (unsigned char)*src;
        if (isspace(c)) {
            if (!prev_space) *dst++ = ' ';
            prev_space = true;
        } else {
            *dst++ = (char)tolower(c);
            prev_space = false;
        }
    }
    *dst = '\0';
    return input;
}
