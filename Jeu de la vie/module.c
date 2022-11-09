#include "module.h"

int survie[9] = {0, 0, 1, 1, 0, 0, 0, 0, 0};
int naissance[9] = {0, 0, 0, 1, 0, 0, 0, 0, 0};

void sauvegarder(int grille[HAUTEUR][LARGEUR])
{
    FILE *fichier = NULL;
    time_t now;
    time(&now);
    char nomfichier[100];
    strcpy(nomfichier, "sauvegarde");
    strcat(strcat(nomfichier, ctime(&now)), ".txt");
    fichier = fopen(nomfichier, "w");
    int i, j;
    for (i = 0; i < HAUTEUR; i++)
    {
        for (j = 0; j < LARGEUR; j++)
        {
            fprintf(fichier, "%d ", grille[i][j]);
        }
        fprintf(fichier, "\n");
    }
    fclose(fichier);
}

int loadConfig(int grille[HAUTEUR][LARGEUR], char nom_fic[100])
{
    int erreur = 1; // pas d'erreur

    FILE *fichier = NULL;
    fichier = fopen(nom_fic, "r");
    if (fichier == NULL)
        erreur = 0;
    char ligne[LARGEUR + 1];
    // int k;
    // for (k = 0; k < LARGEUR; k++)
    //     ligne[k] = '4';
    ligne[0] = '\0';
    // printf(ligne);

    if (fichier)
    {
        int i = 0;
        int j;
        printf("On commence ici\n");
        // while (!feof(fichier))
        // {
        //     fgets(ligne, LARGEUR+1, fichier);

        //     if(i<HAUTEUR)
        //     {
        //         for (j = 0; j < LARGEUR; j++)
        //         {
        //             if ((ligne[j] != '0') && (ligne[j] != '1'))
        //                 erreur = 0;
        //             grille[i][j] = ligne[j] - '0' ;
        //             printf("%c", ligne[j]);
        //         }
        //         printf("\n");
        //         i++;
        //     }
        // }

        while (!feof(fichier))
        {
            for (j = 0; j < LARGEUR; j++)
            {
                fscanf(fichier, "%d", &grille[i][j]);
                printf("%d ", grille[i][j]);
            }
            printf("\n");
            i++;
        }

        fclose(fichier);
    }

    return erreur;
}

void affichage(int grille[HAUTEUR][LARGEUR], SDL_Renderer *renderer)
{
    int i;
    int j;
    for (i = 0; i < HAUTEUR; i++)
    {
        for (j = 0; j < LARGEUR; j++)
        {

            SDL_Rect rect = {TAILLECELLULES * j, TAILLECELLULES * i, TAILLECELLULES - 2, TAILLECELLULES - 2};
            if (grille[i][j] == 0)
            {
                SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
                SDL_RenderFillRect(renderer, &rect);
            }
            else
            {
                SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
                SDL_RenderFillRect(renderer, &rect);
            }
        }
    }
    // SDL_RenderPresent(renderer);
}

int nb_voisins(int i, int j, int grille[HAUTEUR][LARGEUR], int monde)
{
    int voisins = 0;
    int horz[3] = {-1, 0, 1};
    int vert[3] = {-1, 0, 1};
    int maxvert = 3;
    int minvert = 0;
    int minhorz = 0;
    int maxhorz = 3;
    if (monde == 0) // normal
    {
        if (j == 0)
            minhorz = 1;
        if (j == LARGEUR - 1)
            maxhorz = 2;
        if (i == 0)
            minvert = 1;
        if (i == HAUTEUR - 1)
            maxvert = 2;
    }
    else // torique
    {
        if (j == 0)
            horz[0] = LARGEUR - 1 - j;
        if (j == LARGEUR - 1)
            horz[2] = 0 - j;
        if (i == 0)
            vert[0] = HAUTEUR - 1 - i;
        if (i == HAUTEUR - 1)
            vert[2] = 0 - i;
    }

    for (int b = minvert; b < maxvert; b++)
    {
        for (int a = minhorz; a < maxhorz; a++)
        {

            if (horz[a] != 0 || vert[b] != 0)
            {
                if (grille[vert[b] + i][horz[a] + j] == 1)
                    voisins++;
            }
        }
    }
    return voisins;
}

