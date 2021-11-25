#ifndef __PARSEUR_H
#define __PARSEUR_H

#define TAILLE_DOMAINE 9

typedef struct contrainte_somme			s_contrainte_somme;
typedef struct contrainte_difference	s_contrainte_difference;
typedef struct variable					s_variable;

/**---------------------------------------------------------------------------*
 * Les deux couleurs possibles pour une case.                                 *
 *                                                                            *
 * @BLANCHE	: devra être remplie par un chiffre de 1 à 9.                     *
 * @NOIRE	: Étant donnés deux entiers x et y, ”x\y” indique que le mot      *
 *            vertical aura pour somme x et le mot horizontal pour somme y.   *
 *            ”\” représentera donc une case noire sans chiffre associé.      *
 *            ”x\” et ”\y” représenteront des cases noires ne contenant la    *
 *            définition que d’un mot respectivement vertical et horizontal   *
 *----------------------------------------------------------------------------*/
enum e_couleur
{
	BLANCHE,
	NOIRE
};

/**---------------------------------------------------------------------------*
 * Une simple implémentation d'un type booléen afin de représenter de façon   *
 * explicite le domaine d'une variable.                                       *
 *                                                                            *
 * @OUVERT	:                                                                 *
 * @FERME	:                                                                 *
 *----------------------------------------------------------------------------*/
enum e_domaine
{
	OUVERT,
	FERME
};

/**---------------------------------------------------------------------------*
 * La structure représentant une case.                                        *
 *                                                                            *
 * @couleur				: la nature de la case.                               *
 * @num					: le numéro de la case s'il s'agit d'une case noire.  *
 * @somme_horizontale	: la valeur de la somme horizontale le cas écheant,   *
 *                        -1 sinon.                                           *
 * @somme_verticale		: la valeur de la somme verticale le cas écheant,     *
 *                        -1 sinon.                                           *
 *----------------------------------------------------------------------------*/
typedef struct
{
	enum e_couleur couleur;
	int num;
	int somme_horizontale;
	int somme_verticale;
} s_case;

/**---------------------------------------------------------------------------*
 * La structure représentant une variable.                                    *
 *                                                                            *
 * @num			       : le numéro de la variable                             *
 * @affectation	       : l'affectation courante de la variable                *
 * @domaine		       : le domaine de résolution                             *
 * @contrainte_somme_H :                                                      *
 * @contrainte_somme_V :                                                      *
 *----------------------------------------------------------------------------*/
struct variable
{
	int num;
	int affectation;
	int domaine_length;
	enum e_domaine domaine[TAILLE_DOMAINE];
	s_contrainte_somme* contrainte_somme_H;
	s_contrainte_somme* contrainte_somme_V;
	s_contrainte_difference* contrainte_difference;
};

/**---------------------------------------------------------------------------*
 * La structure représentant une contraine de différence.                     *
 *                                                                            *
 * @type   : //TODO                                                           *
 * @portee : //TODO                                                           *
 * @taille : //TODO                                                           *
 * @somme  : //TODO                                                           *
 *----------------------------------------------------------------------------*/
struct contrainte_difference
{
	int arite;
	int* portee;
};

/**---------------------------------------------------------------------------*
 * La structure représentant une contraine de somme.                          *
 *                                                                            *
 * @portee : //TODO                                                           *
 * @taille : //TODO                                                           *
 * @somme  : //TODO                                                           *
 *----------------------------------------------------------------------------*/
struct contrainte_somme
{
	int somme;
	int arite;
	int* portee;
};

/**---------------------------------------------------------------------------*
 * TODO                                                                       *
 *----------------------------------------------------------------------------*/
void parser(char* nom_fichier);

/**---------------------------------------------------------------------------*
 * Fonction permettant la création d'une nouvelle variable de numéro @num.    *
 *----------------------------------------------------------------------------*/
void creer_variable(int num);

/**---------------------------------------------------------------------------*
 * Fonction permettant la création d'une nouvelle contrainte binaire de       *
 * différence entre les variables @var1 et @var2.                             *
 *----------------------------------------------------------------------------*/
void creer_contrainte_difference(int var1, int var2);

/**---------------------------------------------------------------------------*
 * Fonction permettant la création d'une nouvelle contrainte n-aire de somme  *
 * portant sur les variables contenues dans le tableau @portee de taille      *
 * @arite et dont la valeur est @val                                          *
 *----------------------------------------------------------------------------*/
void creer_contrainte_somme_horizontale(int portee[], int arite, int val);

/**---------------------------------------------------------------------------*
 * Fonction permettant la création d'une nouvelle contrainte n-aire de somme  *
 * portant sur les variables contenues dans le tableau @portee de taille      *
 * @arite et dont la valeur est @val                                          *
 *----------------------------------------------------------------------------*/
void creer_contrainte_somme_verticale(int portee[], int arite, int val);

#endif
