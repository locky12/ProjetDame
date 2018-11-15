#include <SDL2/SDL.h>

typedef struct Damier {
  int pion;
  SDL_Rect cases;
}Damier;

typedef struct Move {
  int position[2];
  int newPosition[2];
}Move;

#define VIDE 0
#define PION 1
#define REINE 2
#define PRIS 3
#define MULTIPLICATEUR_JOUEUR 10
