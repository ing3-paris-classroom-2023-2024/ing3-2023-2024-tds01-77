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
    unsigned encoding = GetConsoleOutputCP();
    SetConsoleOutputCP(encoding);

    int lignes, colonnes;
    int snoopy[2]={1,1};
    int move[2] = {1,1};
    char *niveau = "niveaux/niveau1.txt";
    char blocs[] = {};
    longueur(niveau,&lignes, &colonnes);

    char tableau[lignes][colonnes];

    import(niveau, &lignes, &colonnes, tableau);

    char entree;

    while (entree != 'x'){

        switch(entree){
            case 'z':
                move[1]+=1;
                break;
            case 'q':
                move[0]+=(-1);
                break;
            case 's':
                move[1]+=(-1);
                break;
            case 'd':
                move[0]+=1;
                break;
        }

        gotoligcol(0,0);
        for (int i=0; i<lignes; i++){
            for (int j=0; j<colonnes; j++){
                printf("%c",tableau[i][j]);
                if (i==snoopy[0] && j==snoopy[1]){
                    putchar(169);
                }
            }
            printf("\n");
        }

        printf("\n");
        entree = getch();
    }


    printf("appuyez sur n'importe quoi pour fermer la console...");
    getch();
    return 0;
}