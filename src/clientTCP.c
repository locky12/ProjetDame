
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
#include <SDL2/SDL_ttf.h>
#include "sdl2.h"
#include "communication.h"
#define TAILLE_SAISIE 500
typedef struct in_addr IN_ADDR;





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
  void client(int port, char * pseudo, char * pwd, int observer, int inscription)
  {
    //Message
    ssize_t taille_recue;
    char buffer_message[200];
    fd_set rd;
    char buffer_saisie[200];
    /********************************************************/
/**********************Varaible SDL *****************************/
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
SDL_Window * window;
  window = init_view ();
// creation du renderer TODO : mettre dans une fonction :
renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
if(renderer == NULL)
{
    printf("Erreur lors de la creation d'un renderer : %s",SDL_GetError());
    return;
}
// initialise le tableau avec les pion selectionné
arrayTexture = create_texture (renderer, arrayTexture);


/********************************************************/
    int socket_connexion = connexion(port);
    puts("avant co client");
    connexion_client(socket_connexion, pseudo, pwd, observer,inscription);
    //afficheConnection (renderer);
    while(1)
    {

      print_damier(renderer, arrayTexture, damier);
      SDL_RenderPresent(renderer);
      SDL_WaitEvent(&event);
      quit = exit_client (event,quit);
      eventClient(event, renderer, damier, move);
      memset(&buffer_message,'\0',sizeof(buffer_message));
      // FD_ZERO(&rd);
      // FD_SET(STDIN_FILENO,&rd);
      // FD_SET(socket_connexion,&rd);
      // select(socket_connexion + 1,&rd,0,0,0);
      // if (FD_ISSET(STDIN_FILENO,&rd))
      // {
      //
      //   saisie(buffer_saisie);
      //   write_serveur(socket_connexion,buffer_saisie);
      //   if(strncmp(buffer_saisie,"quit",4)==0)
      //   {
      //     close(socket_connexion);
      //     exit(1);
      //   }
      //
      // }
      // else if (FD_ISSET(socket_connexion,&rd)) {
      //   taille_recue = recv(socket_connexion, buffer_message, sizeof(buffer_message),0);
      //   if (taille_recue == -1){
      //     perror("recv()");
      //     exit (-1);
      //   }
      //   else {printf(": %s", buffer_message );}
      //
      // }
    }
    close(socket_connexion);

  }



  int main (int argc, char **argv)
  {
    if(argc != 6) {
      printf("port pseudo pwd (joueur = 1 / observateur = 2) _ 1 pour inscription / 2 sinon ");
      exit(1);
    }
    client(atoi(argv[1]), argv[2], argv[3], atoi(argv[4]), atoi(argv[5]));

    return 0;
  }
