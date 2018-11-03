#include <sqlite3.h>
/* Prototypes */
static int callback(void *data, int argc, char **argv, char **azColName);
//Ouvrir la database
sqlite3 *ouvrir_db ();
//Cree la table Joueur (Id,Pseudo,Pwd, Game_win, Game_loose)
void cree_table_joueur(sqlite3 * db);
// Affiche les resultat de la requete passer en parametre
void read_data (sqlite3 * db, char * requete);
// crée une nouvelle entrée joueur
void write_data_player (sqlite3 * db, char * pseudo, char * pwd);
// retourne le mdp par rapport au pseudo
char * retrieve_pwd (sqlite3 *db,char * pseudo);



/********* TEST ********/

void insert_joueur (sqlite3 *db);
void select_joueur (sqlite3 * db);
