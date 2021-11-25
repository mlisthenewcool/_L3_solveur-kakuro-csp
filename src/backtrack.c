#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "backtrack.h"
#include "util.h"
#include "extern.h"

/**---------------------------------------------------------------------------*
 *                                                                            *
 *----------------------------------------------------------------------------*/
int backtrack()
{
	int current_var_indice = 0;
	int consistant;

	//printf("\nBacktrack...\n");

	// tant qu'il reste des variables
	while(current_var_indice < nb_variables)
	{
		//printf("-- {%d} --\n", variables[current_var_indice]->num);
		nb_noeuds_arbre_recherche++;
		consistant = 0;

		// tant que le domaine de la variable actuelle n'est pas vide
		while(variables[current_var_indice]->domaine_length > 0 && variables[current_var_indice]->affectation < TAILLE_DOMAINE)
		{
			// on teste la variable suivante
			variables[current_var_indice]->affectation++;

			//afficher_CSP(variables);

			//printf("AFFECTATION : %d\n", variables[current_var_indice]->affectation);

			// test de consistance
			consistant = est_consistant(variables[current_var_indice]);

			// la solution est consistante : on passe à la variable suivante
			if(consistant == 1)
			{
				//printf("CONSISTANT\n");
				current_var_indice++;
				break;
			}
		}

		// solution non consistance, on revient en arrière
		if(consistant == 0)
		{
			variables[current_var_indice]->affectation = 0;
			current_var_indice--;

			// si on est revenu à la première variable, il n'y a pas de solution
			if(current_var_indice == 0)
			{
				return 0;
			}
		}
	}

	// solution trouvée !
	return 1;
}

/*
int backtrack()
{
	//s_variable **pile = malloc(nb_variables * sizeof(s_variable*));

	int current_var_indice = 0;
	int consistant;

	// tant qu'il reste des variables
	while(current_var_indice < nb_variables)
	{
		//pile[current_var_indice] = variables[current_var_indice];
		consistant = 0;

		// tant que le domaine de la variable actuelle n'est pas vide
		while(variables[current_var_indice]->domaine_length > 0)
		{
			//printf("{%d}\n", current_var_indice);
			nb_noeuds_arbre_recherche++;

			variables[current_var_indice]->affectation++;

			if(variables[current_var_indice]->domaine[pile[current_var_indice]->affectation-1] == OUVERT)
			{
				consistant = est_consistant(pile[current_var_indice]);
				//printf("--> %d\n", pile[current_var_indice]->affectatio.n);

				// la solution est consistante
				// on copie la variable courante dans la strucuture @variables
				// puis on passe à la variable suivante
				if(consistant == 1)
				{
					variables[current_var_indice] = pile[current_var_indice];
					current_var_indice++;
					break;
				}
			}
			// TODO ici puisque la seule heuristique utilisée réduit les domaines
			// sur les max on peut s'arrêter si un domaine inférieur n'est pas OUVERT
			else
			{
				break;
			}

			//pile[current_var]->affectation++;
		}

		// solution non consistance, on revient en arrière
		if(consistant == 0)
		{
			variables[current_var_indice]->affectation = 0;
			pile[current_var_indice]->affectation = 0;

			// si on est revenu à la première variable, il n'y a pas de solution
			if(current_var_indice == 0)
			{
				free(pile);
				return 0;
			}

			current_var_indice--;
		}
	}

	// libération de la pile
	free(pile);
	// solution trouvée !
	return 1;
}
*/


/**---------------------------------------------------------------------------*
 *                                                                            *
 *----------------------------------------------------------------------------*
int backtrack1()
{
	printf("\n\n");
	s_variable **pile = malloc(nb_variables * sizeof(s_variable*));
	int current_var_indice = 0;
	int consistant;

	//afficher_CSP(variables);

	// tant qu'il reste des variables
	while(current_var_indice < nb_variables)
	{
		//printf("{%d}\n", variables[current_var_indice]->num);
		pile[current_var_indice] = variables[current_var_indice];
		consistant = 0;

		// tant que le domaine de la variable actuelle n'est pas vide
		while(pile[current_var_indice]->affectation < 9)
		{
			nb_noeuds_arbre_recherche++;
			pile[current_var_indice]->affectation++;

			//printf("--> %d\n", pile[current_var_indice]->affectation);

			if(pile[current_var_indice]->domaine[pile[current_var_indice]->affectation-1] == OUVERT)
			{
				//printf("* OK {%d} --> %d*\n", current_var, current_var->affectation);

				//variables[current_var_indice] = pile[current_var_indice];//TODO
				consistant = est_consistant(pile[current_var_indice]);

				// la solution est consistante
				// on copie la variable courante dans la strucuture @variables
				// puis on passe à la variable suivante
				if(consistant)
				{
					variables[current_var_indice] = pile[current_var_indice];
					afficher_CSP(variables);
					current_var_indice++;
					break;
				}
			}
			// TODO ici puisque la seule heuristique utilisée réduit les domaines
			// sur les max on peut s'arrêter si un domaine inférieur est pas OUVERT
			else
			{
				break;
			}
		}

		//
		if(consistant == 0)
		{
			variables[current_var_indice]->affectation = 0;
			pile[current_var_indice]->affectation = 0;

			if(current_var_indice == 0)
			{
				free(pile);
				return 0;
			}

			current_var_indice--;
		}
	}

	// libération de la pile
	free(pile);
	// solution trouvée !
	return 1;
}
*/
