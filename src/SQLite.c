#include <stdio.h>
#include <sqlite3.h>
#include <string.h>
#include <stdlib.h>

#include "SQLite.h"
/*function qui permet d'afficher les résultats d'une requete */
/* argc est le nombre de ligne, argv les resultat de la requete */
/* azColname le nom des colonnes */
static int callback(void *data, int argc, char **argv, char **azColName) {

  int i;
  fprintf(stderr, "%s: ", (const char*)data);

  for(i = 0; i<argc; i++) {
    printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
  }
  printf("\n");
  return 0;
}

/* Permet de lier le descripteur au fichier /*
/* La fonction retourne le descripteur. */
sqlite3 *ouvrir_db () {
  sqlite3 *db;
  int rc;
  char *zErrMsg = 0;
  rc = sqlite3_open("BDD1.db", &db);



  if( rc ) {
    fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
    return(0);
  } else {
    fprintf(stderr, "Opened database successfully\n");
  }
  cree_table_joueur(db);
  return db;
}
/* Fonction qui crée la table joueur avec un Id qui s'autoincrement, un pseudo,
/*un mot de passe et les parties gagner et perdu. */
void cree_table_joueur(sqlite3 * db){
  int rc;
  char *ErrMsg = 0;
  char *sql;

  sql = "CREATE TABLE JOUEUR("  \
  "ID INTEGER PRIMARY KEY AUTOINCREMENT ," \
  "PSEUDO TEXT(30) NOT NULL," \
  "PWD TEXT(30) NOT NULL,"\
  "GAME_WIN INT," \
  "GAWE_LOOSE INT);";

  rc = sqlite3_exec(db, sql, callback, 0, &ErrMsg);

  if( rc != SQLITE_OK ){
    fprintf(stderr, "SQL error: %s\n", ErrMsg);
    sqlite3_free(ErrMsg);
  } else {
    fprintf(stdout, "Table created successfully\n");
  }
}
/*Fonction qui insert un joueur */
/* TODO : sert example à ne pas utiliser pour autre que les tests */
void insert_joueur (sqlite3 *db){
  char * sql;
  int rc;
  char * ErrMsg = 0;

  sql = "INSERT INTO JOUEUR (PSEUDO,PWD)" \
  "VALUES ('DONA', 'mdp');";

  rc = sqlite3_exec(db, sql, callback, 0, &ErrMsg);

  if( rc != SQLITE_OK ){
    fprintf(stderr, "SQL error: %s\n", ErrMsg);
    sqlite3_free(ErrMsg);
  } else {
    fprintf(stdout, "Records created successfully\n");
  }
}
/* Select de la table joueur */
/* TODO : Pour les tests */
void select_joueur (sqlite3 * db){
  int rc;
  char *sql;
  char * ErrMsg = 0;
  const char* data = "Callback function called";
  sql = "Select * from JOUEUR";
  rc = sqlite3_exec(db, sql, callback, (void*)data, &ErrMsg);
  if( rc != SQLITE_OK ) {
    fprintf(stderr, "SQL error: %s\n", ErrMsg);
    sqlite3_free(ErrMsg);
  } else {
    fprintf(stdout, "Operation done successfully\n");
  }
}
// Permet d'executer une requete en parametre et d'afficher.
int read_data (sqlite3 * db, char * requete){

  char * ErrMsg = 0;
  int rc;
  char * data;
  rc = sqlite3_exec(db, requete, callback, (void*)data, &ErrMsg);
  if( rc != SQLITE_OK ) {
    fprintf(stderr, "SQL error: %s\n", ErrMsg);
    sqlite3_free(ErrMsg);
    return 0;
  } else {
    fprintf(stdout, "Operation done successfully\n");
    return 1;
  }
}

/* fonction qui d'ajouter un joueur. Elle prend le pseudo et le mot de passe */
/* et les ajoutes dans la base de donnee. Elle initialise les scores à O */
int write_data_player (sqlite3 * db, char * pseudo, char * pwd){
  char * ErrMsg = 0;
  int rc;
  char sql[500];

  sprintf(sql, "INSERT INTO JOUEUR VALUES(?,'%s','%s',0,0)", pseudo,pwd );
  rc = sqlite3_exec(db, sql, callback, 0, &ErrMsg);
  if( rc != SQLITE_OK ) {
    fprintf(stderr, "SQL error pas insertion: %s\n", ErrMsg);
    sqlite3_free(ErrMsg);
    return 0;
  } else {
    fprintf(stdout, "Operation done successfully\n");
    return 1;
  }
}
/* Prend en parametre un pseudo et returne le mot de passe correspondant */

