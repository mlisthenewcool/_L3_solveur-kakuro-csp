#include <stdio.h>
#include <stdlib.h>

#include "util.h"
#include "extern.h"

/**---------------------------------------------------------------------------*
 * Affiche le CSP.                                                            *
 *----------------------------------------------------------------------------*
void afficher_CSP(s_variable** vars)
{
	for(int i = 0; i < nb_variables; i++)
	{
		printf("{%2d}\t(%d)\t(%d) [ ", vars[i]->num, vars[i]->affectation, vars[i]->domaine_length);

		for(int j = 0; j < TAILLE_DOMAINE; j++)
		{
			vars[i]->domaine[j] == OUVERT ? printf("%d ", j+1) : printf("  ");
		}

		printf("]\n\t(%d) [ ", vars[i]->contrainte_somme_H->arite);
		for(int j = 0; j < variables[i]->contrainte_somme_H->arite; j++)
		{
			printf("%d ", variables[i]->contrainte_somme_H->portee[j]);
		}
		printf("]\n");

		printf("\t(%d) [ ", vars[i]->contrainte_somme_V->arite);
		for(int j = 0; j < variables[i]->contrainte_somme_V->arite; j++)
		{
			printf("%d ", variables[i]->contrainte_somme_V->portee[j]);
		}
		printf("]\n");

		printf("\t(%d) [ ", vars[i]->contrainte_difference->arite);
		for(int j = 0; j < variables[i]->contrainte_difference->arite; j++)
		{
			printf("%d ", variables[i]->contrainte_difference->portee[j]);
		}
		printf("]\n");
	}
}
*/

int get_nouvelle_place_from_num(int num)
{
	for(int i = 0; i < nb_variables; i++)
	{
		if(variables[i]->num == num)
		{
			return i;
		}
	}

	return -1;
}

int get_num_from_nouvelle_place(int nouvelle_place)
{
	return variables[nouvelle_place]->num;
}

void reorganiser_indices_contraintes(s_variable** vars)
{
	int acces_variables[nb_variables];
	int i, j;

	for(i = 0; i < nb_variables; i++)
	{
		acces_variables[i] = get_nouvelle_place_from_num(i);
	}

	for(i = 0; i < nb_variables; i++)
	{
		for(j = 0; j < variables[i]->contrainte_difference->arite; j++)
		{
			variables[i]->contrainte_difference->portee[j] = acces_variables[variables[i]->contrainte_difference->portee[j]];
		}

		for(j = 0; j < variables[i]->contrainte_somme_H->arite; j++)
		{
			variables[i]->contrainte_somme_H->portee[j] = acces_variables[variables[i]->contrainte_somme_H->portee[j]];
		}

		for(j = 0; j < variables[i]->contrainte_somme_V->arite; j++)
		{
			variables[i]->contrainte_somme_V->portee[j] = acces_variables[variables[i]->contrainte_somme_V->portee[j]];
		}
	}
}

/**---------------------------------------------------------------------------*
 * Affiche le CSP.                                                            *
 *----------------------------------------------------------------------------*/
void afficher_CSP(s_variable** vars)
{
	for(int i = 0; i < nb_variables; i++)
	{
		printf("{%3d}\t(%d)\t[ ", vars[i]->num, vars[i]->affectation);
		for(int j = 0; j < 9; j++)
		{
			vars[i]->domaine[j] == OUVERT ? printf("%d ", j+1) : printf("  ");
		}
		printf("] (%d)\t(sum_H:%d) (sum_V:%d) (diff:%d)\n", vars[i]->domaine_length, vars[i]->contrainte_somme_H->arite, vars[i]->contrainte_somme_V->arite, vars[i]->contrainte_difference->arite);

		/*
		for(int j = 0; j < variables[i]->contrainte_somme_H->arite; j++)
		{
			printf("%d ", variables[i]->contrainte_somme_H->portee[j]);
		}
		printf("\n");

		for(int j = 0; j < variables[i]->contrainte_somme_V->arite; j++)
		{
			printf("%d ", variables[i]->contrainte_somme_V->portee[j]);
		}
		printf("\n");

		for(int j = 0; j < variables[i]->contrainte_difference->arite; j++)
		{
			printf("%d ", variables[i]->contrainte_difference->portee[j]);
		}
		printf("\n");
		*/
	}
}

