#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#include "poketudiant.h"

int main(int argc, char* argv[]){

	srand(time(NULL));
	
	Equipe team;
	PokeCafeteria cafet;

	team = initEquipe();
	cafet = initPokeCafeteria();

	int choix;
	int ch, a, b;
	PKETD *pk;

	do {
		printf("MENU\n");
		printf("\t 1- generer un poketudiant et l'afficher\n");
		printf("\t 2- generer un poketudiant et l'ajouter dans l'equipe\n");
		printf("\t 3- generer un poketudiant et l'ajouter dans la PokeCafeteria\n");
		printf("\t 4- switcher des poketudiant dans l'equipe\n");
		printf("\t 5- drop un poketudiant de l'equipe vers la PokeCafeteria\n");
		printf("\t 6- afficher l'equipe\n");
		printf("\t 7- afficher la PokeCafeteria\n");
		printf("\t 0-Quitter !\n");
		printf("Votre choix : ");

		scanf("%d", &choix);


		switch(choix) {
			case 1 :
				do {
					printf("Quel type de pokétudiant voulez vous générer ? aleaoire , NOISY, LAZY, MOTIVATED, TEACHER (0, 1, 2, 3, 4)  ");
					scanf("%d", & ch);
				} while (ch < 0 || ch > 4);
				pk = generePoketudiant(ch);
				affichePoketudiant(pk);
				break;
			case 2 :
				printf("Quel type de pokétudiant voulez vous générer ? aleaoire , NOISY, LAZY, MOTIVATED, TEACHER (0, 1, 2, 3, 4)  ");
				scanf("%d", & ch);
				pk = generePoketudiant(ch);
				ajouteEquipe(&team, pk); 
				break;
			case 3 : 
				printf("Quel type de pokétudiant voulez vous générer ? aleaoire , NOISY, LAZY, MOTIVATED, TEACHER (0, 1, 2, 3, 4)  ");
				scanf("%d", & ch);
				pk = generePoketudiant(ch);
				ajouteCafet(&cafet, pk); 
				break;
			case 4 :
				printf("Quels poketudiants voulez vous échanger ?\n");
				printf("pokétudiant 1 : "); scanf("%d", &a);
				printf("pokétudiant 2 : "); scanf("%d", &b);
				switchEquipe(&team, a, b);
				break;
			case 5 :
				printf("Quel poketudiant voulez vous transferer ? "); scanf("%d", &ch);
				drop(&team, &cafet, ch);
				break;

			case 6 :
				afficheEquipe(team);
				break;

			case 7 :
				affichePokeCafeteria(cafet);
				break;
				

			case 0 : 
				printf("Au revoir !\n");
				break;
			default : 
				printf("Vous n'avez pas rentré une valeur acceptable\n");
				break;
		}
	} while(choix !=0);

	return 0;
}
