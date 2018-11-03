#include <stdlib.h>
#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
//#include "deplacement.h"
#include "sdl2.h"
 //#include "SDL2/SDL_Image.h"
//#include <SDL/SDL_image.h>

#define SIZE_DAMIER 10
#define SIZE_CARREAU 80

#define VIDE 0
#define PION 1
#define REINE 2
#define MULTIPLICATEUR_JOUEUR 10


/*
typedef struct Damier {
  int pion;
  SDL_Rect cases;
}Damier;*/
// varaible global
int NEW_TEXTURE = 0;





// void afficheConnection (SDL_Renderer * renderer) {
//   puts("a");
//   int h1 = 0, w1 = 0, h2 = 200, w2 =200;
//   char str[1000];
//   int ch, n = 0;
//   SDL_Color color = { 255, 255, 255 };
//   TTF_Font * police;
//   puts("a");
//   police = TTF_OpenFont("../fonts/arial.ttf", 25);
//   if(police == NULL) {
//     printf("%s\n","police null" );
//   }
//   puts("a");
//   SDL_Surface * surface = TTF_RenderText_Solid(police,"hello",color);
//   SDL_Texture * texture = SDL_CreateTextureFromSurface(renderer,surface);
//   SDL_QueryTexture(texture,NULL, NULL,&w2,&h2);
//   SDL_Rect rect1 = {w2 ,h2, 100, 100 };
//   SDL_Surface * surface2;
//   SDL_Texture * texture2;
//   //SDL_QueryTexture(texture,NULL, NULL,&w2,&h2);
//   SDL_Rect rect2 = { 130, 100, 130, 100 };
//   SDL_RenderCopy(renderer,texture,NULL,&rect1);
//   while (1) {
//   while ((ch = getchar()) != EOF && n < 1000 ) {
//     printf("%s\n","dans la boucle" );
//     printf("%c\n",ch );
//     str[n] = ch;
//     ++n;
//     surface2 = TTF_RenderText_Solid(police,str,color);
//     texture2 = SDL_CreateTextureFromSurface(renderer,surface2);
//     SDL_RenderCopy(renderer,texture2,NULL,&rect2);
//     SDL_RenderPresent(renderer);
//   }
// //
// // }
//
//
//
//   //SDL_RenderCopy(renderer,texture2,NULL,&rect2);
//   puts("a");
//   SDL_RenderPresent(renderer);
//   puts("a");
//   //message = TTF_RenderText_Solid( police, "Test pour sdl_ttf", textColor );
//   //apply_surface( 0, 200, message, screen );
//   SDL_Delay(10000);
// }


void affichermatrice (Damier  damier[10][10]) {
  puts("affiche damier");
  for (int  i = 0; i < 10; i++) {
    for (int j = 0; j < 10; j++) {
      printf("( %d / %d, %d ) | ",damier[i][j].pion, damier[i][j].cases.y, damier[i][j].cases.x );
    }
    puts("");
  }
}
/*************************************************************************/
/********** initialise les pions a la position de depart *****************/
/** Prend en parametre un matrice de type struct Damier ******************/
/*************************************************************************/
void init_game (Damier damier[10][10]) {
  for (int  i = 0; i < SIZE_DAMIER; i++) {
    for (int j = 0; j < SIZE_DAMIER; j++) {
      if((i+j)%2 == 0) {
        if(i < 4){

          damier[i][j].pion = 2 * MULTIPLICATEUR_JOUEUR + PION;
        }
        else if (i > 5){

          damier[i][j].pion = 1 * MULTIPLICATEUR_JOUEUR + REINE;

        }
        else {
          damier[i][j].pion = VIDE;
        }
      }
      else {

        damier[i][j].pion = VIDE;
      }
    }
  }

}
/*************************************************************************/
/********** initialise la position de chaque case de la matrice **********/
/** Prend en parametre une matrice de type struct Damier ******************/
/*************************************************************************/
void place_tile (Damier damier[10][10]) {
  damier[0][0].cases.x = damier[0][0].cases.y = 0;
  damier[0][0].cases.w = damier[0][0].cases.h = SIZE_CARREAU;

  for (int  i = 0; i < SIZE_DAMIER; i++) {
    for (int j = 0; j < SIZE_DAMIER; j++) {
      if(j==0 && i != 0){
           //  printf("(%d, %d)\n",i,j );
             damier[i][0].cases.y = damier[i-1][0].cases.y + 80;
             damier[i][0].cases.x = 0;

      }
      if(j != 0 ) {
           damier[i][j].cases.y = damier[i][j-1].cases.y ;
           damier[i][j].cases.x = damier[i][j-1].cases.x + 80;

      }

          damier[i][j].cases.w = 80;
          damier[i][j].cases.h = 80;
    }
    puts("");
  }

}
/*************************************************************************/
/************** Alloue une matrice de type struct Damier *****************/
/*************************************************************************/
Damier ** allocDamier () {
  Damier ** damier;
  damier = malloc(10 * sizeof(damier));
  if(damier == NULL) {
    perror("damier : malloc()");
    exit(1);
  }
  for (int  i = 0; i < 10; i++) {
    damier[i] = malloc(10 * sizeof(*damier));
    if(damier[i] == NULL) {
      perror("damier : malloc()");
      exit(1);
    }
  }
  puts("end allocDamier()");
  return damier;
}

