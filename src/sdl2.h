#include "deplacement.h"
typedef struct Move {
  int position[2];
  int newPosition[2];
}Move;

enum {TEXTURE_PW, TEXTURE_PB, TEXTURE_LW, TEXTURE_LB};

void reset_st_move (Move * move);
int  move_pion (SDL_Event event, Damier damier[10][10], Move *move);
void event_click (SDL_Renderer *renderer, SDL_Event event, Damier damier[10][10], Move *move);
int select_pion (SDL_Event event, Damier damier[10][10], Move *move);
int control_position_pion (Damier damier[10][10], Move *move) ;
int control_position_empty (Damier damier[10][10], Move *move) ;
void changeposition (Damier damier[10][10], Move *move);
void init_game (Damier damier[10][10]);
void place_tile (Damier damier[10][10]);
void print_picture (SDL_Renderer *renderer,SDL_Texture ** arrayTexture , int enumTexture, SDL_Rect cases);
SDL_Window * init_view ();
void play_view (SDL_Window * window);
SDL_Texture ** create_texture (SDL_Renderer *renderer, SDL_Texture ** arrayTexture);
void eventClient(SDL_Event event, SDL_Renderer * renderer, Damier damier [10][10], Move * move);
SDL_bool exit_client (SDL_Event event, SDL_bool quit);
void affichermatrice (Damier  damier[10][10]);
void print_damier (SDL_Renderer *renderer,SDL_Texture ** arrayTexture, Damier  damier[10][10]);
void afficheConnection (SDL_Renderer * renderer);
