#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include <netdb.h>
#include "login.h"
#include <SDL2/SDL_image.h>
#include <SDL2/SDL.h>
#include <fcntl.h>
#include <pthread.h>
// #include <SDL2/SDL_ttf.h>
#include "sdl2.h"
#include "communication.h"
#define TAILLE_SAISIE 500
pthread_cond_t condition = PTHREAD_COND_INITIALIZER; /* Création de la condition */
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER; /* Création du mutex */
typedef struct in_addr IN_ADDR;
typedef struct Game Game;
struct Game {

  int socket;
  int numPlayer;
  char * buffer;
  int change;
  int roundPlayer;
  //Damier damier[10][10];
};

int  is_observer();
int number_player (int socket);
int chooseMod (int port);
void * game_play(void * arg);
Game * init_st_game ( int socket, int numberPlayer);
void saisie(char *buffer){
  memset(buffer,'\0',sizeof(buffer));
  fgets(buffer,TAILLE_SAISIE,stdin);
}
int  connexion(int port)
{
  char *ip = "127.0.0.1";
  struct hostent *adr;
  long host_addr;
  int socket_connexion;
  struct sockaddr_in adresse_serveur;
  char buffer_addresse[20];
  char pseudo[21];

  socket_connexion = socket(AF_INET, SOCK_STREAM, 0);

  if (socket_connexion == -1)
  {
    perror( "socket()" );
    exit(-1);
  }
  adr = gethostbyname(ip);
  memset(&adresse_serveur, 0,sizeof(adresse_serveur));
  adresse_serveur.sin_family = AF_INET;
  adresse_serveur.sin_port = htons (port);
  adresse_serveur.sin_addr =*(IN_ADDR*) adr->h_addr;
  //inet_pton (AF_INET, "93.29.84.53", &(adresse_serveur.sin_addr.s_addr));

  if ( connect(
    socket_connexion
    ,(const struct sockaddr *) &adresse_serveur
    ,sizeof( adresse_serveur)
  ) == -1 )
  {
    perror("connect()");
    exit(-1);
  }


  printf( "connexion a %s : %d reussie \n"
  ,inet_ntop(
    AF_INET
    ,&(adresse_serveur.sin_addr)
    ,buffer_addresse
    ,128)
    , adresse_serveur.sin_port);
    return socket_connexion;
  }
  int observer = 0;
  void client(int port)
  {
    //Message
    ssize_t taille_recue;
    char *buffer_message;
    fd_set rd;
    char buffer_saisie[200];
    int numberPlayer = 0;
    int control =0;
    int controlChange =0;
    int count = 0;
    int flag = 0;
    int controlSend = 0;


    pthread_t thread;

    /********************************************************/
    /**********************Varaible SDL *****************************/
    SDL_Event event;
    SDL_bool quit = SDL_FALSE;
    SDL_Renderer *renderer;
    SDL_Texture **arrayTexture;
    SDL_Window * window;
    //varaible de struct
    Damier damier[10][10];
    Move * move = malloc(sizeof(move));
    Game * game = NULL;
    // initialisation du damier
    init_game(damier);
    place_tile (damier);
    //affichermatrice(damier);
    int socket_connexion = chooseMod(port);
    //initialise et cree une fenetre
    window = init_view ();
    //initialise et crée un renderer
    renderer = create_renderer(window);
    //contient toutes les textures utilisé
    arrayTexture = create_texture (renderer, arrayTexture);


    /********************************************************/
    printf("obser : %d\n",observer );
    puts("avant co client");
    //chooseMod (socket_connexion);
    if (observer == 1) {
      numberPlayer = number_player (socket_connexion);
      control = numberPlayer;
      if(numberPlayer == 1){
        controlChange = 1;
      }
      else {
        controlChange = 0;
      }
    }
    else {
      controlChange = 0;
    }
    /********************************************************/
    //afficheConnection (renderer);
    printf("%s\n","la" );

    printf("%s\n","la2" );

    //game = {event, socket, numberPlayer, damier };
    game = init_st_game(socket_connexion, numberPlayer);
    print_damier(renderer, arrayTexture,damier);
    SDL_RenderPresent(renderer);
    pthread_create(&thread, NULL, game_play, game);


    while (!quit) {
      // puts("SDL_WaitEvent00");
      SDL_PollEvent(&event);
      quit = exit_client (event,quit);
      if(game->change == 1){
        //  pthread_mutex_lock (&mutex); /* On verrouille le mutex */
        changeDamier(damier,game->buffer,numberPlayer);
        print_damier(renderer, arrayTexture,damier);
        SDL_RenderPresent(renderer);
        puts("crash");
        game->change =0;
        write_serveur(socket_connexion,"OK");

			  //pthread_cond_signal (&condition); /* On délivre le signal : condition remplie */
			// pthread_mutex_unlock (&mutex); /* On déverrouille le mutex */
        puts("crash2");
      }
      else if(game->roundPlayer == 1){
        controlSend = eventClient(event, renderer, damier, move, socket_connexion,numberPlayer);
        if(controlSend == 1) {
          print_damier(renderer, arrayTexture,damier);
          SDL_RenderPresent(renderer);
          game->roundPlayer = 0;
        }

      }

    //  SDL_Delay(100);

      // if(pthread_tryjoin_np(thread, &arg) == 0){
      // //   controlSend = 1;
      // // }



    }
    close(socket_connexion);

  }
  void * game_play (void * arg) {
    int controlChange;
    char buffer [100];
    printf("Dans le thread \n");
    Game *game = arg;
    int i = 0;
    while(1){

      sprintf(game->buffer,"%s",recv_msg_server(game->socket,buffer));
      controlChange = control_recv_msg(game->buffer);
    //  memset(game->buffer[1],'\0',100);
      i++;
      if(controlChange == 0){
        puts("Le serveur donne la main");
        game->roundPlayer = 1;
        //SDL_Delay(10000);
      }
      else if (controlChange == 1){
         // pthread_mutex_lock(&mutex);
  		   // pthread_cond_wait (&condition, &mutex);
        puts("on change la matrice et on reécoute");
        game->change = 1;
         // pthread_mutex_unlock(&mutex);
      }
    }
  }
  Game * init_st_game ( int socket, int numberPlayer){
    Game *game = malloc(sizeof(game));
    game-> buffer = malloc(100);
    // for(int i = 0; i < 2; i++ ){
    //   game->buffer[i] = malloc(100);
    // }
    game-> socket = socket;
    game-> numPlayer = numberPlayer;
    game->change =0;
    game->roundPlayer =0;
    return game;
  }
  int chooseMod (int port) {
    char choose [100];
    char pseudo [100];
    char pwd    [100];
    //int observer = 0;
    int socket_connexion;
    int true = 0;

    while (!true) {
      printf("1. Mode invite 2.connexion 3.inscription \n");
      scanf("%s", &choose);
      puts("saisie effectuer");
      if (choose[0] == '1') {
        observer = is_observer();
        socket_connexion = connexion(port);
        connexion_client(socket_connexion, "invite", "", observer,3);

        return socket_connexion;
      }
      else if (choose[0] == '2' || choose[0] == '3') {
        saisie_login(pseudo,20,"Votre pseudo : ",0);
        saisie_login(pwd, 20,"votre mot de passe : ",1);
        observer = is_observer();
        socket_connexion = connexion(port);
        if(choose[0] == 2) {
          connexion_client(socket_connexion, pseudo, pwd, observer,2);
          return socket_connexion;
        }
        else {
          connexion_client(socket_connexion, pseudo, pwd, observer,1);
          return socket_connexion;
        }
      }
      else  {
        printf("Saisie incorrect \n");
        return 0;
      }
    }
  }
  int  is_observer () {
    char buffer [3];
    printf("1.Jouer / 2.observe\n");
    while (buffer[0] != '1' || buffer[0] != '2'){
      scanf("%s", &buffer);
      buffer[1] = '\0';
      if(buffer[0] == '1' || buffer[0] == '2'){
        return atoi(buffer);
      }
    }
    return atoi(buffer);
  }
  /*  void write_serveur(int socket, char *buffer)
  {
  int taille_envoyee;
  taille_envoyee = send( socket, buffer, strlen(buffer), 0);
  if (taille_envoyee == -1)
  {
  perror("send()");

}
printf("%d *****-******\n", taille_envoyee);
}

void read_serveur(int socket, char *buffer)
{
  ssize_t taille_recue;
  taille_recue = recv(socket, buffer,100,0);
  if (taille_recue == -1){
    perror("recv()");
  }
  printf("%ld *****-******\n", taille_recue);
}*/

int number_player (int socket)
{
  char buffer [100];
  read_serveur(socket, buffer);
  printf("buffer ; %s\n",buffer );
  printf("numéro du joueur = %d\n",charInInt(buffer[2] ));
  return charInInt(buffer[2]);
}


int main (int argc, char **argv)
{
  if(argc != 2) {
    printf("port pseudo pwd (joueur = 1 / observateur = 2) _ 1 pour inscription / 2 sinon ");
    exit(1);
  }
  client(atoi(argv[1]));

  return 0;
}
