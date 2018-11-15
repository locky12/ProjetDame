

enum {TEXTURE_PW, TEXTURE_PB, TEXTURE_LW, TEXTURE_LB,TEXTURE_PRIS};

void reset_st_move (Move * move);
int  move_pion (SDL_Event event, Damier damier[10][10], Move *move, int numberPlayer);
int event_click (SDL_Renderer *renderer, SDL_Event event, Damier damier[10][10], Move *move, int socket,int numberPlayer);
int select_pion (SDL_Event event, Damier damier[10][10], Move *move);
int control_position_pion (Damier damier[10][10], Move *move) ;
int control_position_empty (Damier damier[10][10], Move *move,int numberPlayer) ;
void changeposition (Damier damier[10][10], Move *move);
void init_game (Damier damier[10][10]);
void place_tile (Damier damier[10][10]);
void print_picture (SDL_Renderer *renderer,SDL_Texture ** arrayTexture , int enumTexture, SDL_Rect cases);
SDL_Window * init_view ();
void play_view (SDL_Window * window);
SDL_Texture ** create_texture (SDL_Renderer *renderer, SDL_Texture ** arrayTexture);
int eventClient(SDL_Event event, SDL_Renderer * renderer, Damier damier [10][10], Move * move, int socket,int numberPlayer);
SDL_bool exit_client (SDL_Event event, SDL_bool  quit, int socket);
void affichermatrice (Damier  damier[10][10]);
void print_damier (SDL_Renderer *renderer,SDL_Texture ** arrayTexture, Damier  damier[10][10]);
void afficheConnection (SDL_Renderer * renderer);
void changeDamier ( Damier damier[10][10],char * buffer, int numberPlayer);
int charInInt (char c);
char * recv_msg_server (int socket, char * buffer );
int control_recv_msg(char * buffer);
void print_case_damier (SDL_Renderer *renderer,SDL_Texture ** arrayTexture, Damier  damier[10][10], int i, int j);
void freeTexture (SDL_Texture ** arrayTexture);
SDL_Renderer *create_renderer (SDL_Window *window);
/*************************************************************************************************/
void write_serveur(int socket, char *buffer);
void read_serveur(int socket, char *buffer);
void send_move (Move * move, int ** arrayCapture, int size,int option, char * buffer  );
char **read_move_query(char * buffer);
