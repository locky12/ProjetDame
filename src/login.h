#include <sqlite3.h>

void crypt_pwd(char * pwd, char * pwdcrypt);
void decrypt_pwd(char * pwd, char * salt);
void saisie (char *buffer, int sizeBuffer, char * text, int option);
void inscription();
char * catQuery(char * pseudo, char * mdp, int option);
void readQueryLog(char * pseudo, char );
char * control_connect (int sock);
int log_player (sqlite3 *db, char * pseudo, char * pwd);
int sign_player (sqlite3 *db, char * pseudo, char * pwd);
char ** read_query_log (char * buffer, char ** result);

typedef struct Player Player;
typedef struct Room Room;

struct Player {
  int   socket;
  int   observer;
  char  pseudo[30];

};
struct Room {
  Player *play;
};
