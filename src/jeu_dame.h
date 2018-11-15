
#include <SDL2/SDL.h>

int deplacementValide(Damier damier[10][10], int joueurActuel, int xDepart, int yDepart, int xArrivee, int yArrivee);
int deplacementValidePion(Damier damier[10][10], int joueurActuel, int xDepart, int yDepart, int xArrivee, int yArrivee);
int canTakePion(Damier damier[10][10], int joueurActuel, int xDepart, int yDepart);
int canTakePlayer(Damier damier[10][10], int joueurActuel);
int deplacementValideReine(Damier damier[10][10], int joueurActuel, int xDepart, int yDepart, int xArrivee, int yArrivee);
int comptePion(Damier damier[10][10], int joueurActuel, int xDepart, int yDepart, int xArrivee, int yArrivee, int deltaX, int delatY);
int comptePionAdverse(Damier damier[10][10], int joueurActuel, int xDepart, int yDepart, int xArrivee, int yArrivee, int deltaX, int delatY);
int marquePion(Damier damier[10][10], int joueurActuel, int xDepart, int yDepart, int xArrivee, int yArrivee);
//int compteCoup(Damier damier[10][10], int joueurActuel, int xDepart, int yDepart);
//int coupPossible(Damier damier[10][10], int joueurActuel, int xDepart, int yDepart, int * tabCoup[2]);
int verfiePion(Damier damier[10][10], int joueurActuel, int xDepart, int yDepart);
void effectueDeplacement(Damier damier[10][10], int joueurActuel, int xDepart, int yDepart, int xArrivee, int yArrivee);
int deletePion (Damier damier[10][10]);
int rafleMax(Damier damier[10][10], int joueurActuel);
int rafleMaxPion(Damier damier[10][10], int joueurActuel, int xDepart, int yDepart);


int jeuEstFini(Damier damier[10][10], int joueurActuel);
void initialiseDamier(Damier damier[10][10]);
int copieDamier(Damier damier[10][10], Damier copie[10][10]);
//void damierBuffer(Damier damier[10][10], char * buffer);
// int charInInt (char c);
void bufferDamier(Damier damier[10][10], char * buffer);
// int * read_list_game(char * buffer);
