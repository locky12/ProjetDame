#include <stdio.h>
#include <sqlite3.h>
#include <string.h>
#include <stdlib.h>
typedef struct maxID maxID;
struct maxID {
  int id;
};
int ID;
static int callback(void *data, int argc, char **argv, char **azColName) {
  int i;
  maxID maxid;
  fprintf(stderr, "%s: ", (const char*)data);

  for(i = 0; i<argc; i++) {
    printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
  }
  printf("\n");
  return 0;
}
int callbackID(void * data,int argc, char **argv, char **azColName){
  char id[10] = {0};
  maxID maxid;
  int i;
  for(i = 0; i<argc; i++) {
    printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
    strncat(id,argv[i],1);
    printf("%s\n",id );
    ID = atoi(id);
  }
  printf("id :: %d\n", ID);
  return 0;
}
sqlite3 *ouvrir_db (int rc) {
  sqlite3 *db;
  char *zErrMsg = 0;


  rc = sqlite3_open("baseD.db", &db);

  if( rc ) {
    fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
    return(0);
  } else {
    fprintf(stderr, "Opened database successfully\n");
  }
  return db;
}
void cree_table_joueur(sqlite3 * db, int rc){
  char *ErrMsg = 0;
  char *sql;

  sql = "CREATE TABLE JOUEUR("  \
  "ID INTEGER PRIMARY KEY AUTOINCREMENT ," \
  "PSEUDO CHAR(30) NOT NULL," \
  "PWD CHAR(30) NOT NULL,"\
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
void insert_joueur (sqlite3 *db, int rc){
  char * sql;
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
void read_data (sqlite3 * db, char * requete){

  char * ErrMsg = 0;
  int rc;
  char * data = "salut";
  rc = sqlite3_exec(db, requete, callbackID, (void*)data, &ErrMsg);
  if( rc != SQLITE_OK ) {
    fprintf(stderr, "SQL error: %s\n", ErrMsg);
    sqlite3_free(ErrMsg);
  } else {
    fprintf(stdout, "Operation done successfully\n");
  }
  //printf("%s\n",data );
}
void write_data_joueur (sqlite3 * db, char * pseudo, char * pwd){
  char * ErrMsg = 0;
  int rc;
  char sql[500];

  sprintf(sql, "INSERT INTO JOUEUR VALUES(?,'%s','%s',0,0)", pseudo,pwd );
  rc = sqlite3_exec(db, sql, callback, 0, &ErrMsg);
  if( rc != SQLITE_OK ) {
    fprintf(stderr, "SQL error pas insertion: %s\n", ErrMsg);
    sqlite3_free(ErrMsg);
  } else {
    fprintf(stdout, "Operation done successfully\n");
  }
}
char * retrieve_pwd (sqlite3 *db,char * pseudo) {
  char * ErrMsg = 0;
  unsigned char *mdp;
  int rc;
  char sql[500];
  void *data;
  sqlite3_stmt *res;
  // sprintf(sql,"Select PWD from JOUEUR where PSEUDO = '%s'",pseudo);
  // rc = sqlite3_exec(db, sql, callback, (void*)data, &ErrMsg);
  // if( rc != SQLITE_OK ) {
  //   fprintf(stderr, "SQL error pas insertion: %s\n", ErrMsg);
  //   sqlite3_free(ErrMsg);
  // } else {
  //   fprintf(stdout, "Operation done successfully\n");
  // }
  // "SELECT Id, Name FROM Cars WHERE Id = @id"
  sprintf(sql,"Select PWD from JOUEUR where pseudo = ?");
  //sql = "Select PWD from JOUEUR where pseudo = ?";
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
     //sprintf(mdp, (char *)sqlite3_column_text(res, 0));
     mdp = (char *)sqlite3_column_text(res, 0);

  //   printf("%s\n", sqlite3_column_text(res, 1));

return mdp;
 }

}
void execute() {
  int rc;
  char * mdp;
  sqlite3 *db;
  printf("%s\n","avant ouverture" );
  db = ouvrir_db(rc);
  //cree_table_joueur(db,rc);
  //  insert_joueur(db, rc);

  //read_data(db, "Select max(ID) from Joueur");
  //write_data_joueur(db, "locky", "mdp");
  //select_joueur(db);
  //sqlite3_close(db);
  mdp = retrieve_pwd(db,"DONA");

  printf("mot de passe dans  : %s\n",mdp );
}
int main(int argc, char* argv[]) {
  execute();

}