/**---------------------------------------------------------------------------*
 * Affiche le @message d'erreur donn?? en param??tre puis termine le programme. *
 *----------------------------------------------------------------------------*/
void erreur(const char* message)
{
	fprintf(stderr, "(ERREUR) %s\n", message);
	nettoyage_memoire(); // pas vraiment ce qu'il faudrait faire mais plus rapide
	exit(1);
}

/**---------------------------------------------------------------------------*
 * Affiche le @message d'erreur donn?? en param??tre puis termine le programme. *
 * Le message peut contenir un %s variable, @argument donn?? en param??tre.     *
 *----------------------------------------------------------------------------*/
void erreur_1s(const char* message, const char* argument)
{
	fprintf(stderr, "(ERREUR) ");
	fprintf(stderr, message, argument);
	fprintf(stderr, "\n");
	nettoyage_memoire(); // pas vraiment ce qu'il faudrait faire mais plus rapide
	exit(1);
}

/**---------------------------------------------------------------------------*
 * Retourne le minimum entre deux valeurs enti??res.                           *
 * Retourne le premier en cas d'??galit??.                                      *
 *----------------------------------------------------------------------------*/
int min(int val_1, int val_2)
{
	return val_1 <= val_2 ? val_1 : val_2;
}

/**---------------------------------------------------------------------------*
 *                                                                            *
 *----------------------------------------------------------------------------*/
void filtrer_domaines()
{
	int minimum;
	int j;

	// pour toutes les variables...
	for(int i = 0; i < nb_variables; i++)
	{
		// ...on trouve la somme maximale entre le mot horizontal et vertical...
		// cas 1 o?? la variable appartient ?? deux mots
		if(variables[i]->contrainte_somme_H->somme != 0 && variables[i]->contrainte_somme_V->somme != 0)
		{
			minimum = min(variables[i]->contrainte_somme_H->somme, variables[i]->contrainte_somme_V->somme);
		}
		// cas 2 o?? la variable appartient ?? un mot horizontal
		else if(variables[i]->contrainte_somme_H->somme == 0)
		{
			minimum = variables[i]->contrainte_somme_V->somme;
		}
		// cas 3 o?? la variable appartient ?? un mot vertical
		else if(variables[i]->contrainte_somme_V->somme == 0)
		{
			minimum = variables[i]->contrainte_somme_H->somme;
		}
		// cas 4 o?? la variable n'appartient ?? aucun mot ?
		else
		{
			minimum = TAILLE_DOMAINE;
		}

		j = minimum;

		// ...puis on retire les valeurs sup??rieures du domaine...
		while(j < TAILLE_DOMAINE)
		{
			variables[i]->domaine[j] = FERME;
			j++;
		}

		// ...pour enfin mettre ?? jour la taille du domaine
		if(minimum < 9)
		{
			variables[i]->domaine_length = (TAILLE_DOMAINE - (TAILLE_DOMAINE - minimum));
		}
	}
}

/**---------------------------------------------------------------------------*
 *                                                                            *
 *----------------------------------------------------------------------------*/
int test_contrainte_difference(s_variable* var)
{
	//printf("test contrainte difference\n");

	// pour toutes les contraintes de var...
	for(int i = 0; i < var->contrainte_difference->arite; i++)
	{
		nb_tests_contrainte++;

		//indice = get_var_from_num(var->contrainte_difference->portee[i]);

		// ...si la variable contenue dans la contrainte est instanci??e...
		if(variables[var->contrainte_difference->portee[i]]->affectation != 0)
		{
			// ...on regarde qu'elle soit bien diff??rente
			if(variables[var->contrainte_difference->portee[i]]->affectation == var->affectation)
			{
				//printf("valeur identique ?? {%d}\n", get_num_from_nouvelle_place(var->contrainte_difference->portee[i]));
				return 0;
			}
		}
	}

	// la contrainte est respect??e
	return 1;
}

/**---------------------------------------------------------------------------*
 *                                                                            *
 *----------------------------------------------------------------------------*/
