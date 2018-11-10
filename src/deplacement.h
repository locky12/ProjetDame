
#include "struct.h"



int deplacementValide(Damier damier[10][10], int joueurActuel, int xDepart, int yDepart, int xArrivee, int yArrivee);
int deplacementValidePion(Damier damier[10][10], int joueurActuel, int xDepart, int yDepart, int xArrivee, int yArrivee);
int possiblePrise(Damier damier[10][10], int joueurActuel, int xDepart, int yDepart);
int deplacementValideReine(Damier damier[10][10], int joueurActuel, int xDepart, int yDepart, int xArrivee, int yArrivee);
int comptePion(Damier damier[10][10], int joueurActuel, int xDepart, int yDepart, int xArrivee, int yArrivee, int deltaX, int delatY);
int comptePionAdverse(Damier damier[10][10], int joueurActuel, int xDepart, int yDepart, int xArrivee, int yArrivee, int deltaX, int delatY);
int prisePion(Damier damier[10][10], int xDepart, int yDepart, int xArrivee, int yArrivee);
int compteCoup(Damier damier[10][10], int joueurActuel, int xDepart, int yDepart);
int coupPossible(Damier damier[10][10], int joueurActuel, int xDepart, int yDepart, int * tabCoup[2]);
int verfiePion(Damier damier[10][10], int joueurActuel, int xDepart, int yDepart);
