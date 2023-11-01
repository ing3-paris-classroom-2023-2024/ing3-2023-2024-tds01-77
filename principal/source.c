#include "header.h"

int main() {
    char *x[3];
    char y[6];
    int i,j;
    i = 0;
    j = 0;

    char *niveau = "niveaux/niveau1.txt";
    FILE *fp = fopen(niveau, "r");

    char ch;
    while ((ch = fgetc(fp)) != EOF) {
        if (ch!=10){
            y[i]=ch;
            i++;
            printf("%s",y);
        }
        if (ch=10){
            i=0;
            x[j]=y;
        }
    }
    printf("%s",y);

    fclose(fp);
    return 0;
}