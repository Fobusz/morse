#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

#include "treeHandling.h"
#include "debugmalloc.h"

static int readDictionary(DictionaryElement **dictionary, char *file_name) {

    //opening a UTF-8 file
    FILE *pfile = fopen(file_name, "r, ccs=UTF-8");

    //if pfile == NULL -> missing dictionary files
    if (!pfile) {
        wprintf(L"Nem található a szótárfájl: %s", file_name);
        return -1;
    }

    //true if reading the first char of the line
    bool first_char = true;
    int str_length = 0, arr_size = 0;
    DictionaryElement current_element;

    int column = 0;

    wint_t utf8_ch;
    while ((utf8_ch = fgetwc(pfile)) != WEOF) {
        switch (utf8_ch) {
            //reach the tabulator
            case 9:
                column = 1;
                first_char = !first_char;
                str_length = 0;
                break;
            //reach the end
            case 10: {
                arr_size++;

                DictionaryElement* reallocated_dictionary = realloc(*dictionary, arr_size * sizeof(DictionaryElement));
                if (reallocated_dictionary)
                    *dictionary = reallocated_dictionary;
                else {
                    printf("Out of memory");
                    exit(ENOMEM);
                }

                wcscpy((*dictionary + arr_size - 1)->element[0], current_element.element[0]);
                wcscpy((*dictionary + arr_size - 1)->element[1], current_element.element[1]);

                //clean up for the next line
                column = 0;
                current_element.element[0][0] = '\0';
                current_element.element[1][0] = '\0';

                first_char = true;
                str_length = 0;
                break;
            }
            //carriage return
            case 13:
                break;
            default: {
                current_element.element[column][str_length] = utf8_ch;
                current_element.element[column][++str_length] = '\0';
                break;
            }
        }
    }
    if (arr_size == 0) {
        wprintf(L"Üres a szótárfájl!");
    }
    return arr_size;
}


static int compChars(void* masterColumn, const void* a, const void* b) {

    DictionaryElement first  = (*(DictionaryElement *)a);
    DictionaryElement second  = (*(DictionaryElement *)b);
    int column = *(const int*)masterColumn;

    return wcscmp(first.element[column], second.element[column]);
}

static TreeBlock* makeBlock(DictionaryElement letter) {

    //allocating a new block
    TreeBlock *block = (TreeBlock* ) malloc(sizeof(TreeBlock));

    //the given letter will be the letter
    block->letter = letter;
    block->lbranch = NULL;
    block->rbranch = NULL;

    return block;
}

static TreeBlock* arrToTree(DictionaryElement *dictionary_arr, int first, int last) {

    if (first > last) return NULL;

    int mid = (first + last) / 2;
    //making a letter block for the main letter
    TreeBlock *main_letter = makeBlock(dictionary_arr[mid]);

    //first half of the array will be in the left branch
    main_letter->lbranch = arrToTree(dictionary_arr, first, mid - 1);
    //second half of the array will be in the right branch
    main_letter->rbranch = arrToTree(dictionary_arr, mid + 1, last);

    return main_letter;
}

void freeTree(TreeBlock *block) {

    if (block == NULL) {
        return;
    }
    freeTree(block->lbranch);
    freeTree(block->rbranch);
    free(block);
}

TreeBlock* treeBuild(char *file_name, int direction) {

    DictionaryElement *dictionary = NULL;
    int dictionary_length = readDictionary(&dictionary, file_name);

    //missing or empty dictionary file
    if (dictionary_length < 1) return 0;

    qsort_s(dictionary, dictionary_length, sizeof(DictionaryElement), compChars, &direction);

    //converting the sorted array to a Binary Search Tree
    TreeBlock *tree = arrToTree(dictionary, 0, dictionary_length - 1);
    free(dictionary);
    return tree;
}