#include "header.h"

void longueur(char *lvl, int *adrlignes, int *adrcolonnes){
    int lignes = 1;
    int colonnes = 0;
    FILE *fp = fopen(lvl, "r");

    char ch;
    while ((ch = fgetc(fp)) != EOF) {
        colonnes++;
        if (ch == '\n'){
            lignes++;
            colonnes=0;
        }
    }
    fclose(fp);

    *adrlignes = lignes;
    *adrcolonnes = colonnes;
}

void import(char *lvl){
    int lignes, colonnes, i, j;
    i=0;
    j=0;
    longueur(lvl, &lignes, &colonnes);
    char tab[lignes][colonnes];
    char *niveau = lvl;
    FILE *fp = fopen(niveau, "r");

    char ch;
    while ((ch = fgetc(fp)) != EOF) {
        if (ch!=10){
            tab[j][i]=ch;
            i++;
        }
        if (ch==10){
            i=0;
            j++;
        }
    }

    for (j=0 ; j<lignes ; j++){
        for (i=0 ; i<colonnes ; i++){
            printf("%c",tab[j][i]);
        }
        printf("\n");
    }
}

int main() {
    char *niveau = "niveaux/niveau1.txt";
    import(niveau);
    printf("%c",getch());
    getch();

/*
 *  char te = getchar();
    printf("%c",te);
    getch() pour pas besoin de touche entree
 */

    return 0;
}