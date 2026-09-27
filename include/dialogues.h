#ifndef DIALOGUES_H
#define DIALOGUES_H

#include <stdbool.h> // for bool

// one line of dialogue
typedef struct {
    const char *location;
    const char *key;
    const char *text;
    const char *sweet_text; // text for sweet+ mode
} DialogueEntry;


// get a line of dialogue
const char *get_dialogue(const char *location, const char *key, bool sweet_mode);

#endif // DIALOGUES_H
