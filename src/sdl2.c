#include <stdlib.h>
#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
 //#include "SDL2/SDL_Image.h"
//#include <SDL/SDL_image.h>

#define SIZE_DAMIER 10
#define SIZE_CARREAU 80


typedef struct Move {
  int position[2];
  int newPosition[2];
}Move;

typedef struct Damier {
  int pion;
  SDL_Rect cases;
}Damier;

void reset_st_move (Move * move);
int  move_pion (SDL_Event event, Damier damier[10][10], Move *move);
void event_click (SDL_Renderer *renderer, SDL_Event event, Damier damier[10][10], Move *move);
int select_pion (SDL_Event event, Damier damier[10][10], Move *move);
int control_position_pion (Damier damier[10][10], Move *move) ;
int control_position_empty (Damier damier[10][10], Move *move) ;
void changeposition (Damier damier[10][10], Move *move);
void init_game (Damier damier[10][10]);
void place_tile (Damier damier[10][10]);
void print_picture (SDL_Renderer *renderer, char * pathPicture, SDL_Rect cases);

enum {EMPTY, PION_W, PION_B, QUEEN_W, QUEEN_B};
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

          damier[i][j].pion = PION_W;
        }
        else if (i > 5){

          damier[i][j].pion = PION_B;

        }
        else {
          damier[i][j].pion = EMPTY;
        }
      }
      else {

        damier[i][j].pion = EMPTY;
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
void print_picture (SDL_Renderer *renderer, char * pathPicture, SDL_Rect cases) {

  SDL_Surface* picture = IMG_Load(pathPicture);
  SDL_Texture* myPicture = SDL_CreateTextureFromSurface(renderer,picture);
  SDL_QueryTexture(myPicture, NULL, NULL, &cases.w, &cases.h);
  SDL_RenderCopy(renderer,myPicture,NULL,&cases);
  SDL_DestroyTexture(myPicture);
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
void print_damier (SDL_Renderer *renderer, Damier  damier[10][10]) {
  for (int  i = 0; i < SIZE_DAMIER; i++) {
    for (int j = 0; j < SIZE_DAMIER; j++) {
      SDL_Rect * cs = &damier[i][j].cases;
      switch (damier[i][j].pion) {
        case EMPTY :
          if((i+j)%2 == 0 ) {
            print_black_tile(renderer,cs);
          }
          else {
            print_white_tile(renderer,cs);
          }
          break;
        case PION_W :
          print_picture(renderer,"../picture/pionBlanc.png",damier[i][j].cases);
          break;
        case PION_B :
          print_picture(renderer,"../picture/pionNoir.png",damier[i][j].cases);
          break;
        case QUEEN_W :
          print_picture(renderer,"../picture/queenW.png",damier[i][j].cases);
          break;
        case QUEEN_B :
          print_picture(renderer,"../picture/quennB.png",damier[i][j].cases);
          break;
        default :
          print_blue_tile(renderer,cs);
          break;
      }
    }
  }
}
/**************************************************************/

int main(int argc, char** argv)
{

  // bug sans problème de pilote graphique
  // désactive accélération matériel
  SDL_SetHint(SDL_HINT_RENDER_DRIVER, "software");
  SDL_SetHint(SDL_HINT_FRAMEBUFFER_ACCELERATION, 0);

  //Variable :
  SDL_Event event;
  SDL_bool quit = SDL_FALSE;
  SDL_Window * window;
  SDL_Renderer *renderer, *rendererW;
  Damier damier[10][10];
  Move * move = malloc(sizeof(move));

  // initialisation;



  if(SDL_VideoInit(NULL) < 0)
  {
      printf("Erreur d'initialisation de la SDL : %s",SDL_GetError());
      return EXIT_FAILURE;
  }
  window = SDL_CreateWindow("Une fenetre SDL" , SDL_WINDOWPOS_CENTERED , SDL_WINDOWPOS_CENTERED , 800 , 800 , 0);
  if(window == NULL)
  {
      printf("Erreur lors de la creation d'une fenetre : %s",SDL_GetError());
      return EXIT_FAILURE;
  }
  renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
  if(renderer == NULL)
  {
      printf("Erreur lors de la creation d'un renderer : %s",SDL_GetError());
      return EXIT_FAILURE;
  }

  //damier = allocDamier();
  init_game(damier);
  place_tile (damier);
  affichermatrice(damier);


while(!quit)
{

    //SDL_Delay(20);

    print_damier(renderer,damier);
    SDL_RenderPresent(renderer);
    SDL_WaitEvent(&event);
    //test = 0;
    if(event.type == SDL_QUIT){
        quit = SDL_TRUE;
      }
    if(event.type == SDL_MOUSEBUTTONDOWN){
      printf("%s\n","passe" );

        event_click(renderer,event,damier,move);

    }


}
SDL_DestroyWindow(window);
SDL_Quit();
}


/**************************************************************/
/**************** change la postion d'un pion *****************/
/**************************************************************/
void change_position (Damier damier[10][10], Move *move) {
  damier[move->newPosition[0]][move->newPosition[1]].pion = damier[move->position[0]][move->position[1]].pion;
  damier[move->position[0]][move->position[1]].pion = EMPTY;
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
              damier[move->newPosition[0]][move->newPosition[1]].pion == 0) {
    return 1;
  }
  puts("test vide echec");
  return 0;
}
