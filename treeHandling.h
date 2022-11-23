#ifndef MORZE_TREEHANDLING_H
#define MORZE_TREEHANDLING_H

#include <wchar.h>

typedef struct DictionaryElement{
    wchar_t element[2][10];
} DictionaryElement;

typedef struct TreeBlock{
    DictionaryElement letter;
    struct TreeBlock* lbranch;
    struct TreeBlock* rbranch;
} TreeBlock;

TreeBlock* treeBuild(char *file_name, int direction);
void freeTree(TreeBlock *block);


#endif //MORZE_TREEHANDLING_H
