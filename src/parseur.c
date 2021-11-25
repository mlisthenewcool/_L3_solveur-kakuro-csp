#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#include "parseur.h"
#include "util.h"

/**---------------------------------------------------------------------------*
 *                                                                            *
 *----------------------------------------------------------------------------*/
s_variable** variables;	// la structure contenant toutes les variables
int nb_variables;		// le nombre de variables déjà trouvées

int nb_lignes;			// le nombre de lignes de la grille
int nb_colonnes;		// le nombre de colonnes de la grille
s_case** grille;		// la grille de jeu

/**---------------------------------------------------------------------------*
 *                                                                            *
 *----------------------------------------------------------------------------*/
void initialiser_contrainte_somme(int var)
{
	variables[var]->contrainte_somme_H = malloc(sizeof(s_contrainte_somme));
	if (variables[var]->contrainte_somme_H == NULL)
	{
		erreur("variables[i]->contrainte_somme_H");
	}

	variables[var]->contrainte_somme_H->arite = 0;
	variables[var]->contrainte_somme_H->somme = 0;
	variables[var]->contrainte_somme_H->portee = malloc((TAILLE_DOMAINE-1) * sizeof(int));
	if (variables[var]->contrainte_somme_H->portee == NULL)
	{
		erreur("variables[i]->contrainte_somme_H->portee");
	}
	
	variables[var]->contrainte_somme_V = malloc(sizeof(s_contrainte_somme));
	if (variables[var]->contrainte_somme_V == NULL)
	{
		erreur("variables[i]->contrainte_somme_V");
	}
	variables[var]->contrainte_somme_V->arite = 0;
	variables[var]->contrainte_somme_V->somme = 0;
	variables[var]->contrainte_somme_V->portee = malloc((TAILLE_DOMAINE-1) * sizeof(int));
	if (variables[var]->contrainte_somme_V->portee == NULL)
	{
		erreur("variables[i]->contrainte_somme_V->portee");
	}
	
	for(int i = 0; i < TAILLE_DOMAINE-1; i++)
	{
		variables[var]->contrainte_somme_H->portee[i] = -1;
		variables[var]->contrainte_somme_V->portee[i] = -1;
	}
}

/**---------------------------------------------------------------------------*
 *                                                                            *
 *----------------------------------------------------------------------------*/
void initialiser_contrainte_difference(int var)
{
	variables[var]->contrainte_difference = malloc(sizeof(s_contrainte_difference));
	if (variables[var]->contrainte_difference == NULL)
	{
		erreur("variables[i]->contrainte_difference");
	}
	variables[var]->contrainte_difference->arite = 0;
	variables[var]->contrainte_difference->portee = malloc(((TAILLE_DOMAINE-1)*2) * sizeof(int));
	if (variables[var]->contrainte_difference->portee == NULL)
	{
		erreur("variables[i]->contrainte_somme_H");
	}
	
	for(int i = 0; i < (TAILLE_DOMAINE-1)*2; i++)
	{
		variables[var]->contrainte_difference->portee[i] = -1;
	}
}

/**---------------------------------------------------------------------------*
 *                                                                            *
 *----------------------------------------------------------------------------*/
void initialiser_structure_variables()
{
	variables = malloc(nb_variables * sizeof(s_variable*));
	if (variables == NULL)
	{
		erreur("malloc structure variables");
	}

	for(int i = 0; i < nb_variables; i++)
	{
		variables[i] = malloc(sizeof(s_variable));
		if (variables[i] == NULL)
		{
			erreur("malloc structure variables[i]");
		}

		variables[i]->num = i;
		variables[i]->affectation = 0;
		variables[i]->domaine_length = TAILLE_DOMAINE;
		for(int j = 0; j < TAILLE_DOMAINE; j++)
		{
			variables[i]->domaine[j] = OUVERT;
		}
		initialiser_contrainte_somme(i);
		initialiser_contrainte_difference(i);
	}
}

/**---------------------------------------------------------------------------*
 *                                                                            *
 *----------------------------------------------------------------------------*/
