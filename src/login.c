#include <ncurses.h>
#include  <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <rpc/des_crypt.h>
#include "login.h"
#include "SQlite.h"

int main(void)
{

  char salt[] = "fc£ed3?fB6W9.Fd$v3f1fùr";
  char key[] = "Locky1234";
  inscription();

  return 0;
}



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
void saisie (char *buffer, int sizeBuffer, char * text, int option) {
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
// et une option 0 pour inscription et 1 pour connexion.
char * catQuery(char * pseudo, char * mdp, int option) {
  char * buffer = malloc((strlen(pseudo)*strlen(mdp)+3)*sizeof(buffer));
  if(buffer == NULL) {
    perror("malloc()");
  }
  sprintf(buffer,"%d/%s/%s/", option,pseudo,mdp);
  printf("%s", buffer);
  return buffer;
}
char ** read_query_log (char * buffer, char ** result) {
  int       i         = 0,
            r         = 0,
            count     = 0;
  const int sizeAlloc = 3;
  char      temp [100];
  //char          **result;

  result = malloc(sizeAlloc * sizeof(*result));
  if(result == NULL){
    perror("malloc()");
    exit(1);
  }

  while(buffer[i] != '\0'){
    if(buffer[i] != '/'){
      temp[r] = buffer[i];
      i++;
      r++;
    }
    else {
      i++;
      result[count] = malloc((strlen(temp) + 1)*sizeof(result));
      if(result[count] == NULL){
        perror("malloc()");
        exit(1);
      }
      printf("%s\n",temp );
      strcpy(result[count],temp);
      count++;
      r = 0;

      memset(temp,'\0',strlen(temp));
    }
  }


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

  saisie (pseudo,taille_inscription, "Votre pseudo", option);
  printf("%s\n",pseudo);
  saisie (mdp,taille_inscription, "Votre mot de passe", option);
  printf("***********\n");
  crypt_pwd(mdp,pwdcrypt);

  printf("mdp  : %s **** \n",pwdcrypt );

  //decrypt_pwd(mdp,pwdcrypt);
  test = catQuery(pseudo,pwdcrypt,0);
 printf("test : %s ****\n",test );
  test2 = read_query_log (test,test2);
  printf("test[0] : %s : %ld /*\n",test2[0],strlen(test2[0]));
  printf("test[1] : %s  /*\n",test2[1]);
  printf("test[2] : %s  /*\n",test2[2]);
  //TODO : envoyer les données au serveur.
  //saisie_mdp(mdp, taille_inscription);
}
int sign_player (sqlite3 *db, char * pseudo, char * pwd) {
  write_data_player(db,pseudo,pwd);
  return 1;
}
int log_player (sqlite3 *db, char * pseudo, char * pwd){
  char *tempPwd;
  tempPwd = retrieve_pwd(db,pseudo);
  if (strcmp(tempPwd,pwd) == 0){
    return 1;
  }
  return 0;
}
char * control_connect (int sock) {
  sqlite3 *db;
  db = ouvrir_db();
  char pseudo[20];
  char buffer [100];
  //read(sock,buffer);
}