/*********************** Affiche une image  ******************************/
/** Prend en parametre, un renderer, le chemin de l'image et ça position */
/*************************************************************************/
SDL_Texture ** create_texture (SDL_Renderer *renderer, SDL_Texture ** arrayTexture) {
  arrayTexture = malloc(4 * sizeof(arrayTexture));

    SDL_Surface* picturePW = IMG_Load("../picture/pionBlanc.png");
    SDL_Surface* picturePB = IMG_Load("../picture/pionNoir.png");
    SDL_Surface* pictureLW = IMG_Load("../picture/queenW.png");
    SDL_Surface* pictureLB = IMG_Load("../picture/quennB.png");

    arrayTexture[TEXTURE_PW] = SDL_CreateTextureFromSurface(renderer,picturePW);
    arrayTexture[TEXTURE_PB] = SDL_CreateTextureFromSurface(renderer,picturePB);
    arrayTexture[TEXTURE_LW] = SDL_CreateTextureFromSurface(renderer,pictureLW);
    arrayTexture[TEXTURE_LB] = SDL_CreateTextureFromSurface(renderer,pictureLB);

    return arrayTexture;
  }
void print_picture (SDL_Renderer *renderer,SDL_Texture ** arrayTexture, int enumTexture, SDL_Rect cases) {
  SDL_QueryTexture(arrayTexture[enumTexture], NULL, NULL, &cases.w, &cases.h);
  SDL_RenderCopy(renderer, arrayTexture[enumTexture],NULL,&cases);
  //SDL_DestroyTexture(myPicture);
}
/**************************************************************/
/************** Affiche un carreau noir ***********************/
/**************************************************************/
void print_black_tile (SDL_Renderer * renderer, SDL_Rect * rect) {
  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // on associe la couleur Noir
  SDL_RenderFillRect(renderer, rect);
}
/**************************************************************/
/************** Affiche un carreau blanc ***********************/
/**************************************************************/
void print_white_tile (SDL_Renderer * renderer, SDL_Rect * rect) {
  SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // on associe la couleur Noir
  SDL_RenderFillRect(renderer, rect);
}
/**************************************************************/
/************** Affiche un carreau bleu ***********************/
/**************************************************************/
void print_blue_tile (SDL_Renderer * renderer, SDL_Rect * rect) {
  SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255); // on associe la couleur Noir
  SDL_RenderFillRect(renderer, rect);
}
/**************************************************************/
/********************* Affiche le damier **********************/
/**************************************************************/
void print_damier (SDL_Renderer *renderer,SDL_Texture ** arrayTexture, Damier  damier[10][10]) {
  for (int  i = 0; i < SIZE_DAMIER; i++) {
    for (int j = 0; j < SIZE_DAMIER; j++) {
      SDL_Rect * cs = &damier[i][j].cases;
      switch (damier[i][j].pion) {
        case VIDE :
          if((i+j)%2 == 0 ) {
            print_black_tile(renderer,cs);
          }
          else {
            print_white_tile(renderer,cs);
          }
          break;
        case (1 * MULTIPLICATEUR_JOUEUR + PION) :
          print_picture(renderer,arrayTexture, TEXTURE_PW,damier[i][j].cases);
          break;
        case (2 * MULTIPLICATEUR_JOUEUR + PION) :
          print_picture(renderer,arrayTexture,TEXTURE_PB,damier[i][j].cases);
          break;
        case (1 * MULTIPLICATEUR_JOUEUR + REINE) :
          print_picture(renderer,arrayTexture,TEXTURE_LW,damier[i][j].cases);
          break;
        case (2 * MULTIPLICATEUR_JOUEUR + REINE) :
          print_picture(renderer,arrayTexture,TEXTURE_LB,damier[i][j].cases);
          break;
        default :
          print_blue_tile(renderer,cs);
          break;
      }
    }
  }
}

