#include "header.h"

void gotoligcol( int lig, int col )   ///sous prog pour placer le curseur où on veut
{
    COORD mycoord;
    mycoord.X = col;
    mycoord.Y = lig;
    SetConsoleCursorPosition( GetStdHandle( STD_OUTPUT_HANDLE ), mycoord );
}


void longueur(char *lvl, int *adrlignes, int *adrcolonnes){     ///obtient le nb de colonnes et de lignes d'un fichier
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


void import(char *lvl, int *lignes, int *colonnes, char tableau[*lignes][*colonnes], int snoopyXY[2], int *oiseaux){   ///import du fichier et insertion des caractères dans un tableau
    int i, j;
    i=0;
    j=0;
    longueur(lvl, lignes, colonnes);

    char *niveau = lvl;
    FILE *fp = fopen(niveau, "r");

    char ch;
    snoopyXY[0] = fgetc(fp) - 64;
    snoopyXY[1] = fgetc(fp) - 64;
    *oiseaux = fgetc(fp) - 64;

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

void regles(){
    system("cls");

    FILE *f =fopen("niveaux/regles.txt","r");
    char ch;
    while ((ch = fgetc(f)) != EOF) {
        printf("%c",ch);
    }
    fclose(f);

    getch();
}

void menu(){
    system("cls");

    FILE *fmenu = fopen("niveaux/menu.txt","r");
    char ch;
    while ((ch = fgetc(fmenu)) != EOF) {
        printf("%c",ch);
    }
    fclose(fmenu);
}


void renvoi_sp(int bloc, int snoopyXY[2], int move[2], int *lignes, int *colonnes, char tableau[*lignes][*colonnes]){   ///exécute le script corresondant au bloc rencontré
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


void save(int *lignes, int *colonnes, char tableau1[*lignes][*colonnes], int snoopyXY1[2]){
    FILE *fichier = fopen("niveaux/sauvegarde.txt", "w");

    fputc(snoopyXY1[0]+64,fichier);
    fputc(snoopyXY1[1]+64,fichier);
    for (int i=0 ; i<*lignes ; i++){
        for (int j=0 ; j<*colonnes ; j++){
            fputc(tableau1[i][j],fichier);
        }
        if (i<*lignes-1){
            fputc('\n',fichier);
        }
    }
}


void jeu(int lvl){
    system("cls");

    char *ListeNiveaux[] = {"niveaux/sauvegarde.txt","niveaux/niveau1.txt", "niveaux/niveautest.txt"};
    int lignes, colonnes;
    char *niveau = ListeNiveaux[lvl];

    int destination;        ///code ascii du bloc rencontré
    ///liste des codes ascii des blocs
    int blocs[] = {169,207,245,124,196,219,178,254, 174,175,185,186,187,188,200,201,202,203,204,205,206,'f'};

    longueur(niveau,&lignes, &colonnes);    ///appel de longueur
    char tableau[lignes][colonnes];     ///définition du tableau en fonction des données fournies par longueur
    int snoopyXY[2]={1,1};      ///coordonnées de snoopy
    int move[2];                        ///coordonnées de snoopy après déplacement
    int oiseaux=0;


    import(niveau, &lignes, &colonnes, tableau, snoopyXY, &oiseaux);    ///place le fichier à ouvrir dans tableau

    move[0]=snoopyXY[0];
    move[1]=snoopyXY[1];


    bool rencontre;                     ///true si bloc rencontré au mouvement initialisé

    char entree;                        ///touche d'entrée utilisateur

    gotoligcol(19,5);
    printf("q pour arreter");

    while (entree != 'q'){
        gotoligcol(0,5);
        printf("%d oiseaux",oiseaux);

        switch(entree){                 ///vérifie si z,q,s ou d est entré par l'utilisateur
            case 72:
                move[0]+=(-1);
                break;
            case 75:
                move[1]+=(-1);
                break;
            case 80:
                move[0]+=1;
                break;
            case 77:
                move[1]+=1;
                break;
            case 's':
                save(&lignes, &colonnes, tableau,snoopyXY);
                break;

        }


        destination = tableau[move[0]][move[1]] + 256;      ///place dans destination le code ascii du prochain caractère rencontré
        rencontre = false;
        for (int i=0 ; blocs[i] != 'f' ; i++){
            if (destination == blocs[i]){   ///si un bloc spécial est rencontré
                rencontre = true;
                renvoi_sp(blocs[i], snoopyXY, move, &lignes, &colonnes, tableau);
                move[0]=snoopyXY[0];
                move[1]=snoopyXY[1];
            }
        }
        if (!rencontre){        ///si aucun bloc spécial n'est rencontré, snoopy bouge en prenant les coordonnées du mouvement demandé
            snoopyXY[0]=move[0];
            snoopyXY[1]=move[1];
        }


        gotoligcol(2,0);        ///imprime le tableau
        for (int i=0; i<lignes; i++){
            for (int j=0; j<colonnes; j++){
                if (i==snoopyXY[0] && j==snoopyXY[1]){      ///aux coordonnées de snoopy, imprime snoopy
                    putchar(169);
                }
                else {                                      ///sinon, imprime le bloc prévu dans le fichier (attention aux conflits)
                    putchar(tableau[i][j]);
                }
            }
            printf("\n");
        }

        printf("\n");
        entree = getch();           ///attends l'entrée utilisateur
    }
}



int main() {
    SetConsoleCP(CP_WINANSI);       ///définir l'encodage du projet
    SetConsoleOutputCP(CP_WINANSI); ///définir l'encodage de la console

///pour cacher le curseur
    HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO info;
    info.dwSize = 100;
    info.bVisible = FALSE;
    SetConsoleCursorInfo(consoleHandle, &info);
///pour cacher les curseur


    system("cls");


    menu();

    char input;
    while (input!='q'){
        input = getch();
        switch (input){
            case 'r':
                regles();
                menu();
                break;
            case 'n':
                jeu(1);
                menu();
                break;
            case 'c':
                jeu(0);
                menu();
                break;

        }
    }




    system("cls");


    printf("appuyez sur n'importe quoi pour fermer la console...");
    getch();
    return 0;
}