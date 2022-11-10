#include "affichageAttaque.h"

SDL_Texture * textureAttaque(SDL_Texture *texture[ligneTexture][colonneTexture], int joueur, int etat[2], int resultat)
{
    SDL_Texture * textureperso;
    switch (resultat)
    {
    case 0: // victoire perso
        if (joueur == 0)
            textureperso = texture[0][etat[0]];
        else
            textureperso = texture[1][4];
        break;
    case 1: // victoire ennemi
        if (joueur == 0)
            textureperso = texture[0][4];
        else
            textureperso = texture[1][etat[1]];
        break;
    default: // ex aequo et défense
        if (joueur == 0)
            textureperso = texture[0][etat[0]];
        else
            textureperso = texture[1][etat[1]];
    }
    return textureperso;
}
