// #include "main.h"
#include "struct.h"
#include "deplacement.h"
#include <stdlib.h>
#include <stdio.h>


#define VIDE 0
#define PION 1
#define REINE 2
#define PRIS 3
#define MULTIPLICATEUR_JOUEUR 10

// prend une case depart et arrivee et appelle la focntion necessaire pour le pion ou la reine.
int deplacementValide(Damier damier[10][10], int joueurActuel, int xDepart, int yDepart, int xArrivee, int yArrivee)
{
	if(damier[xDepart][yDepart].pion/MULTIPLICATEUR_JOUEUR == joueurActuel && damier[xArrivee][yArrivee].pion == VIDE)
	{
		switch (damier[xDepart][yDepart].pion%MULTIPLICATEUR_JOUEUR)
		{
			case PION:
			return deplacementValidePion(damier, joueurActuel, xDepart, yDepart, xArrivee, yArrivee);
			break;
			case REINE:
			return deplacementValideReine(damier, joueurActuel, xDepart, yDepart, xArrivee, yArrivee);
			break;
		}
	}
	return 0;
}

void effectueDeplacement(Damier damier[10][10], int joueurActuel, int xDepart, int yDepart, int xArrivee, int yArrivee)
{
	if(joueurActuel * 9 - 9 == xArrivee)
	{
		damier[xArrivee][yArrivee].pion = joueurActuel * MULTIPLICATEUR_JOUEUR + REINE;
	}
	else
	{
		damier[xArrivee][yArrivee].pion = damier[xDepart][yDepart].pion;
	}
	damier[xDepart][yDepart].pion = VIDE;
	marquePion(damier, joueurActuel, xDepart, yDepart, xArrivee, yArrivee);
}

// prend une case depart et arrivee et retourne si le deplacement du pion est valide
int deplacementValidePion(Damier damier[10][10], int joueurActuel, int xDepart, int yDepart, int xArrivee, int yArrivee)
{
	int deltaX = xArrivee-xDepart;
	int deltaY = yArrivee-yDepart;

	if(abs(deltaY) == 1)
	{
		if(deltaX == (joueurActuel*(2)-3)) // avance d'une case dans le sens du joueur
		{
			return 1; // deplacement sans prise
		}
	}
	else if(abs(deltaY) == 2 && abs(deltaX) == 2)
	{
		int casePrise = damier[xDepart + (deltaX/2)][yDepart + (deltaY/2)].pion;
		if(casePrise/MULTIPLICATEUR_JOUEUR == (joueurActuel%2+1) && casePrise%MULTIPLICATEUR_JOUEUR != PRIS) // la case entre la cas de depart et celle d'arrive se trouve une piece adverse
		{
			return 2; //deplacmeent avec prise d'une piece
		}
	}
	return 0; // deplacement non valide
}

int canTakePlayer(Damier damier[10][10], int joueurActuel)
{
	int ligne;
	int colonne;
	int prise = 0;
	for (ligne = 0 ; ligne < 10 ; ligne++)
	{
		for (colonne = 0 ; colonne < 10 ; colonne++)
		{
			prise = canTakePion(damier, joueurActuel, ligne, colonne) || prise;
		}
	}
	return prise;
}
// prend une case de depart et reoturne 1 si la piece sur la case peut prendre une piece adverse
int canTakePion(Damier damier[10][10], int joueurActuel, int xDepart, int yDepart)
{
	int ligne;
	int colonne;
	for (ligne = 0 ; ligne < 10 ; ligne++)
	{
		for (colonne = 0 ; colonne < 10 ; colonne++)
		{
			if(ligne != xDepart && colonne != yDepart)
			{
				if(damier[ligne][colonne].pion == VIDE && deplacementValide(damier, joueurActuel, xDepart, yDepart, ligne, colonne) == 2 )
				{
					return 1;
				}
			}
		}
	}
	return 0;
}

int rafleMax(Damier damier[10][10], int joueurActuel)
{
	Damier copie[10][10] = {0};
	copieDamier(damier, copie);
	int ligne;
	int colonne;
	int max = 0;
	int temp;
	for (ligne = 0 ; ligne < 10 ; ligne++)
	{
		for (colonne = 0 ; colonne < 10 ; colonne++)
		{
			temp = rafleMaxPion(copie, joueurActuel, ligne, colonne);
			if(max < temp)
			{
				max = temp;
			}
		}
	}
	return max;
}

