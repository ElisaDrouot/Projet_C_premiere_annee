#include "resolution.h"


int gagnant(int etat[2]){
    //0: perso gagne; 1:ennemi gagne; 2: meme attaque; 3 : au moins une defense
    int duel[4][4] = {{2,0,1,3},{1,2,0,3},{0,1,2,3},{3,3,3,2}};
    return duel[etat[0]][etat[1]];
}


void resolutionPV(int PV[], int resultat, int etat[]){
    int degat[3] = {2,3,1}; //degat des attaques eau, feu et terre
    if (resultat == 0){// si le perso a gagné
        PV[1] = PV[1] - degat[etat[0]];
    }
    if (resultat == 1){// si l'ennemi a gagné
        PV[0] = PV[0] - degat[etat[1]];
    }
    // si meme attaque ou si defense, pas de changemement de PV
}

void changementEtatEnnemi(int etat[])
{
    srand(time(NULL));
    int changementEtat[4][3] = {{20, 70, 90}, // tableaux des transitions de Markov
                                {20, 70, 90}, //chaque ligne correspond à un état
                                {20, 70, 90}, // chaque colonne correspond à la proba (sommée avec les probas des états des colonnes précédentes) d'un état à venir
                                {33, 66, 100}};
    int pourcentEtat = rand() % 100; //on détermine aléatoirement un pourcentage
    int i = 0;
    while ((changementEtat[etat[1]][i] < pourcentEtat) && (i < 3)) //on regarde à quelle transition correspond le pourcentage obtenu
        i++;
    etat[1] = i; //on change l'etat de l'ennemi
}

int defaite(int etat[2])
{
    // renvoie l'etat contre lequel gagne l'ennemi
    int etatperdant;
    printf("etatEnnemi:"
           "%d\n",
           etat[1]);
    switch (etat[1])
    {
    case EAU:
        etatperdant = FEU;
        break;
    case FEU:
        etatperdant = TERRE;
        break;
    case TERRE:
        etatperdant = EAU;
        break;
    case DEFENSE:
        etatperdant = DEFENSE;
        break;
    }
    return etatperdant;
}

void reussitedefense(int etat[2], int etatprec)
{
    srand(time(NULL));
    int pourcentEtat = rand() % 100;
    printf("%d\n", pourcentEtat);
    if (etat[0] == 3)
    { // si le perso tente une defense
        switch (etatprec)
        {
        case TERRE:
            if (pourcentEtat < 50)
                etat[0] = defaite(etat);
            break;
        case EAU:
            if (pourcentEtat < 40)
                etat[0] = defaite(etat);
            break;
        case FEU:
            if (pourcentEtat < 30)
                etat[0] = defaite(etat);
            break;
        case DEFENSE:
            etat[0] = defaite(etat);
        }
    }
}

void end_sdl(char ok,            // fin anormale : ok = 0 ; normale ok = 1
             char const *msg,    // message à afficher
             SDL_Window *window, // fenêtre à fermer
             SDL_Renderer *renderer)
{ // renderer à fermer
    char msg_formated[255];
    int l;

    if (!ok)
    { // Affichage de ce qui ne va pas
        strncpy(msg_formated, msg, 250);
        l = strlen(msg_formated);
        strcpy(msg_formated + l, " : %s\n");

        SDL_Log(msg_formated, SDL_GetError());
    }

    if (renderer != NULL)
    {                                  // Destruction si nécessaire du renderer
        SDL_DestroyRenderer(renderer); // Attention : on suppose que les NULL sont maintenus !!
        renderer = NULL;
    }
    if (window != NULL)
    {                              // Destruction si nécessaire de la fenêtre
        SDL_DestroyWindow(window); // Attention : on suppose que les NULL sont maintenus !!
        window = NULL;
    }

 SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit();
    SDL_Quit();
    IMG_Quit();

    if (!ok)
    { // On quitte si cela ne va pas
        exit(EXIT_FAILURE);
    }
}
