#include <locale.h>
#include <windows.h>

#include "debugmalloc.h"
#include "paramCheck.h"
#include "treeHandling.h"
#include "translateText.h"

/*TODO
 * megmagyaraznii a kiirast https://www.cprogramming.com/tutorial/unicode.html
 * */

int main(int argc, char *argv[]) {
    //to read/write in UTF-8 mode
    char *locale = setlocale(LC_ALL, "");
    SetConsoleCP(1252);
    SetConsoleOutputCP(1252);

    TreeBlock *morse_tree = NULL;
    TreeBlock *ekezetes_tree = NULL;

    const int direction = paramCheck(argc, argv);
    switch (direction) {
        case FROM_MORZE:

            morse_tree = treeBuild("morse.dic", direction);
            break;

        case TO_MORZE:

            morse_tree = treeBuild("morse.dic", direction);
            ekezetes_tree = treeBuild("ekezetes.dic", direction);
            break;

        //if direction == HIBA
        default:
            return 0;
    }


    start_translation(direction, ekezetes_tree, morse_tree);

    if (ekezetes_tree != NULL)
        freeTree(ekezetes_tree);
    if (morse_tree != NULL)
        freeTree(morse_tree);

    return 0;
}
