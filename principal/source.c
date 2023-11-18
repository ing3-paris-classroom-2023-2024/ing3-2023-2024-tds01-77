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


void renvoi_sp(int bloc){
    switch (bloc){
        case 124:
            gotoligcol(15,5);
            printf("bird catched");
    }
}


int main() {
    unsigned encoding = GetConsoleOutputCP();
    SetConsoleOutputCP(encoding);

    int lignes, colonnes;
    char destination;
    char *niveau = "niveaux/niveau1.txt";
    int blocs[] = {169,207,245,124,196,219,178,254,174,175};

    longueur(niveau,&lignes, &colonnes);
    char tableau[lignes][colonnes];

    import(niveau, &lignes, &colonnes, tableau);
    int snoopyXY[2]={2,2};
    int move[2];
    move[0]=snoopyXY[0];
    move[1]=snoopyXY[1];

    char entree;

    gotoligcol(10,10);
    printf("x pour arreter");

    while (entree != 'x'){

        switch(entree){
            case 'z':
                move[0]+=(-1);
                break;
            case 'q':
                move[1]+=(-1);
                break;
            case 's':
                move[0]+=1;
                break;
            case 'd':
                move[1]+=1;
                break;
        }


        destination = tableau[move[0]][move[1]];
        for (int i=0 ; i<sizeof(blocs) ; i++){
            if (destination == blocs[i]){
                renvoi_sp(blocs[i]);
            }
        }

        snoopyXY[0]=move[0];
        snoopyXY[1]=move[1];

        gotoligcol(0,0);
        for (int i=0; i<lignes; i++){
            for (int j=0; j<colonnes; j++){
                if (i==snoopyXY[0] && j==snoopyXY[1]){
                    putchar(169);
                }
                else {
                    printf("%c",tableau[i][j]);
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