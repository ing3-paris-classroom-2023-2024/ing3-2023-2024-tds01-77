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

    char *niveau = lvl;
    FILE *fp = fopen(niveau, "r");

    char ch;
    while ((ch = fgetc(fp)) != EOF) {
        if (ch!=10){
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


void renvoi_sp(int bloc, int snoopyXY[2], int move[2], int *lignes, int *colonnes, char tableau[*lignes][*colonnes]){
    switch (bloc){
        case 245:    ///oiseau
            gotoligcol(20,5);
            snoopyXY[0] = move[0];
            snoopyXY[1] = move[1];
            tableau[move[0]][move[1]]=' ';
            printf("bird catched");
            break;

        case 175:    ///teleportation
            for (int i=0 ; i<*lignes ; i++){
                for (int j=0 ; j<*colonnes ; j++){
                    if (tableau[i][j] == -82){
                        snoopyXY[0]=i;
                        snoopyXY[1]=j-1;
                    }
                }
            }
            break;

        case 174:    ///teleportation
            for (int k=0 ; k<*lignes ; k++){
                for (int l=0 ; l<*colonnes ; l++){
                    if (tableau[k][l] == -81){
                        snoopyXY[0]=k;
                        snoopyXY[1]=l+1;
                    }
                }
            }
            break;
    }
}


int main() {
    SetConsoleCP(CP_WINANSI);
    SetConsoleOutputCP(CP_WINANSI);

///pour cacher le curseur
    HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO info;
    info.dwSize = 100;
    info.bVisible = FALSE;
    SetConsoleCursorInfo(consoleHandle, &info);


    int lignes, colonnes;
    int destination;
    char *niveau = "niveaux/niveau1.txt";
    int blocs[] = {169,207,245,124,196,219,178,254, 174,175,185,186,187,188,200,201,202,203,204,205,206,'f'};

    longueur(niveau,&lignes, &colonnes);
    char tableau[lignes][colonnes];

    import(niveau, &lignes, &colonnes, tableau);

    int snoopyXY[2]={1,1};
    int move[2];
    move[0]=snoopyXY[0];
    move[1]=snoopyXY[1];
    bool rencontre;

    char entree;

    gotoligcol(15,5);
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


        destination = tableau[move[0]][move[1]] + 256;
        rencontre = false;
        for (int i=0 ; blocs[i] != 'f' ; i++){
            if (destination == blocs[i]){
                rencontre = true;
                renvoi_sp(blocs[i], snoopyXY, move, &lignes, &colonnes, tableau);
                move[0]=snoopyXY[0];
                move[1]=snoopyXY[1];
            }
        }
        if (!rencontre){
            snoopyXY[0]=move[0];
            snoopyXY[1]=move[1];
        }


        gotoligcol(0,0);
        for (int i=0; i<lignes; i++){
            for (int j=0; j<colonnes; j++){
                if (i==snoopyXY[0] && j==snoopyXY[1]){
                    putchar(169);
                }
                else {
                    putchar(tableau[i][j]);
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