int test_contrainte_somme_H(s_variable* var)
{
	//printf("test contrainte somme H\n");

	// pas de contrainte
	if(var->contrainte_somme_H->arite == 0)
	{
		return 1;
	}

	// la somme est initialis??e par la valeur de l'affectation de la variable en cours
	int somme = var->affectation;

	// pour toutes les contraintes de somme sur le mot horizontal...
	for(int i = 0; i < var->contrainte_somme_H->arite; i++)
	{
		nb_tests_contrainte++;

		// ...si toutes les valeurs ne sont pas encore instanci??es on sort
		//TODO on a d??j?? d??pass?? la somme
		if(variables[var->contrainte_somme_H->portee[i]]->affectation == 0)
		{
			//printf("{%d} pas instanci??e\n", get_num_from_nouvelle_place(var->contrainte_somme_H->portee[i]));
			return 1;
		}
		// ...on effectue l'addition
		somme += variables[var->contrainte_somme_H->portee[i]]->affectation;

		// ...la somme a ??t?? d??pass??e
		if(somme > var->contrainte_somme_H->somme)
		{
			//printf("somme %d d??pass??e\n", var->contrainte_somme_H->somme);
			return 0;
		}
	}

	// la somme est-elle exactement ??gale ?
	return var->contrainte_somme_H->somme == somme;
}

/**---------------------------------------------------------------------------*
 *                                                                            *
 *----------------------------------------------------------------------------*/
int test_contrainte_somme_V(s_variable* var)
{
	//printf("test contrainte somme V\n");

	// pas de contrainte
	if(var->contrainte_somme_V->arite == 0)
	{
		return 1;
	}

	//int indice;

	// la somme est initialis??e par la valeur de l'affectation de la variable en cours
	int somme = var->affectation;

	// pour toutes les contraintes de somme sur le mot vertical...
	for(int i = 0; i < var->contrainte_somme_V->arite; i++)
	{
		nb_tests_contrainte++;

		//indice = get_var_from_num(var->contrainte_somme_V->portee[i]);

		// ...si toutes les valeurs ne sont pas encore instanci??es on sort
		//TODO on a d??j?? d??pass?? la somme
		if(variables[var->contrainte_somme_V->portee[i]]->affectation == 0)
		{
			//printf("{%d} pas instanci??e\n", get_num_from_nouvelle_place(var->contrainte_somme_V->portee[i]));
			return 1;
		}
		// ...on effectue l'addition
		somme += variables[var->contrainte_somme_V->portee[i]]->affectation;

		// ...la somme a ??t?? d??pass??e
		if(somme > var->contrainte_somme_V->somme)
		{
			//printf("somme %d d??pass??e\n", var->contrainte_somme_V->somme);
			return 0;
		}
	}

	// la somme est-elle exactement ??gale ?
	return var->contrainte_somme_V->somme == somme;
}

/**---------------------------------------------------------------------------*
 *                                                                            *
 *----------------------------------------------------------------------------*/
int test_contrainte_somme(s_variable* var)
{
	return test_contrainte_somme_H(var) && test_contrainte_somme_V(var);
}

/**---------------------------------------------------------------------------*
 *                                                                            *
 *----------------------------------------------------------------------------*/
int est_consistant(s_variable* var)
{
	return test_contrainte_difference(var) && test_contrainte_somme(var);
}

/**---------------------------------------------------------------------------*
 *                                                                            *
 *----------------------------------------------------------------------------*/
void nettoyage_memoire()
{
	printf("--------------------------------------------------\n");
	printf("Nettoyage m??moire...\n");

	// variables
	printf("\tvariables\n");
	for(int i = 0; i < nb_variables; i++)
	{
		free(variables[i]->contrainte_somme_H->portee);
		free(variables[i]->contrainte_somme_V->portee);
		free(variables[i]->contrainte_difference->portee);

		free(variables[i]->contrainte_somme_H);
		free(variables[i]->contrainte_somme_V);
		free(variables[i]->contrainte_difference);

		free(variables[i]);
	}
	free(variables);

	// grille
	printf("\tgrille\n");
	for (int num_ligne = 0; num_ligne < nb_lignes; num_ligne++)
	{
		free(grille[num_ligne]);
	}
	free(grille);

	printf("--------------------------------------------------\n");
}
