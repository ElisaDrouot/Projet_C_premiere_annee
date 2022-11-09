#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <math.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h> 
#include <time.h>
#include <SDL2/SDL_mixer.h> //+Ajout de -lSDL2_mixer dans le Makefile

int main()
{
    /************** Initialisation de la SDL  + gestion de l'échec possible *********/
    /*                                                                              */
    /********************************************************************************/

    if(SDL_Init(SDL_INIT_VIDEO| SDL_INIT_AUDIO ) !=0 ){
        SDL_Log("Error : SDL initialisation - %s\n",
            SDL_GetError());    // l'initialisation de la SDL a échoué 
        exit(EXIT_FAILURE);
    }

    /************** Musique* *********/
    /*                                                                              */
    /********************************************************************************/

    /* Chargement musique */
    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);

    Mix_Music *Pantera = Mix_LoadMUS("lore1.mp3");//Chargement de la music
    Mix_PlayMusic(Pantera, 0);//Indice = priorité file
    //Mix_PauseMusic(); 
    //Mix_ResumeMusic();


    /*************************  Gestion des fenetres   *****************************/
    /*                                                                             */
    /*******************************************************************************/

    SDL_DisplayMode taille;
    SDL_GetCurrentDisplayMode(0, &taille);

    SDL_Window *window = NULL;
    SDL_Renderer *renderer = NULL;
    
    /* Création de la fenêtre premiere fenetre*/
    window  = SDL_CreateWindow(
        "Fenetre 1",
        0, 0,
        taille.w, taille.h,
        SDL_WINDOW_RESIZABLE);
    
    
    if (window == NULL){
        SDL_Log("Error : SDL window 1 creation -%s\n",
            SDL_GetError());
        SDL_Quit();                              // On referme la SDL       
        exit(EXIT_FAILURE);
    }

    /* Creation de render */

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED );

    if (renderer == 0) {
     fprintf(stderr, "Erreur d'initialisation de la SDL : %s\n", SDL_GetError()); 
      /*faire ce qu'il faut pour quitter proprement */
    }
    
    
    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
    SDL_RenderClear(renderer);
    //SDL_RenderPresent(renderer); 

    /*************************  Dessins *****************************/
    /*                                                              */
    /****************************************************************/
    

    SDL_Rect rect;
    
    SDL_Texture  *texture;
    SDL_Surface *image = NULL;
    image=IMG_Load("zzags1.png");

    texture = SDL_CreateTextureFromSurface(renderer, image);
    SDL_FreeSurface(image);

    rect.x = 600;
    rect.y = 110;
    rect.w = rect.h = 400;

    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, texture, NULL, &rect);
    
    SDL_RenderPresent(renderer);

    /*************************  Gestion des evenements *****************************/
    /*                                                                             */
    /*******************************************************************************/

    SDL_bool program_on = SDL_TRUE;               // Booléen pour dire que le programme doit continuer
    SDL_Event event;                              // c'est le type IMPORTANT !!

    int x; int y;

    while (program_on){                             // Voilà la boucle des évènements 

        if (SDL_PollEvent(&event)){                 // si la file d'évènements n'est pas vide : défiler l'élément en tête
                                                    // de file dans 'event'
            switch(event.type){                       // En fonction de la valeur du type de cet évènement
                case SDL_QUIT :                           // Un évènement simple, on a cliqué sur la x de la fenêtre
                    program_on = SDL_FALSE;                 // Il est temps d'arrêter le programme
                    break;

                default:                                  // L'évènement défilé ne nous intéresse pas
                    break;
                
                case SDL_MOUSEBUTTONDOWN:                     // Click souris   
                    if (SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(SDL_BUTTON_LEFT) ) {         // Si c'est un click gauche
                        SDL_GetMouseState(&x, &y);
                        SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
                        SDL_RenderClear(renderer);

                        /* aleatoire */
                        srand( time( NULL ) );
                        int p = rand() % 2; 

                        SDL_Texture  *texture;
                        SDL_Surface *image = NULL;

                        if(p == 0){
                            image=IMG_Load("zzags1.png");

                         }
                        else {
                            image=IMG_Load("zzags2.png");}

                            texture = SDL_CreateTextureFromSurface(renderer, image);
                            SDL_FreeSurface(image);

                            rect.x = x-50;
                            rect.y = y-36;
                            rect.w = rect.h = 400;

                        SDL_RenderCopy(renderer, texture, NULL, &rect);

                        SDL_RenderPresent(renderer);
                        SDL_Delay(60);}  
                    break;
            }
        }   
    }
    SDL_DestroyRenderer(renderer);  
    SDL_DestroyWindow(window);
    Mix_CloseAudio(); 
    SDL_Quit();   
    return 0;
}