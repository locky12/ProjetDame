#include <ncurses.h>
#include  <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <rpc/des_crypt.h>

#include <sys/types.h>
#include <sys/socket.h>


#include "login.h"
#include "SQlite.h"
// int main(void)
// {
  //
  //   char salt[] = "fc£ed3?fB6W9.Fd$v3f1fùr";
  //   char key[] = "Locky1234";
  //   inscription();
  //
  //   return 0;
  // }
  //


  void crypt_pwd(char * pwd, char * pwdcrypt) {
    int ErrMsg;

    char salt[] = "fc£ed3?fB6W9.Fd$v3f1fùr";
    des_setparity(pwd);
    ErrMsg = ecb_crypt(pwd, salt,24,DES_ENCRYPT);
    if(ErrMsg < 1){
      perror("ecb_crypt()");
      exit(1);
    }
    strcpy(pwdcrypt,salt);
    //sprintf(returnPwd,salt);
    //printf("mot de passe : %s\n",pwdcrypt);
    //return returnPwd;
  }
  void decrypt_pwd(char * pwd, char * salt) {
    int ErrMsg;
    //char salt[] = "fc£ed3?fB6W9.Fd$v3f1fùr";
    des_setparity(pwd);
    ErrMsg = ecb_crypt(pwd, salt,24,DES_DECRYPT);
    if(ErrMsg < 1){
      perror("ecb_crypt()");
      exit(1);
    }
    //printf("mot de passe : %s\n",salt);
  }
  /*la fonction permet de faire une saisie clavier */
  /*elle prend en paramètre un buffer , la taille du buffer, un texte à afficher,  */
  /*et une option 0 ou 1 Si l'option est à 1 echo est désactivé pour sasir un mot de passe. */
  void saisie_login (char *buffer, int sizeBuffer, char * text, int option) {
    int i = 0;
    char err [100];

    do {
      i = 0;
      printf(" %s : ",text);
      if (option) {
        system("stty -echo");
      }
      if(fgets(buffer,100 , stdin) != NULL){
        if (option) {
          system("stty echo");
        }
        buffer[strlen(buffer)-1] = '\0';
        if (strlen(buffer) > sizeBuffer) {
          memset(buffer,'\0',strlen(buffer));
          i = 1;
        }
        else {
          return;
        }
      }
      else {
        i = 1;
      }
      printf("est incorrect\n");
    }while(i == 1);
  }
  //Concatene dans un style precis pour l'envoyer au serveur et savoir comment traiter cette requete
  //prend le pseudo et le mot de passe à envoyer
  // et une option 1 pour inscription et 2 pour connexion.
  char * catQuery(char * pseudo, char * mdp, int observer, int option) {
    char * buffer = malloc((strlen(pseudo)*strlen(mdp)+3)*sizeof(buffer));
    if(buffer == NULL) {
      perror("malloc()");
    }
    sprintf(buffer,"%d/%s/%s/%d", option,pseudo,mdp,observer);
    printf("%s", buffer);
    return buffer;
  }


  char ** read_query_log (char * buffer, char ** result) {
    int       i         = 0,
    r         = 0,
    count     = 0,
    ok        = 1;
    const int sizeAlloc = 4;
    // char ** result;
    char      temp [100];
    //char          **result;

    result = malloc(sizeAlloc * sizeof(*result));
    if(result == NULL){
      perror("malloc()");
      exit(1);
    }
    printf("ant decomposition : %s\n",buffer );
    while(ok){
      printf("char : %c\n",buffer[i] );
      if(buffer[i] != '/' && buffer[i] != '\0'){
        temp[r] = buffer[i];
        i++;
        r++;
      }
      else {
        puts("passe dans le else");
        i++;
        result[count] = malloc((strlen(temp) + 1)*sizeof(result));
        if(result[count] == NULL){
          perror("malloc()");
          exit(1);
        }
        if (buffer[i] == '\0') {
          ok = 0;
        }
        printf("%s\n",temp );
        strcpy(result[count],temp);
        count++;
        r = 0;

        memset(temp,'\0',strlen(temp));
      }
    }

    //printf("result 3 : %s\n",result[3] );
    return result;
  }


  int sign_player ( char * pseudo, char * pwd) {
    puts("open sign_player ********");
    sqlite3 *db;
    db = ouvrir_db();
    puts("open sign_player");
    write_data_player(db,pseudo,pwd);
    puts("end sign_player");
    read_data(db,"Select * from Joueur");
    sqlite3_close(db);
    return 1;
  }
  int log_player (char * pseudo, char * pwd){
    puts("log player*******");
    sqlite3 *db;
    db = ouvrir_db();
    char *tempPwd = malloc(100);
    tempPwd = controlMDP(db,pseudo,tempPwd);
    // if( ){
      printf("tempPWD : %s\n",tempPwd );
      printf("tempPWD : %s\n",pwd );

    if (tempPwd != NULL &&strcmp(tempPwd,pwd) == 0){
      sqlite3_close(db);
      return 1;
    }
  // }
    sqlite3_close(db);
    return 0;
  }
  char * control_connect (ArrayRoom *array_room,ListObserver *  list, int sock) {
    puts("debut control connect");
    int verif = 0;
    char pseudo[20];
    char buffer [500];
    char ** result;
    // printf("sock reception : %d\n",sock);

    read_msg (sock, buffer);

    result = read_query_log (buffer, result);

    memset(buffer,'\0', 100);
    printf("option : %d\n", atoi(result[0]) );
    if(atoi(result[0]) == 2) {
      sign_player(result[1],result[2]);
      sprintf(buffer,"%d/%s",0,"Ok");
      send_msg(sock,buffer);


    }
    else if (atoi(result[0])== 1 ) {
      sprintf(buffer,"%d/%s",2,"sign_ok");
      verif = log_player(result[1], result[2]);
      printf("verif log player : %d\n",verif );
      if(atoi(result[3]) == 1 && verif == 1) {
        puts("ajout joueur");
        add_client_array (array_room,sock,result[1]);
        send_msg(sock,"ok");
        return "";
      }
      else if (atoi(result[3]) == 1 && verif == 1){
        puts("ajout observateur");
        add_list_observer (list,sock,result[1]);
        puts("fin ajout observateur");
        send_msg(sock,"ok");
        return "";
      }
      else {
        sprintf(buffer,"%d/%s",0,"failed");
        send_msg(sock,buffer);
        printf("%s\n","refus" );
      }

    }
    else if (atoi(result[0]) == 3){
      if(atoi(result[3]) == 1) {
        puts("ajout joueur");
        add_client_array (array_room,sock,"invite");
      }
      else {
        puts("ajout observateur");
        add_list_observer (list,sock,"invite");
        puts("fin ajout observateur");
        return "";
      }
    }
    else {
      sprintf(buffer,"%d/%s",0,"failed");
      send_msg(sock,buffer);
      printf("%s\n","refus" );
      return "";
    }

  }

  int connexion_client (int sock, char *pseudo, char *pwd, int observer, int inscription) {
    char *buffer;
    buffer = catQuery(pseudo,pwd,observer, inscription);
    send_msg (sock, buffer);
    puts("message envoyé");
  }
  int add_observer_array (ArrayRoom * array_room,int sock ,char* pseudo){
    puts("add_observer_array");
    for (int i = 0; i < 10; i++){
      if(array_room->array[i].sizePlay < 4 && array_room->array[i].haveObserver == 0 /*&& array_room->array[i].inGame == 0*/ ) {
        printf("ajout observateur a l'indice : %d\n",i );
        array_room->array[i].sizePlay++;
        array_room->array[i].play[2] = add_client(array_room[i].array[i].play[2], sock, pseudo,2);
        printf("%s ****\n", array_room->array[i].play[2].pseudo);
        puts("midle_add_observer_array");

        array_room->array[i].haveObserver = 1;

        break;
      }
    }
    puts("fin _add_observer_array");
  }
  void copy_observer_array(ArrayRoom * array_room,ListObserver * list, int indRoom, int indList){
    strcpy(array_room->array[indRoom].play[2].pseudo,list->array[indList].pseudo);
    array_room->array[indRoom].play[2].socket = list->array[indList].socket;
    array_room->array[indRoom].play[2].observer = list->array[indList].observer;
    memmove(&list->array[indList],&list->array[indList+1],10);
    list->size --;
    for(int i = 0; i < 10 ; i++){
      printf("pseudo[%d] : %s\n",i, list->array[i].pseudo);
    }
  }
  void add_list_observer (ListObserver * list, int sock ,char* pseudo){
    sprintf(list->array[list->size].pseudo,"%s",pseudo);
    list->array[list->size].socket = sock;
    list->array[list->size].observer = 2;
    list->size++;
    for(int i = 0; i < 10 ; i++){
      printf("pseudo[%d] : %s\n",i, list->array[i].pseudo);
    }
  }
  int * index_observer(int * tab,ListObserver * list){
    fd_set fd;
    int max = 3;
    int i = 0;
    int choose;
    char buffer[100];

    FD_ZERO(&fd);
    //FD_SET(socket,&fd);
    puts("crash");
    printf("taille liste : %d\n", list->size );
    for(i = 0; i < list->size ; i++ ){
      puts("crash2");
      FD_SET(list->array[i].socket,&fd);
      puts("crash3");
      if(list->array[i].socket > max)
      {
        max = list->array[i].socket;
      }
    }
    if(select(max + 1,&fd,NULL,NULL,NULL) < 0)
    {
      perror("select");
      exit(EXIT_FAILURE);
    }
    for(i = 0; i < 10; i++ ){
      if (FD_ISSET(list->array[i].socket,&fd)) {
        read_msg(list->array[i].socket,buffer);
        break;
      }
    }
    printf("buffer == == -- %s\n",buffer );
    if(charinInt(buffer[0] == '6' )){
      choose = charinInt(buffer[2]);
      printf("choose buffer == == == %c\n",buffer[2] );
      if(choose > 0 && choose < 10){
        // if(array_room->array[choose].play[2].observer == 0 && array_room->array[choose].inGame == 1){
          //   //array_room->array[choose].play[2] = list[i];
          //   // TODO :_copie l'observateur
          // }
          printf("choose :: :: '%d'\n",choose );
          tab[0] = choose;
          tab[1] = i;

        }
      }
      printf("index tab dans fct : %d,%d\n",tab[0],tab[1] );
      return tab;
    }

    void send_list_room (ArrayRoom * array_room, ListObserver * list){
      char buffer[100] = {0};

      int count = 0;
      int i = 0;
      for(i = 0; i < 10; i++ ){
        if(array_room->array[i].inGame == 1 && array_room->array[i].play[2].observer == 0 ){
          sprintf(buffer,"%s/%d",buffer,i);
          count++;
        }
      }
      sprintf(buffer,"%s/%d*",buffer,count);
      for(i = 0; i < list-> size; i++){
        send_msg(list->array[i].socket,buffer);
        memset(buffer,'\n',100);
      }

    }
    void choose_game_observer(int * list_room,int socket){
      int i = 0;
      char  buffer[20]={0};
      char bufferSend[20] = {0};
      puts("Choix d'une partie a observer : ");

      while(list_room[i] != -1){
        printf("%d : La partie %d peut etre observer\n",i+1,list_room[i]);
        i++;
      }

      do{
        scanf("%s",&buffer);
      }while((atoi(buffer) < 1 && atoi(buffer) >= i) || buffer[0] == 'a');
      sprintf(bufferSend,"%d/%s",6,buffer);
      send_msg(socket,bufferSend);
    }
    int * read_list_game(char * buffer){
      int count = 0;
  int * result;
  int size;
  int index = 2;// les 2 premier ne sont pas des indices mais id du message
  int posArray = 0;
  char  temp [100];
  // On cherche la fin du message envoyee represente par une '*'
  puts("je crash");
  while(buffer[count] != '*'){
  count++;
  }
  puts("je crash2");
  // on alloue un tableau avec la taille recu dans le message
  size = charinInt(buffer[count]);
  result = malloc(10 * sizeof(result));
  puts("je crash3");
  if(result == NULL){
    perror("malloc()");
    exit(EXIT_FAILURE);
  }
  count = 0;
  // On va récupérer tous les indices de parties envoyée par le serveur
  while(buffer[index] != '*'){
    // tant que c'est different de / je mets dans temp
    if(buffer[index] != '/'){
      temp[count] = buffer[index];
			count++;
    }
    // a chaque fois que je vois un / je mais le buffer dans le tableau
    if(buffer[index] == '/'){
      printf("temp lecture : %s\n",temp );
      result[posArray] = atoi(temp);
      printf("result[posArray]  : %d\n",result[posArray]  );
      posArray++;
			temp[0] = 0;
			temp[1] = 0;
      count =0;
    }
    index++;
  }
  // car je connais pas la taille ça me sert a le lire
  for(int i = posArray; i<10 ;i++){
    result[i] = -1;
  }
  return result;
    }

    int add_client_array (ArrayRoom * array_room,int sock ,char* pseudo){
      puts("add_client_array");
      int control = 0;
      for (int i = 0; i < 10; i++){
        if(array_room->array[i].inGame == 0){
          for(int j = 0; j < 2; j++){

            if(array_room->array[i].play[j].observer == 0){
              printf("++++ insertion en i = %d, en j = %d ++++",i,j);
              array_room->array[i].play[j] = add_client(array_room->array[i].play[j], sock, pseudo,1);
              array_room->array[i].sizePlay++;
              control = 1;
              break;
            }
          }
        }
        if(control){
          break;
        }
      }
    }
    Player add_client (Player  player, int socket, char * pseudo, int observer){
      puts(pseudo);
      puts("add_client");
      sprintf(player.pseudo,"%s",pseudo);
      player.socket = socket;
      player.observer = observer;
      printf("pseudo : %s \n",player.pseudo );
      printf("socket %d \n", player.socket );
      printf(" observer %d\n", player.observer );
      puts("fin_add_client");
      return player;
    }
    void send_msg(int socket, char *buffer)
    {
      int sizeSend;
      sizeSend = send( socket, buffer, strlen(buffer), 0);
      if (sizeSend == -1)
      {
        perror("send()");
      }
      printf("msg : %s + %d\n",buffer,sizeSend );
    }

    void read_msg(int socket, char *buffer)
    {
      ssize_t sizeRecv;
      printf("aille buffer : %ld\n", strlen(buffer) );
      sizeRecv = recv(socket, buffer, 100,0);
      if (sizeRecv == -1){
        perror("recv()");
      }
      printf("msgrecu : %s + %ld\n",buffer, sizeRecv );
    }
    void saisie(char *buffer){
      puts("dans la saisie");
      memset(buffer,'\0',sizeof(buffer));
      fgets(buffer,100,stdin);
      printf("saisie : %s\n",buffer );
    }
    int charinInt (char c) {
      return c - '0';
    }
