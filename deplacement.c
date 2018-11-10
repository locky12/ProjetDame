//#include "main.h"
#include "deplacement.h"
#include <stdlib.h>
#include <stdio.h>

#define VIDE 0
#define PION 1
#define REINE 2
#define MULTIPLICATEUR_JOUEUR 10

// prend une case depart et arrivee et appelle la focntion necessaire pour le pion ou la reine.
int deplacementValide(Damier damier[10][10], int joueurActuel, int xDepart, int yDepart, int xArrivee, int yArrivee)
{
	if (damier[xDepart][yDepart].pion/MULTIPLICATEUR_JOUEUR == joueurActuel) {
		/* code */

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

// prend une case depart et arrivee et retourne si le deplacement du pion est valide
int deplacementValidePion(Damier damier[10][10], int joueurActuel, int xDepart, int yDepart, int xArrivee, int yArrivee)
{
	int deltaX = xArrivee-xDepart;
	int deltaY = yArrivee-yDepart;

	if(deltaY == 1 || deltaY == -1)
	{
		if(deltaX == (joueurActuel*(2)-3)) // avance d'une case dans le sens du joueur
		{
		return 1; // deplacement sans prise
		}
	}
	else if(deltaY == 2 || deltaY == -2)
	{
		if(deltaX == (joueurActuel*(4)-6)) // avance de 2 case dans le sens du joueur
		{

			if(damier[xDepart + (deltaX/2)][yDepart + (deltaY/2)].pion/MULTIPLICATEUR_JOUEUR == (joueurActuel%2+1)) // la case entre la cas de depart et celle d'arrive se trouve une piece adverse
			{
				return 2; //deplacmeent avec prise d'une piece
			}
		}
	}
	return 0; // deplacement non valide
}

// prend une case de depart et reoturn 1 si la piece sur la case peut prendre une piece adverse
int possiblePrise(Damier damier[10][10], int joueurActuel, int xDepart, int yDepart)
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
		if(damier[xDepart][yDepart].pion/MULTIPLICATEUR_JOUEUR == joueurActuel%2+1)
		{
			compte++;
		}
	}
	return compte;
}

// enleve le premier pion entre 2 case dans une diagonale
int prisePion(Damier damier[10][10], int xDepart, int yDepart, int xArrivee, int yArrivee)
{
	int deltaX = xArrivee - xDepart;
	int deltaY = yArrivee - yDepart;
	while ((xDepart + deltaX/abs(deltaX)) != xArrivee)
	{
		xDepart += deltaX/abs(deltaX);
		yDepart += deltaY/abs(deltaY);
		printf(" pion : %d  joueur : %d\n",damier[xDepart][yDepart].pion,damier[xDepart][yDepart].pion/MULTIPLICATEUR_JOUEUR);
		if(damier[xDepart][yDepart].pion/MULTIPLICATEUR_JOUEUR != VIDE)
		{
			damier[xDepart][yDepart].pion = VIDE;
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
