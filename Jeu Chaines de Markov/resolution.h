#ifndef RESOLUTION_H
#define RESOLUTION_H

#include "general.h"

int gagnant(int []);
void resolutionPV(int [], int , int []);
void changementEtatEnnemi(int []);
int defaite(int[]);
void reussitedefense(int [], int );
void end_sdl(char ok, char const *msg, SDL_Window *window, SDL_Renderer *renderer);

#endif