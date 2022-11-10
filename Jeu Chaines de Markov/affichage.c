#include "affichage.h"

void creationTexte(char texte[], char style[], char police[], SDL_Renderer *renderer, int taille, int x, int y, int opacite)
{

    TTF_Font *font = NULL;                   // la variable 'police de caractère'
    font = TTF_OpenFont(police, taille); // La police à charger, la taille désirée
    if (font == NULL)
        fprintf(stderr, "Can't load font  %s\n", SDL_GetError());
    
    if(strcmp(style,"normal") == 0){ //si on veut le texte en style normal
    TTF_SetFontStyle(font, TTF_STYLE_NORMAL); }// en italique, gras TTF_STYLE_ITALIC | TTF_STYLE_BOLD

    if(strcmp(style,"surligne") == 0){ //si on veut le texte surligné
    TTF_SetFontStyle(font, TTF_STYLE_UNDERLINE); }

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

void creationImage(SDL_Texture *my_texture,SDL_Window *window, SDL_Renderer *renderer, int longueur, int largeur, int x, int y){

    SDL_Rect
        source = {0},           // Rectangle définissant la zone de la texture à récupérer
        window_dimensions = {0}, // Rectangle définissant la fenêtre, on n'utilisera que largeur et hauteur
        destination = {0};       // Rectangle définissant où la zone_source doit être déposée dans le renderer

    SDL_GetWindowSize(window, &window_dimensions.w, &window_dimensions.h); // Récupération des dimensions de la fenêtre
    SDL_QueryTexture(my_texture, NULL, NULL, &source.w, &source.h);
    
    destination.x = x;
    destination.y = y;
    destination.w = longueur;
    destination.h = largeur;

    SDL_RenderCopy(renderer, my_texture , &source, &destination);

}


void Animation(SDL_Texture * texture1, SDL_Texture *texture2, SDL_Texture *fond, SDL_Renderer *renderer, SDL_Window *window, int longueur, int largeur, int x1,  int x2, int y,int nbimage1,int nbimage2, int nbimageFond, int ite, int iteFond)
{
    SDL_Rect
        source1 = {0},                             // Rectangle définissant la zone de la texture à récupérer
       // window_dimensions1 = {0},                  // Rectangle définissant la fenêtre, on  n'utilisera que largeur et hauteur
        destination1 = {0};                        // Rectangle définissant où la zone_source doit être déposée dans le renderer

//    SDL_Texture  *texture2;
  //  SDL_Surface *image2 = NULL;

    SDL_Rect
        source2 = {0},                             // Rectangle définissant la zone de la texture à récupérer
       // window_dimensions2 = {0},                  // Rectangle définissant la fenêtre, on  n'utilisera que largeur et hauteur
        destination2 = {0};

SDL_Rect
        sourceFond = {0},                             // Rectangle définissant la zone de la texture à récupérer
       // window_dimensions1 = {0},                  // Rectangle définissant la fenêtre, on  n'utilisera que largeur et hauteur
        destinationFond = {0};


    SDL_Rect state1[nbimage1];                         // Tableau qui stocke les vignettes dans le bon ordre pour l'animation
    SDL_Rect state2[nbimage2];
    SDL_Rect stateFond[nbimageFond]; 

    SDL_QueryTexture(texture1, NULL, NULL, &source1.w, &source1.h); 
    int offset_x1 = source1.w / nbimage1,                // La largeur d'une vignette de l'image
        offset_y1 = source1.h;

    /* construction des différents rectangles autour de chacune des vignettes de la planche */
    int i1 = 0;                                   
    for (int y1 = 0; y1 < source1.h ; y1 += offset_y1) {
    for (int x1 = 0; x1 < source1.w; x1 += offset_x1) {
      state1[i1].x = x1;
      state1[i1].y = y1;
      state1[i1].w = offset_x1;
      state1[i1].h = offset_y1;
      ++i1;
    } 
    }

    SDL_QueryTexture(texture2, NULL, NULL, &source2.w, &source2.h); 
    int offset_x2 = source2.w / nbimage2,                // La largeur d'une vignette de l'image
        offset_y2 = source2.h;

    /* construction des différents rectangles autour de chacune des vignettes de la planche */
    int i2 = 0;                                   
    for (int y2 = 0; y2 < source2.h ; y2 += offset_y2) {
    for (int x2 = 0; x2 < source2.w; x2 += offset_x2) {
      state2[i2].x = x2;
      state2[i2].y = y2;
      state2[i2].w = offset_x2;
      state2[i2].h = offset_y2;
      ++i2;
    } 
    }

     SDL_QueryTexture(fond, NULL, NULL, &sourceFond.w, &sourceFond.h); 
    int offset_xFond = sourceFond.w / nbimageFond,                // La largeur d'une vignette de l'image
        offset_yFond = sourceFond.h;

    /* construction des différents rectangles autour de chacune des vignettes de la planche */
    int iFond = 0;                                   
    for (int yFond = 0; yFond < sourceFond.h ; yFond += offset_yFond) {
    for (int xFond = 0; xFond < sourceFond.w; xFond += offset_xFond) {
      stateFond[iFond].x = xFond;
      stateFond[iFond].y = yFond;
      stateFond[iFond].w = offset_xFond;
      stateFond[iFond].h = offset_yFond;
      ++iFond;
    } 
    }


    destination1.w = largeur;           // Largeur du sprite à l'écran
    destination1.h = longueur;            // Hauteur du sprite à l'écran
    destination1.x = x1; // Position en x pour l'affichage du sprite
    destination1.y = y;  // Position en y pour l'affichage du sprite

    if(offset_x1 >= 150){
        destination1.w = 750;
        destination1.h += 50;
        destination1.x += 50;
        destination1.y -= 50;
    }

    destination2.w = largeur;           // Largeur du sprite à l'écran
    destination2.h = longueur;            // Hauteur du sprite à l'écran
    destination2.x = x2; // Position en x pour l'affichage du sprite
    destination2.y = y;  // Position en y pour l'affichage du sprite

    if(offset_x2 >= 150){
        destination2.w = 750;
        destination2.h += 50;
        destination2.x -= 400;
        destination2.y -= 50;
    }

    destinationFond.w = 1920;           // Largeur du sprite à l'écran
    destinationFond.h = 1080;            // Hauteur du sprite à l'écran
    destinationFond.x = 0; // Position en x pour l'affichage du sprite
    destinationFond.y = 0;  // Position en y pour l'affichage du sprite



    int courant1 = ite % nbimage1;
    int courant2 = ite % nbimage2;
    int courantFond = iteFond % nbimageFond;

    SDL_RenderCopy(renderer, fond, &stateFond[courantFond], &destinationFond);
    SDL_RenderCopy(renderer, texture1, &state1[courant1], &destination1);
    SDL_RenderCopy(renderer, texture2, &state2[courant2], &destination2);

}
