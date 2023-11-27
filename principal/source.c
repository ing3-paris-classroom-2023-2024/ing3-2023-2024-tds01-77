#include "header.h"
///sites consultés:https://learn.microsoft.com ; https://koor.fr/C/cstdbool/cstdbool.wp
///Cours: ALGORITHMIQUE ET PROGRAMMATION EN LANGAGE C ~ Cours – Exercices – Mini projets 2022-2023 ING3 Nouveaux Frédéric Ravaut Enseignant – Chercheur



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


void import(char *lvlname, int *lignes, int *colonnes, char tableau[*lignes][*colonnes], int snoopyXY[2], int oiseaux[2], int *lvl, int *vies, double *pause){   ///import du fichier et insertion des caractères dans un tableau
    int i, j;
    i=0;
    j=0;
    longueur(lvlname, lignes, colonnes);
    int l = *lignes;
    int c = *colonnes;
    char *niveau = lvlname;
    FILE *fp = fopen(niveau, "r");


    /*
     * initialise la lecture du fichier, les 5 premiers termes servent à savoir respectivement
     * 1.le numéro du niveau
     * 2.le nb de vies restantes
     * 3 et 4. les coordonnées de départ de snoopy
     * 4 et 5. le nombre actuel et total d'oiseaux
     * 6. le temps restant
     */
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
    int nb = fgetc(fp) - 65;
    *pause = nb - 120;


    while ((ch = fgetc(fp)) != EOF) {    ///met dans le tableau chaque caractère du fichier
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


void regles()        ///affichage des règles
{
    system("cls");

    FILE *f =fopen("niveaux/regles.txt","r");
    char ch;
    while ((ch = fgetc(f)) != EOF) {
        printf("%c",ch);
    }
    fclose(f);

    getch();
}


void menu()         ///affichage du menu
{
    system("cls");

    FILE *fmenu = fopen("niveaux/menu.txt","r");
    char ch;
    while ((ch = fgetc(fmenu)) != EOF) {
        printf("%c",ch);
    }
    fclose(fmenu);
}


void lose()         ///affichage du game over
{
    system("cls");
    gotoligcol(0,0);
    FILE *perdu = fopen ("niveaux/gameover.txt", "r");
    char ch;
    while ((ch = fgetc(perdu)) != EOF) {
        printf("%c",ch);
    }
    getch();
}


void renvoi_sp(int bloc, int snoopyXY[2], int move[2], int *lignes, int *colonnes, char tableau[*lignes][*colonnes], int oiseaux[2], int *vies) ///exécute le script corresondant au bloc rencontré
{
    int sens[2] = {move[0]-snoopyXY[0], move[1]-snoopyXY[1]};
    switch (bloc){
        case 245:    ///oiseau
            snoopyXY[0] = move[0];      ///change les coordonnées de snoopy pour qu'ils prennent celles du déplacement demandé
            snoopyXY[1] = move[1];
            tableau[move[0]][move[1]]=' ';  ///remplace le caractère de l'oiseau par un espace dans le niveau
            oiseaux[0]+=1;                  ///ajoute 1 au compteur du score
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


void save(char *lvlname, int *lignes, int *colonnes, char tableau1[*lignes][*colonnes], int snoopyXY1[2], int oiseaux[2], int lvl, int vies, double temps)   ///sauvegarde du fichier
{
    FILE *fichier = fopen("niveaux/sauvegarde.txt", "w");
    longueur(lvlname,lignes, colonnes);


    ///lecture des premieres informations
    fputc(lvl+65,fichier);              ///le niveau
    fputc(vies+65,fichier);             ///le nb de vies
    fputc(snoopyXY1[0]+65,fichier);     ///les coordonnées de snoopy
    fputc(snoopyXY1[1]+65,fichier);
    fputc(oiseaux[0]+65,fichier);       ///les oiseaux restants
    fputc(oiseaux[1]+65,fichier);
    fputc(temps+65, fichier);           ///le temps restant

    ///recopie le tableau dans le fichier
    for (int i=0 ; i<*lignes ; i++){
        for (int j=0 ; j<*colonnes ; j++){
            fputc(tableau1[i][j],fichier);
        }
        if (i<*lignes-1){
            fputc('\n',fichier);
        }
    }
}


void ecrire_score(char* nom, int score)     ///sous programme de sauvegarde du score
{
    FILE* fichier = fopen("niveaux/scores.txt", "a");                    /// Ouvre le fichier en mode append
    if (fichier != NULL) {
        fprintf(fichier, "%s a eu un score de %d\n", nom, score); /// Écrit la ligne dans le fichier
        fclose(fichier);                                                   /// Ferme le fichier
    } else {
        printf("Erreur lors de l'ouverture du fichier.\n");              /// Affiche une erreur si le fichier n'a pas pu être ouvert
    }
}


void jeu(int lvl)       ///sous rogramme principal
{
    system("cls");

    char *ListeNiveaux[] = {"niveaux/sauvegarde.txt","niveaux/niveau1.txt", "niveaux/niveau2.txt", "niveaux/niveau3.txt", "niveaux/niveau4.txt", "niveaux/niveau5.txt", "niveaux/niveau10.txt", 0};
    int nb_niveaux = sizeof(ListeNiveaux)/sizeof(ListeNiveaux[0]) - 1;
    int lignes, colonnes;
    char *niveau = ListeNiveaux[lvl];
    char *listeCodes[] = {"aaa","aab","acb","ddb","bbd","bba"};

    int destination;        ///code ascii du bloc rencontré
    ///liste des codes ascii des blocs
    int blocs[] = {169,207,245,179,196,219,178,254, 174,175,185,186,187,188,200,201,202,203,204,205,206,'f'};

    longueur(niveau,&lignes, &colonnes);    ///appel de longueur
    char tableau[lignes][colonnes];     ///définition du tableau en fonction des données fournies par longueur
    int snoopyXY[2]={1,1};      ///coordonnées de snoopy
    int move[2];                        ///coordonnées de snoopy après déplacement
    int oiseaux[2]={0,0};
    int balleXY[2]={1,3};       ///coordonnes de base de la balle
    int balleTraj[2]={1,1};     ///direction de la balle (à changer si elle rencontre un mur)
    int tics=0;                         ///compteur d'entrées dans la boucle
    int vies = 3;
    double temps = 120;
    double pause = 0;                   ///temps de la pause, qu'on soustrait au temps total pour éviter que le temps passe pendant la pause
    int score = 0;
    time_t Tdebut, Tactuel, tampon1, tampon2;
    time(&Tdebut);


    import(niveau, &lignes, &colonnes, tableau, snoopyXY, oiseaux, &lvl, &vies, &pause);    ///place le fichier à ouvrir dans tableau

    move[0]=snoopyXY[0];
    move[1]=snoopyXY[1];

    bool rencontre;                     ///true si bloc rencontré au mouvement initialisé

    char entree;                        ///touche d'entrée utilisateur

    gotoligcol(20,0);
    printf("code : %s\nq pour arreter\np pour pause\ns pour sauvegarder",listeCodes[lvl-1]);

    while (entree != 'q'){          ///si q est enfoncé, sort de la boucle
        gotoligcol(0,2);
        printf("niveau %d sur %d,  %d oiseaux sur %d,  %d vies restantes\n  temps restant : %.0f     ",lvl-1, nb_niveaux-2, oiseaux[0], oiseaux[1], vies, temps);

        if (vies==0 || temps <= 0){
            lose();
            break;
        }

        if (tics==10-lvl){       ///tous les n passages dans la boucle, la balle bouge
            tics=0;
            int nextBloc = tableau[balleXY[0]+balleTraj[0]][balleXY[1]+balleTraj[1]] + 256;
            if (nextBloc == 205){   ///si un mur est rencontré, en fonction du mur, on change la trajectoire
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


        if(oiseaux[0]==oiseaux[1]){         ///si tous les oiseaux sont trouvés
            system("cls");
            printf("BRAVOOOOO, vous avez eu tous les oiseaux\n");
            lvl++;
            score += temps*100;
            if (lvl >= nb_niveaux){
                FILE *fini =fopen("niveaux/fini.txt","r");
                char ch;
                while ((ch = fgetc(fini)) != EOF) {
                    printf("%c",ch);
                }
                fclose(fini);
                printf("\n\nvous avez fini le jeu avec un score de %d \nentrez votre nom pour sauvegarder le score :",score);
                char pseudo[100];
                scanf("%s",pseudo);
                ecrire_score(pseudo,score);

                return;
            }
            else {
                printf("appuyez sur n'importe quelle touche pour passer au niveau suivant...\n");
            }

            if (lvl < nb_niveaux){      ///s'il reste des niveaux, passe au niveau suivant en changeant de tableau
                getch();
                getch();
                system("cls");

                import(ListeNiveaux[lvl], &lignes, &colonnes, tableau, snoopyXY, oiseaux, &lvl, &vies, &pause);
                move[0]=snoopyXY[0];
                move[1]=snoopyXY[1];
            }
            time(&Tdebut);
        }

        switch(entree){                 ///vérifie si une fleche ou un caractère est entré par l'utilisateur
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
                save(niveau, &lignes, &colonnes, tableau, snoopyXY, oiseaux, lvl, vies, temps);
                break;
            case 'p':
                time(&tampon1);
                getch();
                time(&tampon2);
                pause += difftime(tampon2,tampon1);
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
        if (_kbhit()) {             ///si l'utilisateur a appuyé sur une touche, enrengistre cette touche, cela evite de stopper le programme avec getch()
            entree = getch();
        }
        else {
            entree = 0;
        }
        Sleep(10);
        time(&Tactuel);
        temps = 120 - difftime(Tactuel, Tdebut) + pause;
        tics++;
    }
}


void showscore()        ///affichage du score
{
    system("cls");
    FILE *fscore = fopen("niveaux/scores.txt","r");
    char ch;
    while ((ch = fgetc(fscore)) != EOF) {
        printf("%c",ch);
    }
    fclose(fscore);
    printf("\nappuyez sur n'import quelle touche pour retourner au menu");
    getch();
}


void code()     ///sous programme des codes
{
    char entree[10];
    system("cls");
    printf("entrez le code :");
    scanf("%s",entree);
    if (strcmp(entree,"aaa")==0){
        jeu(1);
        return;
    }
    if (strcmp(entree,"aab")==0){
        jeu(2);
        return;
    }
    if (strcmp(entree,"acb")==0){
        jeu(3);
        return;
    }
    if (strcmp(entree,"ddb")==0) {
        jeu(4);
        return;
    }
    if (strcmp(entree,"bbd")==0){
        jeu(5);
        return;
    }
    if (strcmp(entree,"bba")==0){
        jeu(6);
        return;
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
            case('s'):
                showscore();
                menu();
                break;

        }
    }
    system("cls");


    printf("appuyez sur n'importe quoi pour fermer la console...");
    getch();
    return 0;
}
