#include "header.h"
#include <windows.h>

///fonction pour le deplacement du curseur
void gotoligcol( int lig, int col ) {
    COORD mycoord;
    mycoord.X = col;
    mycoord.Y = lig;
    SetConsoleCursorPosition( GetStdHandle( STD_OUTPUT_HANDLE ), mycoord ); }


int main () {
    printf("blablaba\nBAHDUIADGYIAUDG\nBDADBADYUDAB");
    gotoligcol(2,5);
    printf("%c",2);
    return 0;
}
