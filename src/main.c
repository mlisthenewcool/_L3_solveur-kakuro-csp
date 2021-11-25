#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "util.h"
#include "parseur.h"
#include "backtrack.h"
#include "extern.h"

// initialisation des compteurs statistiques pour mesurer les performances
int nb_tests_contrainte = 0;
int nb_noeuds_arbre_recherche = 0;

/**---------------------------------------------------------------------------*
 * Simple affichage des affectations sous forme de grille.                    *
 *----------------------------------------------------------------------------*/
void afficher_grille_solution()
{
	int current_var = 0;

	for(int num_ligne = 0; num_ligne < nb_lignes; num_ligne++)
	{
		for(int num_colonne = 0; num_colonne < nb_colonnes; num_colonne++)
		{
			// on grise les cases noires
			if(grille[num_ligne][num_colonne].couleur == NOIRE)
			{
				printf("\e[30;40m");
				printf("   ");
				printf("\e[0m");
			}
			// on affiche l'affectation des cases blanches
			else
			{
				printf("%3d", variables[get_num_from_nouvelle_place(current_var++)]->affectation);
			}
		}
		printf("\n");
	}
}

/**---------------------------------------------------------------------------*
 *                                                                            *
 *----------------------------------------------------------------------------*/
int cmp_taille_domaine(const void* var_1, const void* var_2)
{
	s_variable* s_var_1 = *(s_variable* const*) var_1;
	s_variable* s_var_2 = *(s_variable* const*) var_2;

	return (s_var_1->domaine_length - s_var_2->domaine_length);
}

/**---------------------------------------------------------------------------*
 *                                                                            *
 *----------------------------------------------------------------------------*/
int main(int argc, char** argv)
{
	// usage de l'exécutable
	if (argc != 2)
	{
		erreur_1s("usage : %s <grille>", argv[0]);
	}

	// fonction de lecture du fichier d'entrée permettant d'initialiser la
	// grille de jeu ainsi que le CSP
	parser(argv[1]);

	// filtrage des domaines
	filtrer_domaines();

	// tri des variables par taille des domaines
	//qsort(variables, nb_variables, sizeof(s_variable*), cmp_taille_domaine);

	//WARNING
	//reorganiser_indices_contraintes(variables);

	// affichage du CSP
	afficher_CSP(variables);

	// résolution par backtrack avec calcul du temps d'exécution
	clock_t debut, fin;
	debut = clock();
	int resultat = backtrack();
	fin = clock();

	afficher_CSP(variables);

	if(resultat)
	{
		printf("Solution trouvée.\n");
		afficher_grille_solution();
	}
	else
	{
		printf("Pas de solution.\n");
	}

	// affichage des statistiques
	printf("--------------------------------------------------\n");
	printf("Temps d'exécution : %lf\n", (double)(fin-debut)/CLOCKS_PER_SEC);
	printf("Nombre de tests de contrainte : %d\n", nb_tests_contrainte);
	printf("Nombre de noeuds de l'arbre de recherche : %d\n", nb_noeuds_arbre_recherche);

	// nettoyage de toutes les structures allouées dynamiquement
	nettoyage_memoire();

	return 0;
}
