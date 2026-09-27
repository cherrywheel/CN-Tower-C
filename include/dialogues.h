#ifndef DIALOGUES_H
#define DIALOGUES_H

#include <stdbool.h> // Для bool

// Структура реплики
typedef struct {
    const char *location;
    const char *key;
    const char *text;
    const char *sweet_text; // Текст для режима Sweet+
} DialogueEntry;


// Получить реплику
const char *get_dialogue(const char *location, const char *key, bool sweet_mode);

#endif // DIALOGUES_H
