#include <locale.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <wchar.h>


void ascii(char** l1, char** l2, char** l3, char** l4, char** l5) {
    srand(time(0));

    int num = 1 + rand() % 3;
    if (num == 1) {
        *l1 = "  ／l、     ";
        *l2 = " ﾟ､ ｡ ７    ";
        *l3 = " l  ~ ヽ    ";
        *l4 = " じしf_,)ノ ";
        *l5 = "            ";
    } else if (num == 2) {
        *l1="        ";
        *l2=" {\\_/}  ";
        *l3=" (●ᴗ●)  ";
        *l4=" ( >🥕  ";
        *l5="        ";
    } else {
        *l1 = " \\    /\\ ";
        *l2 = "  )  ( ')";
        *l3 = " (  /  ) ";
        *l4 = "  \\(__)| ";
        *l5 = "         ";
    }
}


