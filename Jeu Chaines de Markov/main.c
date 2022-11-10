#include "resolution.h"
#include "affichage.h"
#include "affichageAttaque.h"
#include "general.h"


int main(int argc, char *argv[])
{

    SDL_Window *window = NULL;
    SDL_Renderer *renderer = NULL;

    SDL_DisplayMode screen;
    SDL_Rect tailleFenetre;

    /************** Initialisation de la SDL  + gestion de l'échec possible *********/
    /*                                                                              */
    /********************************************************************************/

    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        SDL_Log("Error : SDL initialisation - %s\n",
                SDL_GetError()); // l'initialisation de la SDL a échoué
        exit(EXIT_FAILURE);
    }

    /***************************** Texte ********************************************/
    /*                                                                              */
    /********************************************************************************/

    /*****Initialisation*****/

    if (TTF_Init() < 0)
     fprintf(stderr, "Unable to initialize TTF:  %s\n", SDL_GetError());

    /*************************  Gestion des fenetres   *****************************/
    /*                                                                             */
    /*******************************************************************************/

    SDL_GetCurrentDisplayMode(0, &screen);
    tailleFenetre.w = screen.w * coefReducEcran; //réduction fenetre
    tailleFenetre.h = screen.h * coefReducEcran;
    printf("Résolution écran\n\tw : %d\n\th : %d\n",
           screen.w, screen.h);
    printf("Résolution fenetre\n\tw : %d\n\th : %d\n",
           tailleFenetre.w, tailleFenetre.h);

    /* Création de la fenêtre */
    window = SDL_CreateWindow("Jeu de l'ambiance",
                              SDL_WINDOWPOS_CENTERED,
                              SDL_WINDOWPOS_CENTERED,
                              tailleFenetre.w, tailleFenetre.h,
                              SDL_WINDOW_OPENGL);
    if (window == NULL)
        end_sdl(0, "ERROR WINDOW CREATION", window, renderer);

    /* Création du renderer */
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    if (renderer == NULL)
        end_sdl(0, "ERROR RENDERER CREATION", window, renderer);

   

/////////////// récupération des textures et stockage dans tableau
   SDL_Texture *texture[ligneTexture][colonneTexture];
    char nomFichiers[ligneTexture][colonneTexture][tailleMaxFichiers] = {{"./Sprites/attaqueeau.png", "./Sprites/attaquefeu.png", "./Sprites/attaquepierre.png", "./Sprites/defense.png","./Sprites/attente.png","./Sprites/persobase.png","./Sprites/victoire.png","./Sprites/defaite.png","./Sprites/fond1.jpg","./Sprites/winner.png"},
 {"./Sprites/attaqueeauennemi.png", "./Sprites/attaquefeuennemi.png", "./Sprites/attaquepierreennemi.png", "./Sprites/defenseennemi.png","./Sprites/attenteennemi.png","./Sprites/ennemibase.png","./Sprites/victoireennemi.png","./Sprites/defaiteennemi.png","./Sprites/fond2.jpg","./Sprites/loser.png"}};
    for (int i = 0; i < ligneTexture; i++)
    {
        for (int j = 0; j < colonneTexture; j++)
        {
            texture[i][j] = IMG_LoadTexture(renderer, nomFichiers[i][j]);
            if (texture[i][j] == NULL)
                end_sdl(0, "Echec du chargement de l'image dans la texture", window, renderer);
        }
    }
