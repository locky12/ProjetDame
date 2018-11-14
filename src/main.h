
#include <SDL2/SDL.h>

#define VIDE 0
#define PION 1
#define REINE 2
#define PRIS 3
#define MULTIPLICATEUR_JOUEUR 10

typedef struct Damier {
  int pion;
  SDL_Rect cases;
}Damier;