char * controlMDP(sqlite3 *db,char * pseudo, char * result){
  puts("controlMDP");
  char buffer[100];
  // char  * result;
  int verif = 0;
  sprintf(buffer,"%s'%s'","Select pseudo From Joueur where pseudo = ",pseudo);
  verif = read_data(db,buffer);
   printf("verif %d\n",verif );
  if(verif == 1){
    result = retrieve_pwd(db,pseudo,result);
    printf("result mdp : %s\n",result );
    return result;

  }
 return "\0";
}
char * retrieve_pwd (sqlite3 *db,char * pseudo,char *mdp) {
  char * ErrMsg = 0;
  //char *mdp;
  int rc;
  int verif = 0;
  char sql[500];
  void *data;
  sqlite3_stmt *res;
  char buffer[100];
  // sprintf(buffer,"%s%s","Select pseudo from Joueur where pseudo = ",pseudo);
  // verif = read_data(db,buffer);
  // printf("verif %d\n",verif );
  if(!retrieve_pseudo(db,pseudo)) {
  sprintf(sql,"Select PWD from JOUEUR where pseudo = ?");
  rc = sqlite3_prepare_v2(db, sql, -1, &res, 0);

 if (rc == SQLITE_OK) {

     int idx = sqlite3_bind_parameter_index(res, "@id");
     sqlite3_bind_text(res, 1, pseudo,strlen(pseudo),0);

 } else {

     fprintf(stderr, "Failed to execute statement: %s\n", sqlite3_errmsg(db));
 }

 int step = sqlite3_step(res);

 if (step == SQLITE_ROW) {
     printf("Le mot de passe est : %s\n", sqlite3_column_text(res, 0));
     mdp = (char *)sqlite3_column_text(res, 0);
     //sqlite3_reset(res);
     puts("fin1");
     return mdp;
 }
}
//sqlite3_reset(res);
 puts("fin");
return NULL;
}
int retrieve_pseudo (sqlite3 *db, char * pseudo) {
  puts("dans retrieve pseudo ");
  char * ErrMsg = 0;
  unsigned char *ps;
  int rc;
  char sql[500];
  void *data;
  sqlite3_stmt *res;
  int verif = 0;

  sprintf(sql,"Select PSEUDO from JOUEUR where pseudo = ?");
  rc = sqlite3_prepare_v2(db, sql, -1, &res, 0);

 if (rc == SQLITE_OK) {

     int idx = sqlite3_bind_parameter_index(res, "@id");
     verif = sqlite3_bind_text(res, 1, pseudo,20,0);
     printf("id : %d, %d \n",idx, verif );
 } else {

     fprintf(stderr, "Failed to execute statement: %s\n", sqlite3_errmsg(db));
     return 0;
 }
 puts("crash");
 if(verif == SQLITE_OK){
   puts("crash");
   int step = sqlite3_step(res);


 if (step == SQLITE_ROW) {

     printf("Le pseudo est : %s\n", sqlite3_column_text(res, 0));
     ps = (char *)sqlite3_column_text(res, 0);
     if(strcmp(ps,pseudo) == 0 ){
       return 1;
     }
 }
 }
 return 0;
}

// /* Fonction de test du fichier */
// void execute() {
//   int rc;
//   char * mdp;
//   sqlite3 *db;
//   printf("%s\n","avant ouverture" );
//   db = ouvrir_db(rc);
//   //cree_table_joueur(db,rc);
//   //  insert_joueur(db, rc);
//
//   //read_data(db, "Select max(ID) from Joueur");
//   //write_data_joueur(db, "locky", "mdp");
//   //select_joueur(db);
//   //sqlite3_close(db);
//   mdp = retrieve_pwd(db,"DONA");
//
//   printf("mot de passe dans  : %s\n",mdp );
// }
// int main(int argc, char* argv[]) {
//   execute();
//
// }
