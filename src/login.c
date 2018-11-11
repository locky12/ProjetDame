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

// TODO : associer
// void readQueryLog(char * pseudo, char * mdp ) {}
/* fonction quipermet de s'inscrire  */
/* appel 2 fois la fonction saisie */
/* script le mot de passe */
//TODO : appeller sur le client et envoyee les info au serveur.
void inscription (int option){


  char pseudo[20];
  char mdp [20];
  char pwdcrypt[25];
  int taille_inscription = 15;
  char  *test = malloc(100 * sizeof(test));
  char ** test2;

  saisie_login (pseudo,taille_inscription, "Votre pseudo", option);
  printf("%s\n",pseudo);
  saisie_login (mdp,taille_inscription, "Votre mot de passe", option);
  printf("***********\n");
  crypt_pwd(mdp,pwdcrypt);

  printf("mdp  : %s **** \n",pwdcrypt );

  //decrypt_pwd(mdp,pwdcrypt);
  // test = catQuery(pseudo,pwdcrypt,0);
 printf("test : %s ****\n",test );
  // test2 = read_query_log (test,test2);
  printf("test[0] : %s : %ld /*\n",test2[0],strlen(test2[0]));
  printf("test[1] : %s  /*\n",test2[1]);
  printf("test[2] : %s  /*\n",test2[2]);

  //TODO : envoyer les données au serveur.
  //saisie_mdp(mdp, taille_inscription);
}
int sign_player ( char * pseudo, char * pwd) {
  puts("open sign_player ********");
  sqlite3 *db;
  db = ouvrir_db();
  puts("open sign_player");
  write_data_player(db,pseudo,pwd);
  puts("end sign_player");
  read_data(db,"Select * from Joueur");
  return 1;
}
int log_player (char * pseudo, char * pwd){
  puts("log player*******");
  sqlite3 *db;
  db = ouvrir_db();
  char *tempPwd;
  tempPwd = retrieve_pwd(db,pseudo);
  if (strcmp(tempPwd,pwd) == 0){
    return 1;
  }
  return 0;
}
char * control_connect (ArrayRoom *array_room, int sock) {
  puts("debut control connect");

  char pseudo[20];
  char buffer [500];
  char ** result;
  // printf("sock reception : %d\n",sock);

  read_msg (sock, buffer);
  // printf("buffer : %s\n", buffer);
  // send_msg(sock, "lis ça ");
  result = read_query_log (buffer, result);
  puts(result[1]);
  puts(result[2]);
  // puts("bug ou pas bug");

 memset(buffer,'\0', 100);
  printf("option : %d\n", atoi(result[0]) );
  if(atoi(result[0]) == 1) {
    // puts("dans inscription");
    // puts("avant log player");
    sign_player(result[1],result[2]);
    sprintf(buffer,"%d/%s",1,"connect_ok");
  //  send_msg(sock,buffer);
    printf("%s\n", "connect_ok");

    return result[1];
  }
  else if (atoi(result[0])== 2) {
    sprintf(buffer,"%d/%s",2,"sign_ok");
    log_player(result[1], result[2]);
    //send_msg(sock,buffer);
    if(atoi(result[3]) == 1) {
      puts("ajout joueur");
      add_client_array (array_room,sock,result[1]);
    }
    else {
      puts("ajout observateur");
      add_observer_array (array_room,sock,result[1]);
      puts("fin ajout observateur");
      return "";
    }

  }
  else if (atoi(result[0]) == 3){
    if(atoi(result[3]) == 1) {
      puts("ajout joueur");
      add_client_array (array_room,sock,"invite");
    }
    else {
      puts("ajout observateur");
      add_observer_array (array_room,sock,"invite");
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
    if(array_room->array[i].sizePlay < 3 && array_room->array[i].haveObserver == 0 && array_room->array[i].inGame == 0 ) {
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
// int add_client_array (ArrayRoom * array_room,int sock ,char* pseudo){
//   puts("add_client_array");
//   for (int i = 0; i < 10; i++){
//     if(array_room->array[i].sizePlay < 2) {
//       printf("insertion joueur %d ++++++++++++++++++++++++++++++++++\n",i );
//     array_room->array[i].play[array_room->array[i].sizePlay]  = add_client(array_room->array[i].play[array_room->array[i].sizePlay], sock, pseudo,1);
//       array_room->array[i].sizePlay++;
//       printf("***************  %d ******************\n",array_room->array[i].sizePlay );
//       break;
//     }
//     else if ( (array_room->array[i].sizePlay < 3 &&array_room->array[i].haveObserver == 1) ){
//       array_room->array[i].play[array_room->array[1].sizePlay]  = add_client(array_room->array[i].play[array_room->array[i].sizePlay], sock, pseudo,1);
//         array_room->array[i].sizePlay++;
//     }
//   }
// }

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