int rafleMaxPion(Damier damier[10][10], int joueurActuel, int xDepart, int yDepart)
{
	Damier copie[10][10] = {0};
	copieDamier(damier, copie);
	int ligne;
	int colonne;
	int compte = 0;
	int max = 0;
	int temp;
	for (ligne = 0 ; ligne < 10 ; ligne++)
	{
		for (colonne = 0 ; colonne < 10 ; colonne++)
		{
			copieDamier(damier, copie);
			if(deplacementValide(copie, joueurActuel, xDepart, yDepart, ligne, colonne) == 2)
			{
				effectueDeplacement(copie, joueurActuel, xDepart, yDepart, ligne, colonne);
				temp = 1 + rafleMaxPion(copie, joueurActuel, ligne, colonne);
				if(max < temp)
				{
					max = temp;
				}
			}
		}
	}
	return max;
}

// prend une case depart et arrivee et retourne si le deplacement du reine est valide
int deplacementValideReine(Damier damier[10][10], int joueurActuel, int xDepart, int yDepart, int xArrivee, int yArrivee)
{
	int deltaX = xArrivee-xDepart;
	int deltaY = yArrivee-yDepart;
	if(abs(deltaX) == abs(deltaY))
	{
		int compte = comptePion(damier, joueurActuel, xDepart, yDepart, xArrivee, yArrivee, deltaX, deltaY);
		int compteAdverse = comptePionAdverse(damier, joueurActuel, xDepart, yDepart, xArrivee, yArrivee, deltaX, deltaY);
		if(compteAdverse - compte == 0) // si tout les pion sont des piece adverse
		{
			if(compteAdverse == 1)
			{
				return 2; // deplacement avec prise d'une piece
			}
			else if (compteAdverse == 0)
			{
				return 1; // depalcement sans prise
			}
			return 0; //deplacement avec tentative de plusieurs pris en un deplacement
		}
		return 0; // tentative de passer par dessus une piece allié
	}
	return 0; // deplacement non diagonale
}

// reoturne le compte le nombre de piece entre 2 case dans une diagonale
int comptePion(Damier damier[10][10], int joueurActuel, int xDepart, int yDepart, int xArrivee, int yArrivee, int deltaX, int deltaY)
{
	int compte = 0;
	while ((xDepart + deltaX/abs(deltaX)) != xArrivee)
	{
		xDepart += deltaX/abs(deltaX);
		yDepart += deltaY/abs(deltaY);
		if(damier[xDepart][yDepart].pion != VIDE)
		{
			compte++;
		}
	}
	return compte;
}

// retourne le compte le nombre de piece adverse entre 2 case dans une diagonale
int comptePionAdverse(Damier damier[10][10], int joueurActuel, int xDepart, int yDepart, int xArrivee, int yArrivee, int deltaX, int deltaY)
{
	int compte = 0;
	while ((xDepart + deltaX/abs(deltaX)) != xArrivee)
	{
		xDepart += deltaX/abs(deltaX);
		yDepart += deltaY/abs(deltaY);
		if((damier[xDepart][yDepart].pion/MULTIPLICATEUR_JOUEUR == joueurActuel%2+1) && (damier[xDepart][yDepart].pion%MULTIPLICATEUR_JOUEUR != PRIS))
		{
			compte++;
		}
	}
	return compte;
}

// enleve le premier pion entre 2 case dans une diagonale
int marquePion(Damier damier[10][10], int joueurActuel, int xDepart, int yDepart, int xArrivee, int yArrivee)
{
	int deltaX = xArrivee - xDepart;
	int deltaY = yArrivee - yDepart;
	while ((xDepart + deltaX/abs(deltaX)) != xArrivee)
	{
		xDepart += deltaX/abs(deltaX);
		yDepart += deltaY/abs(deltaY);
		if(damier[xDepart][yDepart].pion/MULTIPLICATEUR_JOUEUR != VIDE)
		{
			damier[xDepart][yDepart].pion = (joueurActuel%2+1) * MULTIPLICATEUR_JOUEUR + PRIS;
			return 1;
		}
	}
	return 0;
}

