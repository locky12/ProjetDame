// #include "main.h"
#include "deplacement.h"
#include "partie.h"
#include "affichage.h"
#include <stdlib.h>
#include <stdio.h>


int unePartie ()
{
	int joueurActuel = 1;
	Damier damier[10][10];
	printf(" Bienvenue dans le jeu de dame\n");
	initialiseDamier(damier);
	while (!jeuEstFini(damier, joueurActuel))
	{
		//faireJouer(damier, joueurActuel);
		unTour(damier, joueurActuel);
		joueurActuel = joueurActuel%2+1;
	}
	printf("la game est fini le joueur %d as gagné\n", joueurActuel%2+1);
	return 0;
}

int unTour(Damier damier[10][10], int joueurActuel)
{
	int xDepart;
	int yDepart;
	int xArrivee;
	int yArrivee;
	int deplacement;
	int prise;
	int nombreCoup;
	int max;
	int maxPion;
	int * tabCoup[2] = {0};
	do
	{
		do
		{
			afficheDamier(damier);
			afficherJoueur(joueurActuel);
			printf(" Entrez les coordonnees (x,y) depart :\n");
			printf(" x : \n");
			xDepart = 10-lireCoordonnees();
			printf(" y : \n");
			yDepart = lireCoordonnees()-1;

			printf(" Entrez les coordonnees (x,y) arrivee :\n");
			printf(" x : \n");
			xArrivee = 10-lireCoordonnees();
			printf(" y : \n");
			yArrivee = lireCoordonnees()-1;
			deplacement = deplacementValide(damier, joueurActuel, xDepart, yDepart, xArrivee, yArrivee);
		} while (!deplacement);

		prise = canTakePlayer(damier, joueurActuel) + 1;
		max = rafleMax(damier, joueurActuel);
		Damier copie[10][10] = {0};
		copieDamier(damier, copie);
		effectueDeplacement(copie, joueurActuel, xDepart, yDepart, xArrivee, yArrivee);
		maxPion = rafleMaxPion(copie, joueurActuel, xArrivee, yArrivee) + 1;

	} while(max != 0 && (max != maxPion || deplacement != 2));

	effectueDeplacement(damier, joueurActuel, xDepart, yDepart, xArrivee, yArrivee);
	if(deplacement == 2)
	{
		prise = canTakePion(damier, joueurActuel, xArrivee, yArrivee);
		while(prise)
		{
			do
			{
				do
				{
					if (deplacement == 2)
					{
						xDepart = xArrivee;
						yDepart = yArrivee;
					}
					afficheDamier(damier);
					printf(" Entrez les coordonnees (x,y) arrivee :\n");
					printf(" x : \n");
					xArrivee = 10-lireCoordonnees();
					printf(" y : \n");
					yArrivee = lireCoordonnees()-1;
					deplacement = deplacementValide(damier, joueurActuel, xDepart, yDepart, xArrivee, yArrivee);
				} while(deplacement != 2);
				Damier copie[10][10] = {0};
				max = rafleMaxPion(copie, joueurActuel, xDepart, yDepart);
				copieDamier(damier, copie);
				effectueDeplacement(copie, joueurActuel, xDepart, yDepart, xArrivee, yArrivee);
				maxPion = rafleMaxPion(copie, joueurActuel, xArrivee, yArrivee) + deplacement - 1;
				if(max != maxPion)
				{
					deplacement = 0;
				}
			} while (max != maxPion);
			effectueDeplacement(damier, joueurActuel, xDepart, yDepart, xArrivee, yArrivee);
			prise = canTakePion(damier, joueurActuel, xArrivee, yArrivee);
		}
	}
	prendrePion(damier);
	return 0;
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
	/*damier[6][9].pion = 1 * MULTIPLICATEUR_JOUEUR + REINE;
	damier[5][8].pion = 2 * MULTIPLICATEUR_JOUEUR + PION;
	damier[5][6].pion = 2 * MULTIPLICATEUR_JOUEUR + PION;
	damier[4][5].pion = 2 * MULTIPLICATEUR_JOUEUR + PION;
	damier[7][6].pion = 2 * MULTIPLICATEUR_JOUEUR + PION;
	damier[7][4].pion = 2 * MULTIPLICATEUR_JOUEUR + PION;/*
	damier[4][5].pion = 1 * MULTIPLICATEUR_JOUEUR + PION;
	damier[2][7].pion = 2 * MULTIPLICATEUR_JOUEUR + PION;
	damier[0][9].pion = 2 * MULTIPLICATEUR_JOUEUR + PION;/*
	damier[0][0].pion = 1 * MULTIPLICATEUR_JOUEUR + PION;*/
}

// test sur tout les case du plateau si le joueur a un mouvement possible
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
