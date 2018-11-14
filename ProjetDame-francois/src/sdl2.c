#include <stdlib.h>
#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <fcntl.h>
#include "struct.h"
#include "jeu_dame.h"
//#include <SDL2/SDL_ttf.h>
//#include "deplacement.h"
#include "sdl2.h"

//#include "SDL2/SDL_Image.h"
//#include <SDL/SDL_image.h>

#define SIZE_DAMIER 10
#define SIZE_CARREAU 80

#define VIDE 0
#define PION 1
#define REINE 2
#define PRIS 3
#define MULTIPLICATEUR_JOUEUR 10

#define SIZE_ARRAY_TEXTURE 5




/*
void afficheConnection (SDL_Renderer * renderer) {
puts("a");
int h1 = 0, w1 = 0, h2 = 200, w2 =200;
char str[1000];
int ch, n = 0;
SDL_Color color = { 255, 255, 255 };
TTF_Font * police;
puts("a");
police = TTF_OpenFont("arial.ttf", 25);
if(police == NULL) {
printf("%s\n","police nullle" );
}
puts("a");
SDL_Surface * surface = TTF_RenderText_Solid(police,"hello",color);
SDL_Texture * texture = SDL_CreateTextureFromSurface(renderer,surface);
SDL_QueryTexture(texture,NULL, NULL,&w2,&h2);
SDL_Rect rect1 = {w2 ,h2, 100, 100 };
SDL_Surface * surface2;
SDL_Texture * texture2;
//SDL_QueryTexture(texture,NULL, NULL,&w2,&h2);
SDL_Rect rect2 = { 0, 0, 200, 200 };
SDL_RenderCopy(renderer,texture,NULL,&rect1);
while (1) {
n=0;
while ((ch = getchar()) != EOF && n < 1000 ) {
printf("%s\n","dans la boucle" );
printf("%c\n",ch );
str[n] = ch;
++n;
surface2 = TTF_RenderText_Solid(police,str,color);
texture2 = SDL_CreateTextureFromSurface(renderer,surface2);
SDL_RenderCopy(renderer,texture2,NULL,&rect2);
SDL_RenderPresent(renderer);
//  SDL_Flip(surface2);
}

}



//SDL_RenderCopy(renderer,texture2,NULL,&rect2);
puts("a");
SDL_RenderPresent(renderer);
puts("a");
//message = TTF_RenderText_Solid( police, "Test pour sdl_ttf", textColor );
//apply_surface( 0, 200, message, screen );
SDL_Delay(10000);
}

*/
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

          damier[i][j].pion = 1 * MULTIPLICATEUR_JOUEUR + PION;

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
  /*damier[5][7].pion = 1 * MULTIPLICATEUR_JOUEUR + REINE;
	damier[4][6].pion = 2 * MULTIPLICATEUR_JOUEUR + PION;
	damier[4][4].pion = 2 * MULTIPLICATEUR_JOUEUR + PION;
	damier[3][3].pion = 2 * MULTIPLICATEUR_JOUEUR + PION;
	damier[6][4].pion = 2 * MULTIPLICATEUR_JOUEUR + PION;
	damier[6][2].pion = 2 * MULTIPLICATEUR_JOUEUR + PION;*/
  // damier[1][1].pion = 1 * MULTIPLICATEUR_JOUEUR + PION;
  // damier[8][8].pion = 2 * MULTIPLICATEUR_JOUEUR + PION;
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
/******************* Libere les textures et le tableau *******************/
/* In : Tableau de texture ***********************************************/
/*************************************************************************/
void freeTexture (SDL_Texture ** arrayTexture){
for(int i = 0; i < SIZE_ARRAY_TEXTURE; i++){
  SDL_DestroyTexture(arrayTexture[i]);
}
free(arrayTexture);
}
/*********************** Affiche une image  ******************************/
/** Prend en parametre, un renderer, le chemin de l'image et ça position */
/*************************************************************************/
SDL_Texture ** create_texture (SDL_Renderer *renderer, SDL_Texture ** arrayTexture) {
  arrayTexture = malloc(5 * sizeof(arrayTexture));

  SDL_Surface* picturePW = IMG_Load("../picture/pionBlanc.png");
  SDL_Surface* picturePB = IMG_Load("../picture/pionNoir.png");
  SDL_Surface* pictureLW = IMG_Load("../picture/queenW.png");
  SDL_Surface* pictureLB = IMG_Load("../picture/queenB.png");
  SDL_Surface* picturePris = IMG_Load("../picture/piecePris.png");
  if(picturePB == NULL) {
    printf("texture null\n" );
  }

  arrayTexture[TEXTURE_PW] = SDL_CreateTextureFromSurface(renderer,picturePW);
  arrayTexture[TEXTURE_PB] = SDL_CreateTextureFromSurface(renderer,picturePB);
  arrayTexture[TEXTURE_LW] = SDL_CreateTextureFromSurface(renderer,pictureLW);
  arrayTexture[TEXTURE_LB] = SDL_CreateTextureFromSurface(renderer,pictureLB);
  arrayTexture[TEXTURE_PRIS] = SDL_CreateTextureFromSurface(renderer,picturePris);
  for(int i = 0 ; i < 5 ; i++){
    if(arrayTexture[i] == NULL){
      perror("SDL_CreateTextureFromSurface()");
    }
  }
  return arrayTexture;
}
void print_picture (SDL_Renderer *renderer,SDL_Texture ** arrayTexture, int enumTexture, SDL_Rect cases) {
  SDL_QueryTexture(arrayTexture[enumTexture], NULL, NULL, &cases.w, &cases.h);
  if(renderer == NULL) {
    perror("renderer()");
  }

  //printf("cases : %d, %d\n",cases.w,cases.h );
  SDL_Delay(50);
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
  //affichermatrice(damier);
  for (int  i = 0; i < SIZE_DAMIER; i++) {
    for (int j = 0; j < SIZE_DAMIER; j++) {
      SDL_Rect * cs = &damier[i][j].cases;
      switch (damier[i][j].pion) {
        case VIDE :
        if((i+j)%2 == 0 )
        {
          print_black_tile(renderer,cs);
        }
        else
        {
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
        case (1 * MULTIPLICATEUR_JOUEUR + PRIS) :
        print_picture(renderer,arrayTexture,TEXTURE_PRIS,damier[i][j].cases);
        break;
        case (2 * MULTIPLICATEUR_JOUEUR + PRIS) :
        print_picture(renderer,arrayTexture,TEXTURE_PRIS,damier[i][j].cases);
        break;
        default :
        print_blue_tile(renderer,cs);
        break;
      }
    }
  }
}

void print_case_damier (SDL_Renderer *renderer,SDL_Texture ** arrayTexture, Damier  damier[10][10], int i, int j)
{
  SDL_Rect * cs = &damier[i][j].cases;
  switch (damier[i][j].pion)
  {
    case VIDE :
    if((i+j)%2 == 0 )
    {
      print_black_tile(renderer,cs);
    }
    else
    {
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
/**************************************************************/

// int main(int argc, char** argv)
// {
//   SDL_Renderer *renderer;
//   SDL_Window * window;
//     window = init_view ();
//    play_view (window);
// }

SDL_Window * init_view () {
  // bug sans, problème de pilote graphique, surement du au bash on windows
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
  /*if(TTF_Init() < 0 ){
  printf("Erreur d'initiaisation de la TTF : %s\n", TTF_GetError());
}*/
window = SDL_CreateWindow("Jeu de Dame" , SDL_WINDOWPOS_CENTERED , SDL_WINDOWPOS_CENTERED , 800 , 800 , 0);
if(window == NULL)
{
  printf("Erreur lors de la creation d'une fenetre : %s",SDL_GetError());
  exit(EXIT_FAILURE);
}
return window;

}
SDL_Renderer *create_renderer (SDL_Window *window) {
  SDL_Renderer *renderer;
  renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
  if(renderer == NULL)
  {
    printf("Erreur lors de la creation d'un renderer : %s",SDL_GetError());
    exit(EXIT_FAILURE);
  }
  return renderer;
}


SDL_bool exit_client (SDL_Event event, SDL_bool  quit, int socket){
  if(event.type == SDL_QUIT){
    write_serveur(socket,"5");
    return SDL_TRUE;
  }
  return SDL_FALSE;
}
int eventClient(SDL_Event event, SDL_Renderer * renderer, Damier damier [10][10], Move * move, int socket,int numberPlayer){
  int control = 0;
  if(event.type == SDL_MOUSEBUTTONDOWN){
    control = event_click(renderer,event,damier,move,socket,numberPlayer);
    if(control)
    {
      return 1;
    }

  }
  return 0;
}
/**************************************************************/
/**************** change la postion d'un pion *****************/
/**************************************************************/
void change_position (Damier damier[10][10], Move *move)
{
  int joueurActuel = (damier[move->position[0]][move->position[1]].pion)/MULTIPLICATEUR_JOUEUR;
  effectueDeplacement(damier, joueurActuel, move->position[0], move->position[1], move->newPosition[0], move->newPosition[1]);

  // if(joueurActuel*9-9 == move->newPosition[0])
  // {
  //   damier[move->newPosition[0]][move->newPosition[1]].pion = joueurActuel * MULTIPLICATEUR_JOUEUR + REINE;
  //   damier[move->position[0]][move->position[1]].pion = VIDE;
  // }
  // else
  // {
  //   damier[move->newPosition[0]][move->newPosition[1]].pion = damier[move->position[0]][move->position[1]].pion;
  //   damier[move->position[0]][move->position[1]].pion = VIDE;
  // }


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
int event_click (SDL_Renderer * renderer, SDL_Event event,Damier damier[10][10], Move *move, int socket,int numberPlayer)
{
  char buffer [100];
  if(event.button.button == SDL_BUTTON_LEFT)
  {
    if (select_pion(event, damier, move )== 1)
    {
      // print_blue_tile(renderer, &damier[move->position[0]][move->position[1]].cases);
      // SDL_RenderPresent(renderer);
    }
  }
  else if (event.button.button == SDL_BUTTON_RIGHT)
  {
    int control = 0;
    control = move_pion (event, damier, move, numberPlayer);
    if (control)
    {
      send_move(move, NULL, 0,4,buffer);
      printf("%d\n",socket );
      write_serveur(socket,buffer);
      puts("mouvement envoyé");
      printf("%s ******\n", buffer);
      // if(canTakePion(damier, numberPlayer, move->newPosition[0], move->newPosition[1]))
      // {
      //   printf("prise possible\n");
      //   reset_st_move(move);
      //   return 0;
      // }
      reset_st_move(move);
      return 1;
    }
    else
    {
      reset_st_move(move);
    }

  }
  return 0;
}

char * recv_msg_server (int socket, char * buffer ){
  puts("recv_msg_server()");
  read_serveur(socket,buffer);
  printf("recv_msg() : %s\n",buffer );
  return buffer;
}

int control_recv_msg(char * buffer){
  switch (charInInt(buffer[0])) {
    case 3:
    puts("return 0");
    return 0;
    break;
    case 4:
    puts("return 0");
    return 1;
    break;
    case 5:
    puts("un gagnant ou une deco");
    return 2;
    break;
  }
}
void changeDamier ( Damier damier[10][10],char * buffer, int numberPlayer)
{
  puts("changeDamier()");
  if (strlen(buffer)> 1){
    char ** result;
    int i = 0;

    printf("buffer : %s\n",buffer );
    while (buffer[i++] != '\0'){

      if(buffer[i] == '*'){
        //  puts("je change");
        buffer[i]= '\0';
      }
    }

    printf("longeur : %ld\n",strlen(buffer) );
    //printf("char i - 1 : %c\n",buffer[10] );

    result = read_move_query(buffer);
    puts(buffer);

    printf("char  :%c\n",buffer[strlen(buffer)-2]);
    i = 0;
    while (i != 2){
      printf("result = %c/%c\n",result[i][0],result[i][1] );
      i++;
    }
    puts("middle changeDamier");
    effectueDeplacement(damier, numberPlayer, charInInt(result[0][0]), charInInt(result[0][1]), charInInt(result[1][0]), charInInt(result[1][1]));
    int prise = marquePion(damier, numberPlayer, charInInt(result[0][0]), charInInt(result[0][1]), charInInt(result[1][0]), charInInt(result[1][1]));
  }
  puts("fin changeDamier");
}
/**************************************************************/
/*********Selectionne un pion si la case est pas vide *********/
/**************************************************************/
int select_pion (SDL_Event event, Damier damier[10][10], Move *move) {
  reset_st_move(move);
  move->position[0] = event.motion.y/80;
  move->position[1] = event.motion.x/80;
  printf("select pion   %d, %d\n",move->position[0],move->position[1] );
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
int  move_pion (SDL_Event event, Damier damier[10][10], Move *move, int numberPlayer)
{
  move->newPosition[0] = event.motion.y/80;
  move->newPosition[1] = event.motion.x/80;
  if(control_position_empty(damier, move, numberPlayer))
  {
    int deplacement = deplacementValide(damier, numberPlayer, move->position[0], move->position[1], move->newPosition[0], move->newPosition[1]);
    if(deplacement)
    {
      if(move->position[0] != -1 && move->position[1] != -1)
      {
        int max = rafleMax(damier, numberPlayer);
        Damier copie[10][10] = {0};
        copieDamier(damier, copie);
        effectueDeplacement(copie, numberPlayer, move->position[0], move->position[1], move->newPosition[0], move->newPosition[1]);
        int maxPion = rafleMaxPion(copie, numberPlayer, move->newPosition[0], move->newPosition[1]);
        printf(" max : %d maxPion : %d deplacement : %d \n", max, maxPion, deplacement);
        if(max == 0 || (max == maxPion + 1 && deplacement == 2))
        {

          change_position(damier, move);
          if(maxPion == 0)
          {
            deletePion(damier);
          }

          return 1;
        }

      }
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
  //printf(" old postion  = %d, %d\n",move->position[0],move->position[1]  );
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
  int control_position_empty (Damier damier[10][10], Move *move,int numberPlayer){
    //printf(" new postion  = %d, %d\n",move->newPosition[0],move->newPosition[1]  );
    int deplacement = deplacementValide(damier, numberPlayer , move->position[0], move->position[1], move->newPosition[0], move->newPosition[1]);
    if (move->newPosition[0] >= 0                               &&
      move->newPosition[0] < SIZE_DAMIER              &&
      move->newPosition[1] >= 0                       &&
      move->newPosition[1] < SIZE_DAMIER              &&
      deplacement                                     &&
      damier[move->newPosition[0]][move->newPosition[1]].pion == 0)
      {

        return 1;
      }
      puts("test vide echec");
      return 0;
    }
    void send_move (Move * move, int ** arrayCapture, int size,int option, char * buffer  ){
      printf("new position : %d;%d\n",move->newPosition[0],move->newPosition[1] );
      sprintf(buffer,"%d/%d-%d/%d-%d",option,move->position[0],move->position[1],move->newPosition[0],move->newPosition[1]);
      for (int i = 0; i < size; i++){
        printf("%s\n","ta");
        sprintf(buffer,"%s/%d-%d",buffer,arrayCapture[i][0],arrayCapture[i][0]);
      }
      sprintf(buffer,"%s/%d%c",buffer,size,'*');

    }
    char ** read_move_query (char * buffer)
    {
      int       i         = 0,
      r         = 0,
      count     = 0,
      ok        = 1;
      const int sizeAlloc = 3;

      char      temp [100];
      char ** result;
      result = malloc(sizeAlloc * sizeof(result));
      if(result == NULL)
      {
        perror("malloc()");
        exit(1);
      }

      // printf("ant decomposition : %s\n",buffer );
      while(ok){
        //printf("char : %c\n",buffer[i] );
        if(buffer[i] != '/' && buffer[i] != '\0' && buffer[i] != '-'){
          temp[r] = buffer[i];
          i++;
          r++;
        }
        else if (buffer[i] == '/') {
          puts("passe dans le else");
          i++;
          result[count] = malloc(3*sizeof(result));
          if(result[count] == NULL){
            perror("malloc()");
            exit(1);
          }
          printf("%s\n",temp );
          strcpy(result[count],temp);
          count++;
          r = 0;
          // memset(temp,'\0',strlen(temp));
        }
        else if (buffer[i] == '-') {
          char c = buffer[i+1] ;
          printf("buffer i +1 : %c\n",c );
          sprintf(result[count-1],"%c%c",temp[0],c);
          i++;
        }
        else if (buffer[i] == '\0') {
          ok = 0;
        }

      }
      printf("result 3 : %c\n",result[1][1] );
      return result;
    }
    int charInInt (char c) {
      return c - '0';
    }

    void read_serveur(int socket, char *buffer)
    {
      ssize_t taille_recue;
      taille_recue = recv(socket, buffer,1000,0);
      printf(" Buffer : read_server %s\n", buffer);
      if (taille_recue == -1){
        perror("recv()");
      }
    }

    void write_serveur(int socket, char *buffer)
    {
      int taille_envoyee;
      taille_envoyee = send( socket, buffer, strlen(buffer), 0);
      if (taille_envoyee == -1)
      {
        perror("send()");

      }
      printf("%d *****-******\n", taille_envoyee);
    }