// retourn le nombre de coup possible pour une piece donné
int compteCoup(Damier damier[10][10], int joueurActuel, int xDepart, int yDepart)
{
	int ligne;
	int colonne;
	int nombreCoup = 0;
	for (ligne = 0 ; ligne < 10 ; ligne++)
	{
		for (colonne = 0 ; colonne < 10 ; colonne++)
		{
			if(ligne != xDepart && colonne != yDepart)
			{
				if(damier[ligne][colonne].pion == VIDE && deplacementValide(damier, joueurActuel, xDepart, yDepart, ligne, colonne))
				{
					nombreCoup++;
				}
			}
		}
	}
	printf(" nombreCoup : %d\n", nombreCoup);
	return nombreCoup;
}

// modifie la liste des coup possible d'une piece donné
int coupPossible(Damier damier[10][10], int joueurActuel, int xDepart, int yDepart, int * tabCoup[2])
{
	int ligne;
	int colonne;
	int nombreCoup = 0;
	for (ligne = 0 ; ligne < 10 ; ligne++)
	{
		for (colonne = 0 ; colonne < 10 ; colonne++)
		{
			if(ligne != xDepart && colonne != yDepart)
			{
				if(damier[ligne][colonne].pion == VIDE && deplacementValide(damier, joueurActuel, xDepart, yDepart, ligne, colonne))
				{
					printf(" Coup %d : ligne : %d  colonne : %d \n", nombreCoup, 10-ligne, colonne+1);
					tabCoup[0][nombreCoup] = ligne;
					tabCoup[1][nombreCoup] = colonne;
					nombreCoup++;
				}
			}
		}
	}
	return 0;
}

// prend une piece et un joueur et retourn 1 si la piece appartient au joueur
int verfiePion(Damier damier[10][10], int joueurActuel, int xDepart, int yDepart)
{
	if(damier[xDepart][yDepart].pion/MULTIPLICATEUR_JOUEUR == joueurActuel)
	{
		return 1;
	}
	return 0;
}

int deletePion (Damier damier[10][10])
{
	int ligne;
	int colonne;
	for (ligne = 0 ; ligne < 10 ; ligne++)
	{
		for (colonne = 0 ; colonne < 10 ; colonne++)
		{
			if(damier[ligne][colonne].pion%MULTIPLICATEUR_JOUEUR == PRIS)
			{
				damier[ligne][colonne].pion = VIDE;
			}
		}
	}
}


int copieDamier(Damier damier[10][10], Damier copie[10][10])
{
	int i, j;
	for(i = 0 ; i < 10 ; i++)
	{
		for(j = 0 ; j < 10 ; j++)
		{
			copie[i][j] = damier[i][j];
		}
	}
	return 1;
}

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
			//afficheDamier(damier);
			//afficherJoueur(joueurActuel);
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
					//afficheDamier(damier);
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
	deletePion(damier);
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
	for (int  ligne = 0; ligne < 10; ligne++) {
    for (int colonne = 0; colonne < 10; colonne++) {
      if((ligne+colonne)%2 == 0) {
        if(ligne < 4){

          damier[ligne][colonne].pion = 2 * MULTIPLICATEUR_JOUEUR + PION;
        }
        else if (ligne > 5){

          damier[ligne][colonne].pion = 1 * MULTIPLICATEUR_JOUEUR + PION;

        }
        else {
          damier[ligne][colonne].pion = VIDE;
        }
      }
      else {

        damier[ligne][colonne].pion = VIDE;
      }
    }
  }
  /*damier[5][7].pion = 1 * MULTIPLICATEUR_JOUEUR + REINE;
	damier[4][6].pion = 2 * MULTIPLICATEUR_JOUEUR + PION;
	damier[4][4].pion = 2 * MULTIPLICATEUR_JOUEUR + PION;
	damier[3][3].pion = 2 * MULTIPLICATEUR_JOUEUR + PION;
	damier[6][4].pion = 2 * MULTIPLICATEUR_JOUEUR + PION;
	damier[6][2].pion = 2 * MULTIPLICATEUR_JOUEUR + PION;/*
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
