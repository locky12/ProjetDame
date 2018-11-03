#include "main.h"
#include "affichage.h"
#include <stdio.h>

void afficherJoueur(int joueurActuel)
{
	printf("C'est maintenant au joueur ");
	if (joueurActuel==1) {
		printf("blanc");
	}
	else {
		printf("noir");
	}
	printf(" de jouer\n");
}

void afficheDamier(Damier damier[10][10])
{
	int ligne;
	int colonne;
	for (ligne=0; ligne<10; ligne++)
	{
		if(ligne != 0)
		{
		printf(" %d  ", 10-ligne);
		}
		else
		{
		printf(" %d ", 10-ligne);
		}
		for (colonne=0; colonne<10; colonne=colonne+1)
		{
			if (damier[ligne][colonne].pion==VIDE)
			{
				printf("-- ");
			}
			else {
				switch (damier[ligne][colonne].pion%MULTIPLICATEUR_JOUEUR)
				{
					case PION: printf("P"); break;
					case REINE: printf("Q"); break;
					printf("%d ", damier[ligne][colonne].pion);
				}
				printf("%d", damier[ligne][colonne].pion/MULTIPLICATEUR_JOUEUR); // afficher numero joueur
				printf(" ");
			}
		}
		printf("\n");
	}
	printf("   ");
	for (colonne=0; colonne<10; colonne++)
	{
		printf(" %d ", colonne+1);
	}
	printf("\n");
}

void afficheCoup(int * tabCoup[2], int nombreCoup)
{
	int i;
	for(i = 0 ; i < nombreCoup ; i++)
	{
		printf(" coup %d : ligne : %d  colonne : %d \n", i+1, 10-tabCoup[0][i], tabCoup[1][i]+1 );
	}
}