void parser(char* nom_fichier)
{
	FILE* f;			// le descripteur du fichier à lire
	int c;				// le caractère courant

	int num_ligne;		// le numéro de la ligne courante
	int num_colonne;	// le numero de la colonne courante
	int somme;			// une somme
	
	int* portee;		// la portée d'une contrainte
	int arite;			// l'arité d'une contrainte

	int i, j;			// des compteurs de boucles


	/** ouverture du fichier @nom_fichier en lecture */
	f = fopen(nom_fichier, "r");
	if (f == NULL)
	{
		erreur_1s("fopen %s en lecture.", nom_fichier);
	}

	/** calcul de la taille de la grille */
	nb_lignes = 0;
	nb_colonnes = 0;

	do
	{
		c = fgetc(f);

		if (c == '\n')
		{
			nb_lignes++;
		}
		else
		{
			if ((nb_lignes == 0) && ((c == '.') || (c == '\\')))
			{
				nb_colonnes++;
			}
		}
	} while ( ! feof(f));

	printf("Taille : %d (lignes) x %d (colonnes).\n", nb_lignes, nb_colonnes);


	/** remplissage de la @grille */
	grille = malloc(sizeof(s_case*) * nb_lignes);
	if (grille == NULL)
	{
		erreur("malloc lignes de la grille.");
	}

	for (num_ligne = 0; num_ligne < nb_lignes; num_ligne++)
	{
		grille[num_ligne] = malloc(sizeof(s_case) * nb_colonnes);
		if (grille[num_ligne] == NULL)
		{
			erreur("malloc lignes de la grille.");
		}
	}

	fseek(f, 0, SEEK_SET); // retourner au début du fichier

	num_ligne = 0;
	num_colonne = 0;
	nb_variables = 0;

	do
	{
		c = fgetc(f);

		if (c == '\n')
		{
			num_ligne++;
			num_colonne = 0;
		}
		else
		{
			if (c == ' ')
			{
				num_colonne++;
			}
			else
			{
				/** case blanche */
				if (c == '.')
				{
					grille[num_ligne][num_colonne].couleur = BLANCHE;
					grille[num_ligne][num_colonne].num = nb_variables;
					grille[num_ligne][num_colonne].somme_horizontale = -1;
					grille[num_ligne][num_colonne].somme_verticale = -1;

					//creer_variable(nb_variables);

					nb_variables++;
				}
				/** case noire */
				else
				{
					/** case noire de la forme \y ou \ */
					if (c == '\\')
					{
						grille[num_ligne][num_colonne].couleur = NOIRE;
						grille[num_ligne][num_colonne].num = -1;
						grille[num_ligne][num_colonne].somme_verticale = -1;

						c = fgetc(f);

						/** case noire de la forme \y */
						if (isdigit (c))
						{
							ungetc(c, f);
							fscanf(f, "%d", &somme);
							grille[num_ligne][num_colonne].somme_horizontale = somme;
						}
						/** case noire de la forme \ */
						else
						{
							ungetc(c, f);
							grille[num_ligne][num_colonne].somme_horizontale = -1;
						}
					}

					else
					{
						/** case noire de la forme x\ ou x\y */
						if (isdigit(c))
						{
							grille[num_ligne][num_colonne].couleur = NOIRE;
							grille[num_ligne][num_colonne].num = -1;

							ungetc(c, f);
							fscanf(f, "%d", &somme);
							grille[num_ligne][num_colonne].somme_verticale = somme;

							c = fgetc(f);	/* lecture du caractère \ */
							c = fgetc(f);

							/* case noire de la forme x\y */
							if (isdigit(c))
							{
								ungetc(c, f);
								fscanf(f, "%d", &somme);
								grille[num_ligne][num_colonne].somme_horizontale = somme;
							}
							/* case noire de la forme x\ */
							else
							{
								ungetc(c, f);
								grille[num_ligne][num_colonne].somme_horizontale = -1;
							}
						}
					}
				}	/* fin case noire */
			}		/* fin case */
		}			/* fin else */
	} while ( ! feof(f));

	printf("Nombre de variables trouvées : %d\n", nb_variables);
	fclose(f);

	/** initialisation de la structure des variables */
	initialiser_structure_variables();

	/** création des contraintes */
	if (nb_lignes < nb_colonnes)
	{
		portee = malloc(sizeof(int) * nb_colonnes);
	}
	else
	{
		portee = malloc(sizeof(int) * nb_lignes);
	}

	if (portee == NULL)
	{
		erreur("malloc portee");
	}

	for (num_ligne = 0; num_ligne < nb_lignes; num_ligne++)
	{
		for (num_colonne = 0; num_colonne < nb_colonnes; num_colonne++)
		{
			if (grille[num_ligne][num_colonne].couleur == NOIRE)
			{
				if (grille[num_ligne][num_colonne].somme_horizontale != -1)
				{
					arite = 0;
					i = num_colonne + 1;
					while ((i < nb_colonnes) && (grille[num_ligne][i].couleur == BLANCHE))
					{
						portee[arite] = grille[num_ligne][i].num;
						arite++;

						j = i + 1;
						while ((j < nb_colonnes) && (grille[num_ligne][j].couleur == BLANCHE))
						{
							creer_contrainte_difference(grille[num_ligne][i].num, grille[num_ligne][j].num);
							j++;
						}
						i++;
					}

					creer_contrainte_somme_horizontale(portee, arite, grille[num_ligne][num_colonne].somme_horizontale);
				}

				if (grille[num_ligne][num_colonne].somme_verticale != -1)
				{
					arite = 0;
					i = num_ligne + 1;
					while ((i < nb_lignes) && (grille[i][num_colonne].couleur == BLANCHE))
					{
						portee[arite] = grille[i][num_colonne].num;
						arite++;
						j = i + 1;
						while ((j < nb_lignes) && (grille[j][num_colonne].couleur == BLANCHE))
						{
							creer_contrainte_difference(grille[i][num_colonne].num, grille[j][num_colonne].num);
							j++;
						}
						i++;
					}

					creer_contrainte_somme_verticale(portee, arite, grille[num_ligne][num_colonne].somme_verticale);
				}
			}
		}
	}


	/** libération de la mémoire allouée pour la portée des contraintes */
	free(portee);
}

