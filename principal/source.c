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


void import(char *lvlname, int *lignes, int *colonnes, char tableau[*lignes][*colonnes], int snoopyXY[2], int oiseaux[2], int *lvl, int *vies){   ///import du fichier et insertion des caractères dans un tableau
    int i, j;
    i=0;
    j=0;
    longueur(lvlname, lignes, colonnes);

    char *niveau = lvlname;
    FILE *fp = fopen(niveau, "r");

    char ch;
    *lvl = fgetc(fp) - 65;
    int lives = fgetc(fp)-65;
    if (lives < *vies){
        *vies = lives;
    }
    snoopyXY[0] = fgetc(fp) - 65;
    snoopyXY[1] = fgetc(fp) - 65;
    oiseaux[0] = fgetc(fp) - 65;
    oiseaux[1] = fgetc(fp) - 65;

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


void lose(){
    system("cls");
    gotoligcol(0,0);
    FILE *perdu = fopen ("niveaux/gameover.txt", "r");
    char ch;
    while ((ch = fgetc(perdu)) != EOF) {
        printf("%c",ch);
    }
    getch();
}


void renvoi_sp(int bloc, int snoopyXY[2], int move[2], int *lignes, int *colonnes, char tableau[*lignes][*colonnes], int oiseaux[2], int *vies){   ///exécute le script corresondant au bloc rencontré
    int sens[2] = {move[0]-snoopyXY[0], move[1]-snoopyXY[1]};
    switch (bloc){
        case 245:    ///oiseau
            gotoligcol(20,5);   ///place le curseur aux coordonnées données (20,5)
            snoopyXY[0] = move[0];      ///change les coordonnées de snoopy pour qu'ils prennent celles du déplacement demandé
            snoopyXY[1] = move[1];
            tableau[move[0]][move[1]]=' ';  ///remplace le caractère de l'oiseau par un espace dans le niveau
            oiseaux[0]+=1;                  ///ajoute 1 au compteur du score
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

        case 179:
            if (tableau[move[0] + sens[0]][move[1]] == 32){
                tableau[move[0] + sens[0]][move[1]] = 179;
                tableau[move[0]][move[1]] = ' ';
            }
            break;

        case 196:
            if (tableau[move[0]][move[1] + sens[1]] == 32){
                tableau[move[0]][move[1] + sens[1]] = 196;
                tableau[move[0]][move[1]] = ' ';
            }
            break;

        case 254:
            if (tableau[move[0] + sens[0]][move[1] + sens[1]] == 32){
                tableau[move[0] + sens[0]][move[1] + sens[1]] = 254;
                tableau[move[0]][move[1]] = ' ';
            }
            break;

        case 178:
            tableau[move[0]][move[1]] = ' ';
            *vies += -1;
            snoopyXY[0] = move[0];      ///change les coordonnées de snoopy pour qu'ils prennent celles du déplacement demandé
            snoopyXY[1] = move[1];
    }
}


void save(int *lignes, int *colonnes, char tableau1[*lignes][*colonnes], int snoopyXY1[2], int oiseaux[2], int lvl, int vies){
    FILE *fichier = fopen("niveaux/sauvegarde.txt", "w");

    fputc(lvl+65,fichier);
    fputc(vies+65,fichier);
    fputc(snoopyXY1[0]+65,fichier);
    fputc(snoopyXY1[1]+65,fichier);
    fputc(oiseaux[0]+65,fichier);
    fputc(oiseaux[1]+65,fichier);
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

    char *ListeNiveaux[] = {"niveaux/sauvegarde.txt","niveaux/niveau1.txt", "niveaux/niveau3.txt", "niveaux/niveautest.txt"};
    int nb_niveaux = sizeof(ListeNiveaux)/sizeof(ListeNiveaux[0]);
    int lignes, colonnes;
    char *niveau = ListeNiveaux[lvl];

    int destination;        ///code ascii du bloc rencontré
    ///liste des codes ascii des blocs
    int blocs[] = {169,207,245,179,196,219,178,254, 174,175,185,186,187,188,200,201,202,203,204,205,206,'f'};

    longueur(niveau,&lignes, &colonnes);    ///appel de longueur
    char tableau[lignes][colonnes];     ///définition du tableau en fonction des données fournies par longueur
    int snoopyXY[2]={1,1};      ///coordonnées de snoopy
    int move[2];                        ///coordonnées de snoopy après déplacement
    int oiseaux[2]={0,0};
    int balleXY[2]={1,3};
    int balleTraj[2]={1,1};
    int tics=0;
    int vies = 3;
    double temps = 1;
    time_t Tdebut, Tactuel;
    time(&Tdebut);


    import(niveau, &lignes, &colonnes, tableau, snoopyXY, oiseaux, &lvl, &vies);    ///place le fichier à ouvrir dans tableau

    move[0]=snoopyXY[0];
    move[1]=snoopyXY[1];

    bool rencontre;                     ///true si bloc rencontré au mouvement initialisé

    char entree;                        ///touche d'entrée utilisateur

    gotoligcol(19,5);
    printf("q pour arreter");

    while (entree != 'q'){
        gotoligcol(0,2);
        printf("niveau %d sur %d,  %d oiseaux sur %d,  %d vies restantes\n  temps restant : %.0f",lvl, nb_niveaux, oiseaux[0], oiseaux[1], vies, temps);

        if (vies==0){
            lose();
            break;
        }

        if (tics==5){
            tics=0;
            int nextBloc = tableau[balleXY[0]+balleTraj[0]][balleXY[1]+balleTraj[1]] + 256;
            if (nextBloc == 205){
                balleTraj[0] *= -1;
            }
            if (nextBloc == 186){
                balleTraj[1] *= -1;
            }
            if (nextBloc == 187 || nextBloc == 188 || nextBloc == 200 || nextBloc == 201){
                balleTraj[0] *= -1;
                balleTraj[1] *= -1;
            }
            if (balleXY[0]+balleTraj[0] == snoopyXY[0] && balleXY[1]+balleTraj[1] == snoopyXY[1]) {
                vies += -1;
            }
            balleXY[0]+=balleTraj[0];
            balleXY[1]+=balleTraj[1];
        }


        if(oiseaux[0]==oiseaux[1]){
            system("cls");
            printf("BRAVOOOOO, vous avez eu tous les oiseaux");
            lvl++;
            if (lvl >= nb_niveaux){
                printf("\n\n\nvous avez fini le jeu!!!");
                getch();
                getch();
                return;
            }

            if (lvl < nb_niveaux){
                getch();
                getch();
                system("cls");

                import(ListeNiveaux[lvl], &lignes, &colonnes, tableau, snoopyXY, oiseaux, &lvl, &vies);
                move[0]=snoopyXY[0];
                move[1]=snoopyXY[1];
            }
        }

        switch(entree){                 ///vérifie si z,q,s ou d est entré par l'utilisateur
            time_t tampon;
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
                save(&lignes, &colonnes, tableau, snoopyXY, oiseaux, lvl, vies);
                break;
            case 'p':
                tampon = Tactuel;
                getch();
                getch();
                temps = tampon;
        }


        destination = tableau[move[0]][move[1]] + 256;      ///place dans destination le code ascii du prochain caractère rencontré
        rencontre = false;
        for (int i=0 ; blocs[i] != 'f' ; i++){
            if (destination == blocs[i]){   ///si un bloc spécial est rencontré
                rencontre = true;
                renvoi_sp(blocs[i], snoopyXY, move, &lignes, &colonnes, tableau, oiseaux, &vies);
                move[0]=snoopyXY[0];
                move[1]=snoopyXY[1];
            }
        }
        if (!rencontre){        ///si aucun bloc spécial n'est rencontré, snoopy bouge en prenant les coordonnées du mouvement demandé
            snoopyXY[0]=move[0];
            snoopyXY[1]=move[1];
        }


        gotoligcol(3,0);        ///imprime le tableau
        for (int i=0; i<lignes; i++){
            for (int j=0; j<colonnes; j++){
                if (i==snoopyXY[0] && j==snoopyXY[1]){      ///aux coordonnées de snoopy, imprime snoopy
                    putchar(169);
                }
                else if (i==balleXY[0] && j==balleXY[1]){
                    putchar(207);
                }
                else {                                      ///sinon, imprime le bloc prévu dans le fichier (attention aux conflits)
                    putchar(tableau[i][j]);
                }
            }
            printf("\n");
        }

        printf("\n");
        if (_kbhit()) {
            entree = getch();
        }
        else {
            entree = 0;
        }
        Sleep(10);
        time(&Tactuel);
        temps = 120-difftime(Tactuel, Tdebut);
        tics++;
                 ///attends l'entrée utilisateur
    }
}


void code(){
    char entree[10];
    system("cls");
    printf("entrez le code :");
    scanf("%s",entree);
    if (strcmp(entree,"aab")==0){
        jeu(1);
    }
    printf("\n\ncode invalide, appuyez sur une touche pour retourner au menu");
    getch();
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

    int liste[2] = {0,1};
    system("cls");


    FILE *ff = fopen("la.txt","w");
    fputc(179,ff);
    fclose(ff);


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
            case('m'):
                code();
                menu();
                break;

        }
    }




    system("cls");


    printf("appuyez sur n'importe quoi pour fermer la console...");
    getch();
    return 0;
}