//////////////////////////


    SDL_bool program_on = SDL_TRUE; // Booléen pour dire que le programme doit continuer
    SDL_Event event;                // c'est le type IMPORTANT !!

    SDL_Texture *sprite1;
    SDL_Texture *sprite2;
    SDL_Texture *textureFond;

    int nbrImageSprite1;
    int nbrImageSprite2;
    int animationFinieTerminee = 1;
    int gameOver = 0;

    int ETATJEU = ACCUEIL;
    int i = 0;
    int j=0;

    int PV[2] = {10, 10};
    int etat[2] = {EAU, EAU};

    while (program_on)
    {
        // Voilà la boucle des évènements
        int interessant = 0;
        int choixFait = 0;
        int etatPrec; //sert à l'utilisation de la chaîne de Markov

    /*************************  Gestion des evenements *****************************/
    /*                                                                             */
    /*******************************************************************************/

        while ((interessant == 0) && (SDL_PollEvent(&event)))
        {   // tant que la file d'évènements n'est pas vide : défiler l'élément en tête et l'on a pas d'évènements interessants à traiter
            // de file dans 'event'
            switch (event.type)
            { // En fonction de la valeur du type de cet évènement
            case SDL_QUIT:
                interessant = 1;        // Un évènement simple, on a cliqué sur la x de la fenêtre
                program_on = SDL_FALSE; // Il est temps d'arrêter le programme
                break;
            case SDL_KEYUP: // Le type de event est : une touche lachée

                switch (event.key.keysym.sym)
                { // la touche appuyée est ...
                case SDLK_SPACE:
                    if (ETATJEU == ACCUEIL)
                    {
                        ETATJEU = JEU;
                        i=0;
                        j=0;
                        animationFinieTerminee = 1;
                        gameOver = 0;
                        PV[0] = 10;
                        PV[1] = 10;
                        etat[0] = EAU;
                        etat[1] = EAU;
                    }
                    interessant = 1;
                    break;
                case SDLK_q:
                    if(animationFinieTerminee ==1){
                        if (ETATJEU == JEU)
                        {
                            ETATJEU = ACCUEIL;
                        }
                        interessant = 1;
                    }
                    break;
                case SDLK_e:
                    etat[0] = 0;
                    choixFait = 1;
                    interessant = 1;
                    break;
                case SDLK_f:
                    etat[0] = 1;
                    choixFait = 1;
                    interessant = 1;
                    break;
                case SDLK_t:
                    etat[0] = 2;
                    choixFait = 1;
                    interessant = 1;
                    break;
                case SDLK_d:
                    etatPrec = etat[0];
                    etat[0] = 3;
                    choixFait = 1;
                    interessant = 1;
                    break;
                default:
                    break;
                }

            default: // L'évènement défilé ne nous intéresse pas
                break;
            }
        }
        interessant = 0;

        switch (ETATJEU)
        {
        case ERREUR:
            break;
        case ACCUEIL:

            SDL_RenderClear(renderer);
            creationImage(texture[0][8],window, renderer, tailleFenetre.w, tailleFenetre.h, 0, 0); //on affiche le fond inanimé

            creationTexte("Bienvenue :", "normal", "./fonts/font.ttf", renderer, 100, 600, 100, 255);
            creationTexte("Commandes :", "surligne", "./fonts/font2.ttf", renderer, 90, 400, 300, 255);
            creationTexte("E pour attaque eau", "normal", "./fonts/font2.ttf", renderer, 90, 500, 450, 255);
            creationTexte("F pour attaque feu", "normal", "./fonts/font2.ttf", renderer, 90, 500, 550, 255);
            creationTexte("T pour attaque terre", "normal", "./fonts/font2.ttf", renderer, 90, 500, 650, 255);
            creationTexte("D pour defense", "normal", "./fonts/font2.ttf", renderer, 90, 500, 750, 255);
            creationTexte("Q pour revenir ici", "normal", "./fonts/font2.ttf", renderer, 90, 500, 850, 255);

            break;
        case JEU:

            if(choixFait == 1 && animationFinieTerminee == 1 && gameOver == 0){ // quand l'utilisateur a appuyé sur une touche, qu'il n'y a pas d'animation finie en cours et que le jeu n'est pas fini, on traite le jeu en fonction de la touche pressée
                changementEtatEnnemi(etat);
                reussitedefense(etat, etatPrec);
                int resultat = gagnant(etat);
                resolutionPV(PV, resultat, etat);
                sprite1 = textureAttaque(texture,0, etat, resultat);
                sprite2 = textureAttaque(texture,1, etat, resultat);

                choixFait = 0;
                animationFinieTerminee = 0;
                i=0;
            }

            if(i >= nbrImageSpriteFini && animationFinieTerminee == 0){ // quand une animation finie a affiché chacunes de ses 10 images, on l'arrete
                animationFinieTerminee = 1;
                i=0;
            }

            if(animationFinieTerminee == 1){ // quand une animation finie est terminée, on revient à l'état de base : les deux persos qui donnent des coups de poing dans le vide (animation infinie)
                sprite1 = texture[0][5];
                sprite2 = texture[1][5];

            }

            textureFond = texture[1][8]; //par défaut 
            int nbrImageFond = 1; // possibilité de fond animé 

            if(((PV[0] <= 0)||(PV[1] <= 0))&&(animationFinieTerminee == 1 )){ // si un des deux joueurs est mort et qu'il n'y a pas d'animation finie en cours
                if(PV[1] <=0 ){
                sprite1 = texture[0][6];
                sprite2 = texture[1][7];
                textureFond = texture[0][9];
            }
            if(PV[0] <=0 ){
                sprite1 = texture[0][7];
                sprite2 = texture[1][6];
                textureFond = texture[1][9];
            }
                gameOver = 1;
            }

            for(int i=0;i<ligneTexture;i++){ // En quelque sorte un "tableau du nombre d'image en fonction du sprite" (on a pas eu le temps de faire ça "proprement" avec un vrai tableau).
                for(int j=0;j<colonneTexture;j++){
                    if(sprite1 == texture[i][j]){
                        if(j==0 || j==1 || j==2 || j==3 || j==4){
                            nbrImageSprite1=10;
                        }else if(j==5 || j==6){
                            nbrImageSprite1=2;
                        }else if(j == 7){
                            nbrImageSprite1=3;
                        }else if(j == 8){
                            if(i==0){
                                nbrImageSprite1 = 1;
                            }else if(i==1){
                                    nbrImageSprite1 =1;
                            }
                        }
                    }
                    if(sprite2 == texture[i][j]){
                        if(j==0 || j==1 || j==2 || j==3 || j==4){
                            nbrImageSprite2=10;
                        }else if(j==5 || j==6){
                            nbrImageSprite2=2;
                        }else if(j == 7){
                            nbrImageSprite2=3;
                        }else if(j == 8){
                            if(i==0){
                                nbrImageSprite2 = 1;
                            }else if(i==1){
                                    nbrImageSprite2 =1;
                            }
                        }
                    }
                    if(textureFond == texture[i][j]){
                        if(j==0 || j==1 || j==2 || j==3 || j==4){
                            nbrImageFond=10;
                        }else if(j==5 || j==6){
                            nbrImageFond=2;
                        }else if(j == 7){
                            nbrImageFond=3;
                        }else if(j == 8){
                            if(i==0){
                                nbrImageFond = 1;
                            }else if(i==1){
                                    nbrImageFond =1;
                            }
                        }
                    }
               }
            }

            Animation(sprite1, sprite2, textureFond, renderer, window, 400, 400, 200, 1300, 500, nbrImageSprite1,nbrImageSprite2, nbrImageFond, i, j);

            i++;
            j++;
          
            char message[50];
            char message2[50];
            sprintf(message,"PV : %d",PV[0]);
            sprintf(message2,"PV : %d",PV[1]);
            creationTexte(message, "normal", "./fonts/font.ttf", renderer, 100, 10, 350, 255);
            creationTexte(message2, "normal", "./fonts/font.ttf", renderer, 100, 1450, 350, 255);

            break;
        default:
            ETATJEU = ERREUR;
            break;
        }

        SDL_RenderPresent(renderer);
        SDL_Delay(150);
    }

   end_sdl(1, "FIN NORMALE", window, renderer);
    return 0;
}