/**---------------------------------------------------------------------------*
 *                                                                            *
 *----------------------------------------------------------------------------*/
void creer_contrainte_difference(int var1, int var2)
{
	//printf("Contrainte binaire de différence entre %d et %d\n", var1, var2);
	
	variables[var1]->contrainte_difference->portee[variables[var1]->contrainte_difference->arite++] = var2;
	//realloc(variables[var1]->contrainte_difference->portee, variables[var1]->contrainte_difference->arite * sizeof(s_variable*));
	
	variables[var2]->contrainte_difference->portee[variables[var2]->contrainte_difference->arite++] = var1;
	//realloc(variables[var2]->contrainte_difference->portee, variables[var2]->contrainte_difference->arite * sizeof(s_variable*));
}

/**---------------------------------------------------------------------------*
 *                                                                            *
 *----------------------------------------------------------------------------*/
void creer_contrainte_somme_horizontale(int vars[], int arite, int val)
{
	//printf("Contrainte n-aire de somme_H (%d) portant sur", arite);
	for (int i = 0; i < arite; i++)
	{
		//printf (" %d", vars[i]);
		
		int count = 0;
		for(int j = 0; j < arite; j++)
		{
			if(vars[j] != vars[i])
			{
				variables[vars[i]]->contrainte_somme_H->portee[count] = vars[j];
				count++;
			}
		}
		
		variables[vars[i]]->contrainte_somme_H->arite = arite-1;
		variables[vars[i]]->contrainte_somme_H->somme = val;
		
		//printf(" (%d) ", variables[vars[i]]->contrainte_somme_V->arite);
	}
	//printf(" et de valeur %d\n", val);
}

/**---------------------------------------------------------------------------*
 *                                                                            *
 *----------------------------------------------------------------------------*/
void creer_contrainte_somme_verticale(int vars[], int arite, int val)
{
	//printf("Contrainte n-aire de somme_V (%d) portant sur", arite);
	for (int i = 0; i < arite; i++)
	{
		//printf (" %d", vars[i]);
		
		int count = 0;
		for(int j = 0; j < arite; j++)
		{
			if(vars[j] != vars[i])
			{
				variables[vars[i]]->contrainte_somme_V->portee[count] = vars[j];
				count++;
			}
		}
		
		variables[vars[i]]->contrainte_somme_V->arite = arite-1;
		variables[vars[i]]->contrainte_somme_V->somme = val;
		
		//printf(" (%d) ", variables[vars[i]]->contrainte_somme_V->arite);
	}
	//printf(" et de valeur %d\n", val);
}
