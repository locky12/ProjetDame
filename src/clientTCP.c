
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include <netdb.h>



#define TAILLE_SAISIE 500
typedef struct in_addr IN_ADDR;


void ecrire_serveur(int socket, char *buffer)
{
  int taille_envoyee;
  taille_envoyee = send( socket, buffer, strlen(buffer), 0);
  if (taille_envoyee == -1)
  {
    perror("send()");

  }
}

void lire_serveur(int socket, char *buffer)
{
  ssize_t taille_recue;
  taille_recue = recv(socket, buffer, strlen(buffer),0);
  if (taille_recue == -1){
    perror("recv()");
  }
}


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
  void client(int port)
  {
    ssize_t taille_recue;
    char buffer_message[200];
    fd_set rd;
    char buffer_saisie[200];
    int socket_connexion = connexion(port);
    while(1)
    {


      memset(&buffer_message,'\0',sizeof(buffer_message));
      FD_ZERO(&rd);
      FD_SET(STDIN_FILENO,&rd);
      FD_SET(socket_connexion,&rd);
      select(socket_connexion + 1,&rd,0,0,0);
      if (FD_ISSET(STDIN_FILENO,&rd))
      {

        saisie(buffer_saisie);
        ecrire_serveur(socket_connexion,buffer_saisie);
        if(strncmp(buffer_saisie,"quit",4)==0)
        {
          close(socket_connexion);
          exit(1);
        }

      }
      else if (FD_ISSET(socket_connexion,&rd)) {
        taille_recue = recv(socket_connexion, buffer_message, sizeof(buffer_message),0);
        if (taille_recue == -1){
          perror("recv()");
          exit (-1);
        }
        else {printf(": %s", buffer_message );}

      }
    }
    close(socket_connexion);

  }



  int main (int argc, char **argv)
  {
    if(argc != 2) {
      printf("Le port doit etre passer en parametre");
    }
    client(atoi(argv[1]));

    return 0;
  }