int nouvgen(int i, int j, int grille[HAUTEUR][LARGEUR], int nouvgrille[HAUTEUR][LARGEUR], int monde,int survie[9], int naissance[9])
{
    int voisins = nb_voisins(i, j, grille, monde);
    int modif = 0;
    int survie[9] = {0, 0, 1, 1, 0, 0, 0, 0, 0};
    int naissance[9] = {0, 0, 0, 1, 0, 0, 0, 0, 0};

    if (grille[i][j] == 1)
    {
        if (survie[voisins] == 0)
        {
            nouvgrille[i][j] = 0;
            modif++;
        }
        else
        {
            nouvgrille[i][j] = 1;
        }
    }
    else
    {
        if (naissance[voisins] == 1)
        {
            nouvgrille[i][j] = 1;
            modif++;
        }
        else
        {
            nouvgrille[i][j] = 0;
        }
    }
    return modif;
}

int nouvgengrille(int grille[HAUTEUR][LARGEUR], int nouvgrille[HAUTEUR][LARGEUR], int monde)
{
    int nb_modif = 0;
    for (int i = 0; i < HAUTEUR; i++)
    {
        for (int j = 0; j < LARGEUR; j++)
            nb_modif = nb_modif + nouvgen(i, j, grille, nouvgrille, monde,survie,naissance);
    }
    return nb_modif;
}

void initGrille(int grille[HAUTEUR][LARGEUR])
{
    for (int i = 0; i < HAUTEUR; i++)
    {
        for (int j = 0; j < LARGEUR; j++)
        {
            grille[i][j] = 0;
        }
    }
    /*
    grille[6][6] = 1;
    grille[6][5] = 1;
    grille[7][5] = 1;
    grille[8][5] = 1;
    grille[7][4] = 1;
    grille[2][0] = 1;
    grille[5][4] = 1;
    grille[5][3] = 1;
    grille[0][3] = 1;
    */
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

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

    SDL_Quit();

    if (!ok)
    { // On quitte si cela ne va pas
        exit(EXIT_FAILURE);
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int chargerConfigInit(int grille[HAUTEUR][LARGEUR], int argc, char nomFic[])
{

    return 0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void creationTexte(char texte[], SDL_Renderer *renderer, int taille, int x, int y, int opacite)
{

    TTF_Font *font = NULL;                   // la variable 'police de caractère'
    font = TTF_OpenFont("font.ttf", taille); // La police à charger, la taille désirée
    if (font == NULL)
        fprintf(stderr, "Can't load font  %s\n", SDL_GetError());

    TTF_SetFontStyle(font, TTF_STYLE_ITALIC | TTF_STYLE_BOLD); // en italique, gras

    SDL_Color color = {0, 0, 0, opacite};
    SDL_Surface *text_surface = NULL;                          // la surface  (uniquement transitoire)
    text_surface = TTF_RenderText_Blended(font, texte, color); // création du texte dans la surface
    if (text_surface == NULL)
        fprintf(stderr, "Can't create text surface %s\n", SDL_GetError());

    SDL_Texture *text_texture = NULL;                                    // la texture qui contient le texte
    text_texture = SDL_CreateTextureFromSurface(renderer, text_surface); // transfert de la surface à la texture
    if (text_texture == NULL)
        fprintf(stderr, "Can't create texture from surface %s\n", SDL_GetError());
    SDL_FreeSurface(text_surface); // la texture ne sert plus à rien

    SDL_Rect pos = {x, y, 1000, 1000};                          // rectangle où le texte va être prositionné
    SDL_QueryTexture(text_texture, NULL, NULL, &pos.w, &pos.h); // récupération de la taille (w, h) du texte
    SDL_RenderCopy(renderer, text_texture, NULL, &pos);
    SDL_DestroyTexture(text_texture);
}
