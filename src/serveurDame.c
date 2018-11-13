#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <signal.h>
#include <fcntl.h>
#include "login.h"
#include "struct.h"
#include "deplacement.h"
#include "affichage.h"
//#include "serveurDame.h"




//Prototypes

// Permet de lire un message d'un client
int read_player(int socket_player, char *buffer);
// Permet d'envoyer un message à un client
void send_player(int socket_player,char *buffer);
// Permet d'envoyer un message à plusieurs clients.
void send_all_player(Player *list,int playerSend ,char *buffer,int size);
// Accepte un client.
int accept_player (int socket_ecoute);
// Permet de lier l'adresse serveur au socket d'écoute
// retourne le socket_ecoute.
int listen_net (int port);
void server(int port);
void * room_play_thread (void * args);
void prepare_exit_thread(Room *room);
Player delete_player_room(Player player);
void add_old_player (ArrayRoom *array_room, Room room);
void reset_array_play (ArrayRoom *array_room, Room room);
void send_move (Move * move, int ** arrayCapture, int size,int option, char * buffer  );
void send_number_player (Player *list);
void send_round_player (int socket);
int control_connect_player (Room *room);
char ** read_move_query (char * buffer);
int charInInt (char c);


int main(int argc, char **argv) {
  server(atoi(argv[1]));
  return 0;
}

void printPlayer (Player player){
  printf("le pseudo du joueur : %s\n",player.pseudo);
  printf("Le socket du joueur : %d\n",player.socket);
  printf("Le socket du joueur : %d\n",player.observer);
}

ArrayRoom* init_arrayRoom() {
  ArrayRoom   *array_room = malloc(sizeof(array_room));
  array_room->sizeArray =1;
  array_room->array = malloc(10 * sizeof(Room));
  for(int j = 0; j < 10; j++ ) {
    array_room->array[j].sizePlay = 0;

    printf("init : %d \n",array_room->array[j].sizePlay );
    array_room->array[j].play = malloc(3*sizeof(Player));
    if(array_room->array[j].play == NULL){
      perror("malloc()");
      exit(1);
    }
    else {

    }
  }

  return array_room;
}
void search_array (ArrayRoom *array_room) {
  int i = 0,
  j = 0;
  int size = array_room->sizeArray;
  int index;
  for(i = 0; i < 10; i++) {
    printf("****** le nombre player est : %d ******\n",array_room->array[i].sizePlay );
    if(array_room->array[i].sizePlay < 2 || (array_room->array[i].sizePlay < 3  && array_room->array[i].haveObserver == 1 )) {
      printf("****** index insertion est : %d ******\n",i );
      array_room->indexFree = i;
      return;
    }

  }
  //}
}

int tri_rooms (ArrayRoom *array_room) {

}
// int add_player_array(ArrayRoom *array_room,int socket) {
//   //read
//   int observer = 1;
//   int sizeP = 0;
//   int indexF = array_room->indexFree;
//   printf("index insertion :: %d\n",indexF );
//   sizeP = array_room ->array[indexF].sizePlay;
//   printf("taille :: %d\n",array_room ->array[indexF].sizePlay);
//   if(observer == 1){
//     sprintf(array_room->array[indexF].play[sizeP].pseudo,"bob" );
//     // array_room->array[indexF].play[sizeP].pseudo = "bob";
//     array_room->array[indexF].play[sizeP].socket = socket;
//     array_room->array[indexF].play[sizeP].observer = 44;
//     array_room->array[indexF].sizePlay = sizeP + 1;
//     printf("taille :: %d\n",array_room->array[indexF].sizePlay );
//     if (array_room->array[indexF].sizePlay == 2) {
//       search_array (array_room);
//       printf("%s\n","la ** la" );
//       return 1;
//     }
//
//   }
//   else if(observer == 2){
//     return 0;
//   }
//   return 0;
// }

