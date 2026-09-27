#include "game.h"
#include "ui.h"

#include <sys/stat.h>

#ifndef S_ISDIR
#define S_ISDIR(mode) (((mode) & S_IFMT) == S_IFDIR)
#endif

#define SAVE_MAGIC "CNT2" // Менять при изменении формата сохранения

// Путь к файлу данных: в ../data, если игра запущена из src,
// иначе рядом с тем местом, откуда её запустили (например, из релиза)
const char *data_file(const char *name) {
    static char path[256];
    struct stat st;
    bool in_repo = stat("../data", &st) == 0 && S_ISDIR(st.st_mode);
    snprintf(path, sizeof(path), "%s%s", in_repo ? "../data/" : "", name);
    return path;
}

// Простое сохранение/загрузка (без JSON, бинарный файл)
void save_game(const char *location, Inventory *inventory, const char *filename) {
    FILE *file = fopen(filename, "wb");
    if (file == NULL) {
        perror("Error saving game");
        return;
    }

    // Локация (буфер фиксированного размера для простоты)
    char loc_buffer[LOCATION_SIZE] = {0};
    snprintf(loc_buffer, sizeof(loc_buffer), "%s", location);

    bool ok = fwrite(SAVE_MAGIC, 4, 1, file) == 1
           && fwrite(loc_buffer, sizeof(loc_buffer), 1, file) == 1
           && fwrite(inventory, sizeof(Inventory), 1, file) == 1;

    if (fclose(file) != 0) ok = false;
    printf(ok ? "Game saved.\n" : "Error saving game.\n");
}

// Загрузка игры. При ошибке ничего не меняется.
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

// Возвращает сохранённый возраст или -1, если его нет
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
        return; // Не страшно, в следующий раз спросим снова
    }
    fprintf(file, "%d\n", age);
    fclose(file);
}

// Убрать пробелы в начале и конце строки
char *trim_whitespace(char *str) {
    char *end;

    // Пробелы в начале
    while(isspace((unsigned char)*str)) str++;

    if(*str == 0)  // Одни пробелы?
        return str;

    // Пробелы в конце
    end = str + strlen(str) - 1;
    while(end > str && isspace((unsigned char)*end)) end--;

    // Новый конец строки
    end[1] = '\0';

    return str;
}

// Прочитать строку игрока: без крайних пробелов, в нижнем регистре, с одиночными пробелами.
// Возвращает строку из malloc (освобождает вызывающий) или NULL, если ввод закончился.
char *get_player_input(void) {
    char *input = malloc(INPUT_SIZE); // Память под ввод
    if (input == NULL) {
        perror("Memory allocation failed");
        exit(EXIT_FAILURE); // Выходим, если память не выделилась
    }

    fflush(stdout);
    if (ui_active()) {
        if (!ui_read_line(input, INPUT_SIZE)) {
            free(input);
            return NULL;
        }
    } else {
        ui_set_hints(NULL, 0, 0); // Подсказки действуют на одно чтение
        if (fgets(input, INPUT_SIZE, stdin) == NULL) {
            free(input); // Освобождаем память, если fgets не сработал
            return NULL;
        }

        // Отбросить хвост слишком длинной строки
        if (strchr(input, '\n') == NULL) {
            int c;
            while ((c = getchar()) != '\n' && c != EOF) {}
        }
    }

    // Пишем результат в начало буфера, чтобы его можно было освободить
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
