#include <wchar.h>
#include <string.h>

#include "paramCheck.h"
#include "debugmalloc.h"

int paramCheck(int argc, char *argv[]) {
    if (argc == 1) {                                           //started the program without command-line argument
        wprintf(L"A program használatához az alábbi kapcsolók egyikének használata szükséges:"
                " \n/frommorze | /tomorze");

        return HIBA;

    } else if (argc > 2) {                                     //started the program with too many command-line argument
        wprintf(L"Túl sok kapcsolót adott meg, a program használatához az alábbi kapcsolók egyikének használata "
                "szükséges: \n/frommorze | /tomorze");
        return HIBA;

    } else if (strcmp(strlwr(argv[1]), "/frommorze") == 0) {    //started the program with the /frommorze argument
        return FROM_MORZE;

    } else if (strcmp(strlwr(argv[1]), "/tomorze") == 0) {      //started the program with the /tomorze argument
        return TO_MORZE;

    } else {                                                   //started the program with a wrong command-line argument
        wprintf(L"A program használatához az alábbi kapcsolók egyikének használata szükséges:"
                " \n/frommorze | /tomorze");
        return HIBA;
    }
}