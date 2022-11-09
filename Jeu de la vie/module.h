#ifndef MODULE_H
#define MODULE_H
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <math.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h> 
#include <time.h>

#define ERREUR 10
#define MONDE 20
#define CONFIG 30
#define ITER 40
#define PAUSE 50
#define STABLE 60
#define FIN 70

#define HAUTEUR 30
#define LARGEUR 30
#define TAILLECELLULES 20

#define TAILLE_MAX 50

#define CoinToriqueYHaut 540
#define CoinToriqueYBas 630
#define CoinToriqueXGauche 670 
#define CoinToriqueXDroit 1360

#define CoinNormalYHaut 710
#define CoinNormalYBas 800
#define CoinNormalXGauche 720 
#define CoinNormalXDroit 1320


void sauvegarder(int grille[HAUTEUR][LARGEUR]);
int loadConfig(int grille[HAUTEUR][LARGEUR], char nom_fic[100]);

void end_sdl(char, char const *, SDL_Window *, SDL_Renderer *);
int chargerConfigInit(int[HAUTEUR][LARGEUR], int, char *);
void creationTexte(char texte[], SDL_Renderer *renderer, int taille, int x, int y, int opacite);
void initGrille(int grille[HAUTEUR][LARGEUR]);
int nouvgengrille(int grille[HAUTEUR][LARGEUR], int nouvgrille[HAUTEUR][LARGEUR], int );
int nouvgen(int, int, int grille[HAUTEUR][LARGEUR], int nouvgrille[HAUTEUR][LARGEUR], int , int[9], in[9]);
int nb_voisins(int , int , int grille[HAUTEUR][LARGEUR], int );
void affichage(int grille[HAUTEUR][LARGEUR], SDL_Renderer *);

#endif