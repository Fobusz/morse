#include <wchar.h>

#include "translateText.h"
#include "paramCheck.h"
#include "debugmalloc.h"

static wchar_t *convertChar(wchar_t *input_char, TreeBlock *actual_tree, ConvertParams *convertParams) {

    if (actual_tree == NULL) {
        //in case an invalid character has been set, we need to return that
        if (convertParams->invalidChar == L'\0')
            return input_char;
        else {
            return &convertParams->invalidChar;
        }

    } else if (wcscmp(input_char, actual_tree->letter.element[convertParams->fromIndex]) == 0) {
        return actual_tree->letter.element[convertParams->toIndex];

    } else if (wcscmp(input_char, actual_tree->letter.element[convertParams->fromIndex]) < 0) {
        return convertChar(input_char, actual_tree->lbranch, convertParams);

    } else if (wcscmp(input_char, actual_tree->letter.element[convertParams->fromIndex]) > 0) {
        return convertChar(input_char, actual_tree->rbranch, convertParams);

    } else {
        return NULL;
    }
}

static wchar_t *translate(const wchar_t *input_text, TreeBlock *actual_tree, ConvertParams convertParams) {

    wchar_t *translated_text = calloc(100, sizeof(wchar_t*));
    // we need to set it 99 to leave space for \0
    size_t translated_text_size = 99;

    wchar_t *translated_char = NULL;

    wchar_t *char_to_translate = calloc(10, sizeof(wchar_t*));
    // we need to set it 9 to leave space for \0
    size_t char_to_translate_size = 9;

    size_t outputSeparatorLength = 0;
    if (convertParams.output_separator != L'\0')
        outputSeparatorLength = 1;

    int actual_char_index = 0;
    while (input_text[actual_char_index] != L'\0') {
        if (convertParams.input_separator == L'\0') {
            //in this case char_to_translate[1] always '\0', because of calloc
            char_to_translate[0] = input_text[actual_char_index];

        } else {
            // if we translate from morse, we should have an input char separator
            int pos = 0;
            while (input_text[actual_char_index] != convertParams.input_separator && input_text[actual_char_index] != L'\0') {
                // if the morse letter is bigger than the array size, increase the array
                if (char_to_translate_size == wcslen(char_to_translate)) {
                    char_to_translate = realloc(char_to_translate, sizeof(wchar_t*) * (char_to_translate_size + 10));
                    char_to_translate_size += 10;
                }
                char_to_translate[pos++] = input_text[actual_char_index];
                char_to_translate[pos] = L'\0';
                actual_char_index++;
            }
        }

        if (convertParams.input_separator == L'\0' || convertParams.input_separator == L' ' && input_text[actual_char_index] != L'\0') {
            actual_char_index++;
        }

        translated_char = convertChar(char_to_translate, actual_tree, &convertParams);

        if (wcslen(translated_text) + wcslen(translated_char) + outputSeparatorLength >= translated_text_size) {
            translated_text = realloc(translated_text, sizeof(wchar_t*) * (translated_text_size + 100));
            translated_text_size = translated_text_size + 100;
        }

        wcscat(translated_text, translated_char);

        if (convertParams.output_separator != L'\0') {
            size_t translated_text_length = wcslen(translated_text);
            translated_text[translated_text_length++] = convertParams.output_separator;
            translated_text[translated_text_size] = L'\0';
        }

        char_to_translate[0] = L'\0';
    }


    free(char_to_translate);

    return translated_text;
}

static wchar_t *readLine(wchar_t **text_in, const size_t *text_in_size) {

    wprintf(L"Kérem a konvertálandó szöveget!\n");

    bool eol = false;
    wchar_t ch;

    size_t pos = 0;
    while (!eol) {
        ch = fgetwc(stdin);
        if (ch == '\n') {
            eol = true;

            *(*text_in + pos) = L'\0';

        //In case of Ctrl + Z
        } else if (ch == WEOF) {

            free(*text_in);
            return NULL;
        } else {
            if (*text_in_size == wcslen(*text_in)) {
                *text_in = realloc(*text_in, sizeof(wchar_t*) * 100 + *text_in_size);
                text_in_size += 100;
            }
            *(*text_in + (pos++)) = ch;
            *(*text_in + pos) = L'\0';
        }
    }

    return *text_in;
}

void start_translation(int direction, TreeBlock *ekezetes, TreeBlock *morse) {

    wchar_t *text;
    wchar_t *text_in = calloc(100, sizeof(wchar_t*));
    size_t text_in_size = 99;

    //in the case of EOF, text = NULL
    while ((text = readLine(&text_in, &text_in_size)) != NULL) {

        //we need uppercase the input text, because in the tree we have capital characters
        wcsupr(text);

        wchar_t *translated_text = NULL;
        ConvertParams convert_params;

        switch (direction) {
            case FROM_MORZE:

                convert_params.fromIndex = 1;
                convert_params.toIndex = 0;
                convert_params.invalidChar = L'?';
                convert_params.input_separator = L' ';
                convert_params.output_separator = L'\0';

                translated_text = translate(text, morse, convert_params);
                wprintf(L"A konvertált szöveg: %ls\n", translated_text);
//                text[0] = '\0';

                free(translated_text);
                break;

            case TO_MORZE:

                convert_params.fromIndex = 0;
                convert_params.toIndex = 1;
                convert_params.invalidChar = L'\0';
                convert_params.input_separator = L'\0';
                convert_params.output_separator = L'\0';

                //first, we need to get the text without accents
                wchar_t *translated_without_accent = NULL;
                translated_without_accent = translate(text, ekezetes, convert_params);

                convert_params.invalidChar = L'?';
                convert_params.output_separator = L' ';

                translated_text = translate(translated_without_accent, morse, convert_params);
                wprintf(L"A konvertált szöveg: %ls\n", translated_text);
//                text[0] = '\0';

                free(translated_without_accent);
                free(translated_text);
                break;

            default:
                break;
        }
    }
    free(text);
}