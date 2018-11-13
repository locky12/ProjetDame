#include <SDL2/SDL.h>

typedef struct Damier {
  int pion;
  SDL_Rect cases;
}Damier;

typedef struct Move {
  int position[2];
  int newPosition[2];
}Move;