/**************************************************************/

// int main(int argc, char** argv)
// {
//   SDL_Renderer *renderer;
//   SDL_Window * window;
//     window = init_view ();
//    play_view (window);
// }

 SDL_Window * init_view () {
  // bug sans problème de pilote graphique
  // désactive accélération matériel
  SDL_SetHint(SDL_HINT_RENDER_DRIVER, "software");
  SDL_SetHint(SDL_HINT_FRAMEBUFFER_ACCELERATION, 0);

  //Variable :

  SDL_Window * window;
  //SDL_Renderer *renderer, *rendererW;


  // initialisation;



  if(SDL_VideoInit(NULL) < 0)
  {
      printf("Erreur d'initialisation de la SDL : %s",SDL_GetError());
    //  return EXIT_FAILURE;
  }
  if(TTF_Init() < 0 ){
    printf("Erreur d'initiaisation de la TTF : %s\n", TTF_GetError());
  }
  window = SDL_CreateWindow("Une fenetre SDL" , SDL_WINDOWPOS_CENTERED , SDL_WINDOWPOS_CENTERED , 800 , 800 , 0);
  if(window == NULL)
  {
      printf("Erreur lors de la creation d'une fenetre : %s",SDL_GetError());
      //return EXIT_FAILURE;
  }
  return window;
  //SDL_Delay(10000);
}
void play_view (SDL_Window * window) {
  SDL_Event event;
  SDL_bool quit = SDL_FALSE;
  SDL_Renderer *renderer;
  //damier = allocDamier();
  Damier damier[10][10];
  Move * move = malloc(sizeof(move));
  init_game(damier);
  place_tile (damier);
  affichermatrice(damier);
  SDL_Texture **arrayTexture;
  renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
  if(renderer == NULL)
  {
      printf("Erreur lors de la creation d'un renderer : %s",SDL_GetError());
      return;
  }
  arrayTexture = create_texture (renderer, arrayTexture);

while(!quit)
{

    //SDL_Delay(20);
  //  printf("je suis la \n" );

  print_damier(renderer, arrayTexture, damier);
  SDL_RenderPresent(renderer);
    SDL_WaitEvent(&event);
    quit = exit_client (event,quit);
    eventClient(event, renderer, damier, move);


}
//SDL_DestroyWindow(window);
SDL_Quit();
}

SDL_bool exit_client (SDL_Event event, SDL_bool  quit){
  if(event.type == SDL_QUIT){
      return SDL_TRUE;
    }
    return SDL_FALSE;
}
void eventClient(SDL_Event event, SDL_Renderer * renderer, Damier damier [10][10], Move * move){

  if(event.type == SDL_MOUSEBUTTONDOWN){
    printf("%s\n","passe" );

      event_click(renderer,event,damier,move);
  }
}
/**************************************************************/
/**************** change la postion d'un pion *****************/
/**************************************************************/
void change_position (Damier damier[10][10], Move *move) {
  damier[move->newPosition[0]][move->newPosition[1]].pion = damier[move->position[0]][move->position[1]].pion;
  damier[move->position[0]][move->position[1]].pion = VIDE;
  reset_st_move(move);
}
/**************************************************************/
/**************** Efface les donnees dans move  ***************/
/**************************************************************/
void reset_st_move (Move * move){
  move->position[0] = move->position[1] = -1;
  move->newPosition[0] = move->newPosition[1] = -1;
}

