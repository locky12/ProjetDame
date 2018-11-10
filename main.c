#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include "main.h"
#include "affichage.h"
#include "deplacement.h"

int main(void)
{
	int joueurActuel = 1;
	Damier damier[10][10];
	printf(" Bienvenue dans le jeu de dame\n");
	initialiseDamier(damier);
	while (!jeuEstFini(damier, joueurActuel))
	{
		afficheDamier(damier);
		afficherJoueur(joueurActuel);
		faireJouer(joueurActuel, damier);
		joueurActuel = joueurActuel%2+1;
	}
	printf("la game est fini le joueur %d as gagné\n", joueurActuel%2+1);
	return 0;
}

void faireJouer(int joueurActuel, Damier damier[10][10])
{
	int xDepart;
	int yDepart;
	int xArrivee;
	int yArrivee;
	int deplacement;
	int nombreCoup;
	int * tabCoup[2] = {0};
	do
	{
		do
		{
			printf(" Entrez les coordonnees (x,y) depart :\n");
			xDepart = 10-lireCoordonnees();
			yDepart = lireCoordonnees()-1;
		} while (damier[xDepart][yDepart].pion/MULTIPLICATEUR_JOUEUR!=joueurActuel);

		nombreCoup = compteCoup(damier, joueurActuel, xDepart, yDepart);

		tabCoup[0] = malloc(nombreCoup * sizeof(tabCoup[0]));
		tabCoup[1] = malloc(nombreCoup * sizeof(tabCoup[1]));
		coupPossible(damier, joueurActuel, xDepart, yDepart, tabCoup);
		afficheCoup(tabCoup, nombreCoup);

		do
		{
			printf(" Entrez les coordonnees (x,y) arrivee :\n");
			xArrivee = 10-lireCoordonnees();
			yArrivee = lireCoordonnees()-1;
		} while (damier[xArrivee][yArrivee].pion!=VIDE);
		deplacement = deplacementValide(damier, joueurActuel, xDepart, yDepart, xArrivee, yArrivee);
	} while (!deplacement);

	damier[xArrivee][yArrivee].pion = damier[xDepart][yDepart].pion; // on effectue le deplacement
	damier[xDepart][yDepart].pion = VIDE;
	if(xArrivee == joueurActuel * 9 - 9) // la piece est sur la derniere ligne du plateau donc se transforme en reine
	{
		damier[xArrivee][yArrivee].pion = joueurActuel * MULTIPLICATEUR_JOUEUR + REINE;
	}
	if(deplacement == 2) // si la piece a capturé une piece ennemie
	{
		prisePion(damier, xDepart, yDepart, xArrivee, yArrivee); // on enleve le pion qui vien d'etre pris
		deplacement = possiblePrise(damier, joueurActuel, xArrivee, yArrivee);
		while (deplacement) // tant que la piece peut continué a prendre d'autre piece on demande de nouveau deplacmeent
		{
			afficheDamier(damier);
			xDepart = xArrivee;
			yDepart = yArrivee;
			printf(" Entrez les coordonnees (x,y) arrivee :\n");
			xArrivee = 10-lireCoordonnees();
			yArrivee = lireCoordonnees()-1;
			if(damier[xArrivee][yArrivee].pion==VIDE)
			{
				if(deplacementValide(damier, joueurActuel, xDepart, yDepart, xArrivee, yArrivee) == 2)
				{
					printf("X : %d  Y : %d\n", xArrivee-xDepart, yArrivee-yDepart);
					prisePion(damier, xDepart, yDepart, xArrivee, yArrivee);
					printf("X : %d  Y : %d\n", xArrivee-xDepart, yArrivee-yDepart);
					damier[xArrivee][yArrivee].pion = damier[xDepart][yDepart].pion;
					damier[xDepart][yDepart].pion = VIDE;
					if(xArrivee == joueurActuel * 9 - 9) // la piece est sur la derniere ligne du plateau donc se transforme en reine
					{
						damier[xArrivee][yArrivee].pion = joueurActuel * MULTIPLICATEUR_JOUEUR + REINE;
					}
					deplacement = possiblePrise(damier, joueurActuel, xArrivee, yArrivee);
					printf("salut\n" );
				}
				else
				{
					deplacement = 0;
				}
			}
			else
			{
				deplacement = 0;
			}
		}
	}
}

int lireCoordonnees(void)
{
	int coord = -1;
	while ((coord<1) || (coord>10))
	{
		scanf("%d", &coord);
	}
	return coord;
}

int jeuEstFini(Damier damier[10][10], int joueurActuel)
{
	int lignePion;
	int ligneCase;
	int colonnePion;
	int colonneCase;
	for (lignePion = 0 ; lignePion < 10 ; lignePion++)
	{
		for (colonnePion = 0 ; colonnePion < 10 ; colonnePion++)
		{
			if(damier[lignePion][colonnePion].pion/MULTIPLICATEUR_JOUEUR == joueurActuel)
			{
				for (ligneCase = 0 ; ligneCase < 10 ; ligneCase++)
				{
					for (colonneCase = 0 ; colonneCase < 10 ; colonneCase++)
					{
						if(colonneCase != colonnePion && ligneCase != lignePion)
						{
							if(deplacementValide(damier, joueurActuel, lignePion, colonnePion, ligneCase, colonneCase))
							{
								return 0;
							}
						}
					}
				}
			}
		}
	}
	return 1;
}

void initialiseDamier(Damier damier[10][10])
{
	int ligne;
	int colonne;
	for (ligne=0; ligne<4; ligne++)
	{
		for (colonne=0; colonne<10; colonne++)
		{
			if((ligne + colonne)%2 == 1)
			{
				damier[ligne][colonne].pion = 2 * MULTIPLICATEUR_JOUEUR + PION;
			}
			else
			{
				damier[ligne][colonne].pion = VIDE;
			}
		}
	}
	for (ligne=4; ligne<6; ligne++)
	{
		for (colonne=0; colonne<10; colonne++)
		{
			damier[ligne][colonne].pion = VIDE;
		}
	}
	for (ligne=6; ligne<10; ligne++)
	{
		for (colonne=0; colonne<10; colonne++)
		{
			if((ligne + colonne)%2 == 1)
			{
				damier[ligne][colonne].pion = 1 * MULTIPLICATEUR_JOUEUR + PION;
			}
			else
			{
				damier[ligne][colonne].pion = VIDE;
			}
		}
	}
	/*damier[5][5] = 1 * MULTIPLICATEUR_JOUEUR + REINE;/*
	damier[8][1] = 2 * MULTIPLICATEUR_JOUEUR + REINE;
	damier[6][1] = 2 * MULTIPLICATEUR_JOUEUR + REINE;
	damier[5][0] = 2 * MULTIPLICATEUR_JOUEUR + REINE;
	//damier[6][3] = 2 * MULTIPLICATEUR_JOUEUR + REINE;
	damier[3][3] = 2 * MULTIPLICATEUR_JOUEUR + PION;
	damier[4][5] = 1 * MULTIPLICATEUR_JOUEUR + PION;
	damier[2][7] = 2 * MULTIPLICATEUR_JOUEUR + PION;
	damier[0][9] = 2 * MULTIPLICATEUR_JOUEUR + PION;/*
	damier[0][0] = 1 * MULTIPLICATEUR_JOUEUR + PION;*/
}
