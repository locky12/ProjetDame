#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>



int lire_client(int socket_client, char *buffer_message)

{
  ssize_t		taille_recue;
  memset( buffer_message, '\0', sizeof( buffer_message ) );
  taille_recue = recv( socket_client, buffer_message, 128, 0);
  if ( taille_recue == -1 ){
    perror ("recv()" );
    exit( -1 );
  }
}

void envoyer_client(int socket_client,char *buffer_message)
{
  int taille_envoyee;
  taille_envoyee = send( socket_client, buffer_message, strlen(buffer_message), 0);
  if (taille_envoyee == -1)
  {
    perror("send()");
    exit(-1);
  }
}

void renvoyer_client(int *client,int client_parle ,char *buffer,int n)
{
  int i;
  for ( i = 0; i < n; i++)
  {
    if (client[i] != client_parle)
    {
      envoyer_client(client[i],buffer);
    }
  }
}

int connection (int port)
{
  int	socket_ecoute;			/* Socket d'écoute */
  struct sockaddr_in	adresse_ecoute;			/* Adr locale ou le server ecoute */
  socklen_t		taille;				/* Taille pour accept() */

  puts(" -- Server lancé -- \n");

  /* Creation de la socket; AF_INET	->  IPv4; SOCK_STREAM	->  TPC	*/
  if( ( socket_ecoute = socket( AF_INET, SOCK_STREAM, 0 ) ) == -1){
    perror("socket()");
    exit(-1);
  }

  memset(&adresse_ecoute, 0, sizeof(adresse_ecoute));
  adresse_ecoute.sin_family = AF_INET;
  adresse_ecoute.sin_port = htons( port );
  adresse_ecoute.sin_addr.s_addr = 0;

  if(	bind(socket_ecoute,(struct sockaddr *) &adresse_ecoute,
  sizeof(adresse_ecoute)) == -1)
  {
    perror("bind()");
    exit(-1);
  }

  if( listen( socket_ecoute, 3) == -1 ){
    perror( "listen()" );
    exit(-1);
  }
  return socket_ecoute;
}

/* Fonction qui gere les connections / transmission de message */

void serveur1(int port)

{
  int         socket_ecoute = connection(port);// variable qui prend le socket d'ecoute.
  int			    socket_client[10];			/* Socket pour traiter le client */
  int         max = socket_ecoute;
  struct      sockaddr_in	adresse_client;			/* Adresse du client connecte */
  char		    buffer_adresse[128];		/* Buffer pour l'adresse du client */
  socklen_t		taille;				/* Taille pour accept() */
  ssize_t		  taille_recue;			/* Quantite d'octets reçue */
  int         taille_envoyee;

  fd_set rd;
  taille = sizeof(struct sockaddr_in);

  char buffer_message[128];       /* Buffer pour mettre le message reçu et l'afficher */
  int i = 0;
  int nbrCLIENT = 0;
  int socket;
  int c;
  int client;

  socklen_t taille1 = sizeof(adresse_client);
  /* On traite nos clients dans une boucle infinie */

  while( 1 ){
    printf("socket ecoude : %d  \n",socket_ecoute);

    FD_ZERO(&rd);// ecrase le descripteur
    FD_SET(STDIN_FILENO, &rd);// initialise le descripteur sur une entré clavier
    FD_SET(socket_ecoute,&rd);// initialise le descripteur avec le socket écoute

    for ( i = 0; i < nbrCLIENT; i++)// boucle qui gère les client
    {
      FD_SET(socket_client[i],&rd);//pour chaque client on initialise le fichier descripteur
    }

    if (select(max + 1,&rd,NULL,NULL,NULL ) == -1)//on test l'ensemble des descripteur en lecture
    {
      perror("select()");
      exit(-1);
    }

    else if (FD_ISSET(socket_ecoute, &rd))// test si le socket ecoute à changer
    {

      socket =  accept(socket_ecoute, (struct sockaddr *) &adresse_client
      , &taille1);

      if ( socket== -1 ){
        perror( "accept()" );//accepte renvoie un descripteur
        exit( -1 );
      }
      max = socket > max ? socket : max;
      printf(	    "Un client s'est connecté ... %s : %d\n", inet_ntop(AF_INET
        ,	&(adresse_client.sin_addr),	buffer_adresse,	128 ), adresse_client.sin_port);
        FD_SET(socket, &rd);// on zjoute socket dans rd

        //  c =  csock ;
        socket_client[nbrCLIENT] = socket;// le nombre de client est égales on nombre de descripteur dans l'ensemble
        nbrCLIENT++;
      }
      else // si y a pas de demande de connection
      {
        for ( i = 0; i < nbrCLIENT; i++)// on boucle tout les clients
        {
          if (FD_ISSET(socket_client[i], &rd))// si un client demande à parler
          {
            memset(buffer_message,'\0',sizeof(buffer_message));
            int client_parle = socket_client[i];
            taille_recue = lire_client(socket_client[i],buffer_message);
            printf(  "Message reçu (taille %ld):/n %s\n", taille_recue, buffer_message );
            if (strncmp(buffer_message, "quit", 4) == 0) {
              close(socket_client[i]);
              exit(1);
            }
            renvoyer_client(socket_client,client_parle,buffer_message,nbrCLIENT);
        }
      }
    }
  }
}
void deconnection(int sock_client)
{
  close(sock_client);
}


int main(int argc, char *argv[]) {
  if(argc != 2) {
    printf("Le port doit etre passer en parametre");
  }
  serveur1(atoi(argv[1]));
  return 0;
}
