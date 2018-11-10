
#include <SDL2/SDL.h>

#define VIDE 0
#define PION 1
#define REINE 2
#define MULTIPLICATEUR_JOUEUR 10

typedef struct Damier {
  int pion;
  SDL_Rect cases;
}Damier;

int jeuEstFini(Damier damier[10][10], int joueurActuel);
void initialiseDamier(Damier damier[10][10]);
void faireJouer(int joueurActuel, Damier damier[10][10]);
int lireCoordonnees(void);
