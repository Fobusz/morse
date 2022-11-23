#ifndef MORZE_TRANSLATETEXT_H
#define MORZE_TRANSLATETEXT_H

#include "treeHandling.h"

typedef struct ConvertParams {
    //From morse = 1, To morse = 0
    int fromIndex;
    int toIndex;
    //We can set what the program prints when it can't translate a character
    wchar_t invalidChar;
    //Shows what the character separator character is in the input text (In morse code, this is a space)
    wchar_t input_separator;
    //Shows what the character separator character should be in the output text
    wchar_t output_separator;
} ConvertParams;

void start_translation(int direction, TreeBlock *ekezetes, TreeBlock *morse);

#endif //MORZE_TRANSLATETEXT_H
