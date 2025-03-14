#ifndef DIALOGUES_H
#define DIALOGUES_H

#include <stdbool.h> // For bool

// Dialogue structure
typedef struct {
    const char *key;
    const char *text;
    const char *sweet_text; // Sweet+ mode text
} DialogueEntry;


// Function to get dialogue
const char *get_dialogue(const char *location, const char *key, bool sweet_mode);

#endif //DIALOGUES_H
