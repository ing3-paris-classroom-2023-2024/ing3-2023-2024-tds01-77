#include "header.h"

void gotoligcol( int lig, int col )
{
    COORD mycoord;
    mycoord.X = col;
    mycoord.Y = lig;
    SetConsoleCursorPosition( GetStdHandle( STD_OUTPUT_HANDLE ), mycoord );
}


void longueur(char *lvl, int *adrlignes, int *adrcolonnes){
    int lignes = 1;
    int colonnes = 0;
    FILE *fp = fopen(lvl, "r");
    if (fp == NULL) {
        printf("Erreur lors de l'ouverture du fichier.");
    }

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


void import(char *lvl, int *lignes, int *colonnes, char tableau[*lignes][*colonnes]){
    gotoligcol(0,0);
    int i, j;
    i=0;
    j=0;
    longueur(lvl, lignes, colonnes);

    char tab[*lignes][*colonnes];
    char *niveau = lvl;
    FILE *fp = fopen(niveau, "r");

    char ch;
    while ((ch = fgetc(fp)) != EOF) {
        if (ch!=10){
            tab[j][i]=ch;
            tableau[j][i]=ch;
            i++;
        }
        if (ch==10){
            i=0;
            j++;
        }
    }

    fclose(fp);
}



void impression(){

}


int main() {
    int lignes, colonnes;
    char *niveau = "niveaux/niveau1.txt";
    longueur(niveau,&lignes, &colonnes);

    char tableau[lignes][colonnes];

    import(niveau, &lignes, &colonnes, tableau);

    for (int i=0; i<lignes; i++){
        for (int j=0; j<colonnes; j++){
            printf("%c",tableau[i][j]);
        }
        printf("\n");
    }
    printf("\n");


/*
 *  char te = getchar();
    printf("%c",te);
    getch() pour pas besoin de touche entree
    system("clear");
 */
    printf("appuyez sur n'importe quoi pour fermer la console...");
    getch();
    return 0;
}