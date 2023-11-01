#include "header.h"

int main(){
    char *niveau = "niveaux/niveau1.txt";
    FILE *fp = fopen(niveau, "r");
    char ch;
    while ((ch = fgetc(fp)) != EOF)
        putchar(ch);

    // close the file
    fclose(fp);
    return 0;
}