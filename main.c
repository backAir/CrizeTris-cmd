// #include <stdio.h>
// #include <stdlib.h>
#include "External.h"
#include "Game.h"

#ifdef TEST
#include "test.h"
int main(int argc, char *argv[]) {
    test();
    return 0;
}

#else

int main(int argc, char *argv[]) {
    // int i = time(NULL);
    // free(bag);
    Setup(70,40);
    Play();
    getch();
    // srand(0);
    return 0;
}

#endif



