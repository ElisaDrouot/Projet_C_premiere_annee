#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h> 

void creationImage(char nom[], SDL_Renderer *renderer, int longueur, int largeur, int x, int y){

    SDL_Texture  *texture;
    SDL_Surface *image = NULL;
    SDL_Rect rect;

    image=IMG_Load(nom);

    texture = SDL_CreateTextureFromSurface(renderer, image);
    SDL_FreeSurface(image);

    rect.x = x;
    rect.y = y;
    rect.w = longueur;
    rect.h = largeur;

    SDL_RenderCopy(renderer, texture, NULL, &rect);
}

void Animation(char nom[], char fond[], SDL_Renderer *renderer, SDL_Window *window, int longueur, int largeur, int x, int y,int nbimage, int ite)
{
    SDL_Texture  *texture;
    SDL_Surface *image = NULL;

    SDL_Rect
        source = {0},                             // Rectangle définissant la zone de la texture à récupérer
        window_dimensions = {0},                  // Rectangle définissant la fenêtre, on  n'utilisera que largeur et hauteur
        destination = {0};                        // Rectangle définissant où la zone_source doit être déposée dans le renderer


    image=IMG_Load(nom);

    texture = SDL_CreateTextureFromSurface(renderer, image);
    SDL_FreeSurface(image);

    SDL_Rect state[nbimage];                         // Tableau qui stocke les vignettes dans le bon ordre pour l'animation

    SDL_QueryTexture(texture, NULL, NULL, &source.w, &source.h); 
    int offset_x = source.w / nbimage,                // La largeur d'une vignette de l'image
        offset_y = source.h;

    /* construction des différents rectangles autour de chacune des vignettes de la planche */
    int i = 0;                                   
    for (int y = 0; y < source.h ; y += offset_y) {
    for (int x = 0; x < source.w; x += offset_x) {
      state[i].x = x;
      state[i].y = y;
      state[i].w = offset_x;
      state[i].h = offset_y;
      ++i;
    }
    }

    for(; i< nbimage ; ++i){                  // reprise du début de l'animation en sens inverse  
        state[i] = state[nbimage-1-i];
    }
    destination.w = largeur;           // Largeur du sprite à l'écran
    destination.h = longueur;            // Hauteur du sprite à l'écran
    destination.x = x + i*50; // Position en x pour l'affichage du sprite
    destination.x += 50*ite;
    destination.y = y + i*50;  // Position en y pour l'affichage du sprite

    ite = ite % nbimage;   
    creationImage(fond, renderer, 1920, 1080, 0, 0);
    SDL_RenderCopy(renderer, texture, &state[ite], &destination);

}

int main()
{
    /************** Initialisation de la SDL  + gestion de l'échec possible *********/
    /*                                                                              */
    /********************************************************************************/

    if(SDL_Init(SDL_INIT_VIDEO) !=0 ){
        SDL_Log("Error : SDL initialisation - %s\n",
            SDL_GetError());    // l'initialisation de la SDL a échoué 
        exit(EXIT_FAILURE);
    }

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

    /* Creation de fond*/
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED );

    if (renderer == 0) {
     fprintf(stderr, "Erreur d'initialisation de la SDL : %s\n", SDL_GetError()); 
      /*faire ce qu'il faut pour quitter proprement */
    }

    creationImage("fond.jpg", renderer, 1920, 1080, 0, 0);
    SDL_RenderClear(renderer);
    


    /*************************  Gestion des evenements *****************************/
    /*                                                                             */
    /*******************************************************************************/

    SDL_bool program_on = SDL_TRUE;               // Booléen pour dire que le programme doit continuer
    SDL_Event event;                              // c'est le type IMPORTANT !!

    int i = 0; int interessant = 0;

    while (program_on){ 
                                 // Voilà la boucle des évènements 
        while ((interessant == 0 )&&(SDL_PollEvent(&event))){                 // si la file d'évènements n'est pas vide : défiler l'élément en tête
                                                    // de file dans 'event'
                switch(event.type){                       // En fonction de la valeur du type de cet évènement
                case SDL_QUIT :
                printf("quit\n");                           // Un évènement simple, on a cliqué sur la x de la fenêtre
                    program_on = SDL_FALSE;   
                    interessant = 1;             // Il est temps d'arrêter le programme
                    break;
                default:
                    break;
            }   
        }

        Animation("pacman.png", "fond.jpg", renderer, window, 400, 400, 0, 300, 3, i);
        i++;
        SDL_RenderPresent(renderer);
        interessant = 0;
        SDL_Delay(100);
    }
    SDL_DestroyRenderer(renderer);  
    SDL_DestroyWindow(window); 
    SDL_Quit();   
    return 0;
}