Room * goGame (ArrayRoom * array_room){
  //printPlayer(array_room->array[0].play[1]);
  for (int i = 0; i < 10; i++){
    if(array_room->array[i].sizePlay == 3 || (array_room->array[i].sizePlay == 2 && array_room->array[i].haveObserver == 0)) {

      if(array_room->array[i].inGame == 0 && control_connect_player(&array_room->array[i])) {

        array_room->array[i].inGame = 1;
        return &array_room->array[i];
      }
    }
  }
  return NULL;
}
int control_connect_player (Room *room){
  char buffer[100];
  int count = 0;
  for(int i = 0; i < room->sizePlay; i++){
    send_player(room->play[i].socket,"8");
    read_player(room->play[i].socket,buffer);
    if(buffer[0] == '9'){
      count++;
      if(count == 2){
        return 1;
      }
    }
    else {
      delete_player_room(room->play[i]);
      room->sizePlay -= 1;
      return 0;
    }
    memset(buffer,'\0',100);
  }


  return 0;
}
/***** ********* Serveur ***************/
void server (int port) {
  //variable :
  int         socketServer    = 0,
  socketPlayer    = 0,
  count           = 0,
  indexThread     = 0,
  countPlay       = 0,
  flag            = 0,
  verification    = 0,
  index           = 0;
  char buffer [500];
  Player      player = {0,0,""};
  pthread_t threadRoom[10];
  // tempPlayer;
  Room        *room;
  /*Alloue un tableau de salon et des salons avec 3 joureur*/
  ArrayRoom *array_room = init_arrayRoom();
  socketServer = listen_net(port);
  if(array_room->array[0].sizePlay == 0){
  }


  // permet de rendre la socket d'ecoute non bloquante
  flag = fcntl(socketServer,F_GETFL,0);
  if (!(flag& O_NONBLOCK)) {
    flag |= O_NONBLOCK;
  }
  fcntl(socketServer, F_SETFL, flag);
  //*****
  int observer = 1; // a virer quand la connexion sera ajouté


  while (1) {

    int test = 0;
    index = array_room->indexFree;
    socketPlayer = accept_player(socketServer); // on passe dans accept

    //add_player_array(array_room, socketPlayer);


    if(socketPlayer != 0){ //si accept retourne une socket valideS
      printf("socket ++++++ %d\n",socketPlayer );
      sleep(1);
      control_connect(array_room,socketPlayer);
      printPlayer(array_room->array[0].play[0]);
    }
    // sprintf(array_room->array[9].play[0].pseudo, "bbb");
    // printPlayer(array_room->array[9].play[0]);

    // on ajoute un joueur si la fonction returne 1 une partie peut être lancée
    room =  goGame(array_room);
    if(room != NULL) {
      count++;
      room->index = index;// on récupère l'index du thread pour la sortie
      //room = &array_room->array[index];// on cast le salon en un pointer de salon
      room -> socketServer = socketServer;
      pthread_create(&threadRoom[indexThread], NULL, room_play_thread, room);// on crée le thread

      indexThread++;// le prochain thread crée aura un indice +1;
    }

    else {
      void * arg;
      Room tempRoom;
      //printf("%d\n",test );
      if(count > 0) {
        for (int i = 0; i < indexThread; i++  ){
          if(verification = pthread_tryjoin_np(threadRoom[i], &arg) == 0){
            tempRoom = *(Room*)arg;
            add_old_player (array_room, tempRoom);
            //printPlayer(room[0]-> play[count]);
          }
          //printf("verification : %d \n", verification);
        }
      }


    }
    sleep(1);
  }

}
void add_old_player (ArrayRoom *array_room, Room room){
  int size =  array_room->array[array_room->indexFree].sizePlay;
  printf("add_old_player() : %d\n",size);
  for (int i = 0; i < 3; i++) {
    printf("pseudo : %s\n",room.play[i].pseudo );
    printf("socket : %d\n",room.play[i].socket );
    printf("observateur : %d\n",room.play[i].observer);
    puts("***************************");
  }
  puts("$$$$$$$$$ old $$$$$$$$$$$");
  reset_array_play(array_room, room);
  for(int i = 0; i < size; i++){
      printf("room.play[i].observer : %d\n",room.play[i].observer );
      if(room.play[i].observer == 1){
        puts("ajout d'un acien joueur");
        add_client_array (array_room,room.play[i].socket ,room.play[i].pseudo);
      }
      else if (room.play[i].observer == 2)
      {
        add_observer_array (array_room,room.play[i].socket ,room.play[i].pseudo);
      }
    }

}
void reset_array_play (ArrayRoom *array_room, Room room) {
  int index = room.index;
  array_room->array[index].sizePlay = 0;
  array_room->array[index].haveObserver = 0;
  array_room->array[index].index = 0;
  array_room->array[index].inGame = 0;
  // for(int i = 0; i< 3 ; i++ ){
  //   array_room->array[index].play[i].observer = 0;
  // }

}
/**************************************************************************/
void * room_play_thread (void * args)
{
  printf("%s\n","room_play_thread" );
  fd_set rd;
  int control = 1;
  char buffer[500];
  int socket;
  int size = 2;
  Room *room = args;
  int i, max = room-> socketServer;
  int otherPlayer = 0;
  Damier damier[10][10];
  initialiseDamier(damier);
  int joueurActuel = 1;
  sleep(5);
  send_number_player(room-> play);
  send_round_player(room->play[0].socket);
  //room-> play = args;

  do
  {
    sleep(1);
    if(room->play[2].observer == 2)
    {
      puts("un observateur est entrée");
      size = 3;
      sleep(1);
    }

    FD_ZERO(&rd);
		FD_SET(room ->socketServer,&rd);
    for(int k = 0 ; k < 2 ; k++)
    {
      FD_SET(room-> play[k].socket, &rd);
      if(room-> play[k].socket > max)
      {
        max = room-> play[k].socket;
      }
    }
    printf(" *** select 1  +++++\n" );
    if(select(max + 1,&rd,NULL,NULL,NULL) < 0)
    {
      perror("select");
    }
    printf(" *** select 2+++++\n" );
    for ( i = 0; i < 2; i++ )
    {
      if(FD_ISSET(room-> play[i].socket, &rd))
      {
        joueurActuel = i + 1;
        printf(" **************++++++++++++++++****************\n" );


        control = read_player(room-> play[i].socket, buffer); // fonction a changer par le jeu
        if (control == 0) // il faut qu'elle return 0 si le joueur a déco
        {
          puts("a deco ++----------++");
          room-> play[i].observer =0; // Passe observateur à 0 pour indiquer qu'il a déco.
          break;
        }
        char ** result = read_move_query(buffer);
        afficheDamier(damier);
        printf("%s\n", buffer);
        printf(" result ******* %d : %d : %d : %d : \n", charInInt(result[0][0]), charInInt(result[0][1]), charInInt(result[1][0]), charInInt(result[1][1]));
        int deplacement = deplacementValide(damier, joueurActuel, charInInt(result[0][0]), charInInt(result[0][1]), charInInt(result[1][0]), charInInt(result[1][1]));
        printf("deplacement %d\n", deplacement);
        otherPlayer = (i == 0) ? room-> play[1].socket : room-> play[0].socket;
        if(deplacement)
        {
          effectueDeplacement(damier, joueurActuel, charInInt(result[0][0]), charInInt(result[0][1]), charInInt(result[1][0]), charInInt(result[1][1]));
          send_all_player(room-> play,room-> play[i].socket,buffer,size);
        }
        else
        {
          //send_all_player(room-> play,room-> play[i].socket,"10",size);
        }
        int prise = canTakePion(damier, joueurActuel, charInInt(result[1][0]), charInInt(result[1][1]));
        if(deplacement == 2 && prise)
        {
          printf("rejoue\n");
          // sleep(2);
          send_player(room-> play[i].socket,"3");
          printf("send\n");
          read_player(otherPlayer,buffer);
          printf("read\n");
        }
        else
        {
          deletePion(damier);
          printf("otherPlayer ; %d\n",otherPlayer );
          read_player(otherPlayer,buffer);
          memset( buffer, '\0', sizeof( buffer ) );
          send_player(otherPlayer,"3");
        }

         // TODO :gerer le cas de l'observateur qui déco
        // TODO SI il peut rejouer

      }
        printf("%s\n",buffer );
    }
  }while (control == 1 && !jeuEstFini(damier, joueurActuel%2+1));
  if (control == 1)
  {
    printf(" Le joueur %d a gagné\n", joueurActuel); // TODO
  }
  prepare_exit_thread(room);
  puts("$$$$$$$$$ FIN $$$$$$$$$$$");
  puts("$$$$$$$$$ FIN $$$$$$$$$$$");
  puts("fin thread");

  pthread_exit(room);
}