/**************************************************************/
/**** verifie si evenement est un click gauche ou droit *******/
/**************************************************************/
void event_click (SDL_Renderer * renderer, SDL_Event event,Damier damier[10][10], Move *move) {
  if(event.button.button == SDL_BUTTON_LEFT){
    printf("%s\n","click gauche" );
    if (select_pion(event, damier, move )== 1){
      puts("je colorie");
      // print_blue_tile(renderer, &damier[move->position[0]][move->position[1]].cases);
      // SDL_RenderPresent(renderer);
    }
  }
  else if (event.button.button == SDL_BUTTON_RIGHT) {
    printf("%s\n","click droit" );
    move_pion (event, damier, move);
  }
}

/**************************************************************/
/*********Selectionne un pion si la case est pas vide *********/
/**************************************************************/
int select_pion (SDL_Event event, Damier damier[10][10], Move *move) {
  reset_st_move(move);
  move->position[0] = event.motion.y/80;
  move->position[1] = event.motion.x/80;
  if(control_position_pion(damier, move)) {
    //TODO : appel de color_postition();
    return 1;
  }
  else {
    reset_st_move(move);
  }
  return 0;
}

/**************************************************************/
/*********Déplace le pion si les condition snt remplies *********/
/**************************************************************/
int  move_pion (SDL_Event event, Damier damier[10][10], Move *move) {
  move->newPosition[0] = event.motion.y/80;
  move->newPosition[1] = event.motion.x/80;
  if(control_position_empty(damier, move)) {
    if(move->position[0] != -1 && move->position[1] != -1) {

      prisePion(damier, 2, move->position[0], move->position[1], move->newPosition[0], move->newPosition[1], move->newPosition[0]-move->position[0], move->newPosition[1]-move->position[1]);
      change_position(damier, move);
      return 1;
    }
  }
  return 0;
}
// TODO : color les cases posibles en bleu, appeller une fonction qui renvoie tout les coups possibles
// TODO : Si le coup est possible on récupère la position  carreau dans le damier.cases et on appel la Fonction
// TODO : print_blue_tile
void color_postition () {

}

/**************************************************************/
/*********Controle si le pion selectionner est valide *********/
/**************************************************************/
int control_position_pion (Damier damier[10][10], Move *move) {
  printf(" old postion  = %d, %d\n",move->position[0],move->position[1]  );
  if (move->position[0] >= 0                              &&
              move->position[0] < SIZE_DAMIER             &&
              move->position[1] >= 0                      &&
              move->position[1] < SIZE_DAMIER             &&
              damier[move->position[0]][move->position[1]].pion != 0) {
    return 1;
  }
  return 0;
}






/**************************************************************/
/*********Controle si le pion selectionner est vide *********/
/**************************************************************/
// TODO : manque la vérification  que la position soit correct avant de déplacer
// TODO :
int control_position_empty (Damier damier[10][10], Move *move){
  printf(" new postion  = %d, %d\n",move->newPosition[0],move->newPosition[1]  );
  if (move->newPosition[0] >= 0                               &&
              move->newPosition[0] < SIZE_DAMIER              &&
              move->newPosition[1] >= 0                       &&
              move->newPosition[1] < SIZE_DAMIER              &&
              deplacementValide(damier, 2, move->position[0], move->position[1], move->newPosition[0], move->newPosition[1]) &&
              damier[move->newPosition[0]][move->newPosition[1]].pion == 0)
  {

    return 1;
  }
  puts("test vide echec");
  return 0;
}
