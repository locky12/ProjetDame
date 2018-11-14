#include <sqlite3.h>


typedef struct Player Player;
typedef struct Room Room;
typedef struct ArrayRoom ArrayRoom;

struct Player {
  int   socket;
  int   observer;
  char  pseudo[30];

};
struct Room {
  Player *play;
  int sizePlay;
  int index;
  int haveObserver;
  int inGame;
  int socketServer;
};
struct ArrayRoom {
  Room *array;
  int sizeArray;
  int indexFree;
};
typedef struct ListObserver {
  Player array[10];
  int size;
}ListObserver;





void crypt_pwd(char * pwd, char * pwdcrypt);
void decrypt_pwd(char * pwd, char * salt);
void saisie_login (char *buffer, int sizeBuffer, char * text, int option);
void inscription();
char * catQuery(char * pseudo, char * mdp,int observer, int option);
void readQueryLog(char * pseudo );
char * control_connect (ArrayRoom * array_room,ListObserver * list,int sock);
int log_player (char * pseudo, char * pwd);
int sign_player (char * pseudo, char * pwd);
char ** read_query_log (char * buffer, char ** result);
void read_msg(int socket, char *buffer);
void send_msg(int socket, char *buffer);
int connexion_client (int sock, char *pseudo, char *pwd, int observer, int inscription);
int add_client_array (ArrayRoom * array_room,int sock ,char* pseudo);
int add_observer_array (ArrayRoom * array_room,int sock ,char* pseudo);
Player add_client (Player  player, int socket, char * pseudo, int observer);
int * read_list_game(char * buffer);
void saisie(char *buffer);
int charinInt (char c);
void add_list_observer (ListObserver * list, int sock ,char* pseudo);
void choose_game_observer(int * list_room,int socket);
 int * index_observer(int * tab,ListObserver * list);
void send_list_room (ArrayRoom * array_room, ListObserver * list);
void copy_observer_array(ArrayRoom * array_room,ListObserver * list, int indRoom, int indList);                                                                                 