void prepare_exit_thread(Room *room){
  int   i,
  count = 0;
  for (i = 0; i < room->sizePlay; i++) {
    printf("pseudo avant deplacement%s + %d\n",   room->play[i].pseudo,i);
    printf("observer avant deplacement%s+ %d\n",   room->play[i].pseudo,i);
    if(room->play[i].observer == 0){
      printf("i = %d\n",i );
      room->play[i] = delete_player_room(room->play[i]);
      if(i == 0) {
        puts("count++");
        count ++;
      }
    }
    if(count == 1 && room->play[i].observer != 2){
      // sprintf(room->play[i-1].pseudo,"%s",room->play[i].pseudo);
      // room->play[i-1].socket = room->play[i].socket;
      // room->play[i-1].observer = room->play[i].observer;
      printf("pseudo avant deplacement %s\n",   room->play[i].pseudo);
      room->play[i-1] =  room->play[i];
      room->play[i] = delete_player_room(room->play[i]);
    }


  }
  room->sizePlay -=count;
  printf("pseudo après deplacement%s\n",   room->play[0].pseudo);
}

void send_round_player (int socket){
  send_player(socket,"3");
}
Player delete_player_room(Player player) {
  memset(player.pseudo, '\0', 30);
  player.socket = 0;
  player.observer = 0;
}


int accept_player (int socket_ecoute) {
  struct      sockaddr_in	adresse_client;
  socklen_t   taille = sizeof(adresse_client);
  char		    buffer_adresse[128];
  int socket;

  socket = accept(socket_ecoute, (struct sockaddr *) &adresse_client
  , &taille);
  if (socket == -1 ){
    return 0;
  }
  printf(	    "Un client s'est connecté ... %s : %d\n", inet_ntop(AF_INET
    ,	&(adresse_client.sin_addr),	buffer_adresse,	128 ), adresse_client.sin_port);


    return socket;
  }

  int listen_net (int port)
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

  /* fonction qui appel recv permet de recevoir des message d'un client */
  int read_player(int socket_player, char *buffer)

  {
    puts("dans lire données");
    ssize_t		taille_recue;
    memset( buffer, '\0', sizeof( buffer ) );
    taille_recue = recv( socket_player, buffer, 128, 0);
    if ( taille_recue == -1 ){
      perror ("recv()" );
      return 0;
    }
    if (taille_recue == 0) {
      printf("%s\n","un client a deco" );
      return 0;

    }
    return 1;

  }
  /**/
  /*Fonction qui permet d'envoyer un message à un client */
  void send_player(int socket_player,char *buffer)
  {
    int taille_envoyee;
    taille_envoyee = send( socket_player, buffer, strlen(buffer)+1, 0);
    if (taille_envoyee == -1)
    {
      perror("send()");
      exit(-1);
    }
  }
  /* Fonction qui permet d'envoyer un message à tous les autres clients */
  /* Prend en parametre une liste de client, le clinet qui envoie, un message,*/
  /* le nombre de client */
  void send_all_player(Player *list,int playerSend ,char *buffer,int size)
  {
    int i;

    for ( i = 0; i < size; i++)
    {
      printf("send all : liste sock : %d\n",list[i].socket );
      if (list[i].socket != playerSend)
      {
        send_player(list[i].socket,buffer);
      }
    }
  }
  // envoie le numéro du joueur 1 ou 2
  void send_number_player (Player *list) {
    char buffer [10];

    for (int i = 0; i < 2; i++)
    {
      sprintf(buffer,"%d/%d",1,i+1);
      printf("buffer numberPlayer ***** %s\n",buffer);
      printf("buffer ; %s\n",buffer );
      send_player(list[i].socket,buffer);
      memset(buffer,'\0',10);
    }
  }


  void send_move (Move * move, int ** arrayCapture, int size,int option, char * buffer  ){
    // char buffer[100] = {0};
    sprintf(buffer,"%d/%d-%d/%d-%d",0,move->position[0],move->position[1],move->newPosition[0],move->newPosition[0]);
    for (int i = 0; i < size; i++){
      sprintf(buffer,"%s/%d-%d",buffer,arrayCapture[i][0],arrayCapture[i][0]);
    }
    sprintf(buffer,"%s/%d",buffer,size);


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
        sprintf(result[count-1],"%c%c",temp[0],c);
        i++;
      }
      else if (buffer[i] == '\0') {
        ok = 0;
      }

    }
    return result;
  }
  int charInInt (char c) {
    return c - '0';
  }
