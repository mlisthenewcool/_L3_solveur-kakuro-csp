#ifndef __EXTERN_H
#define __EXTERN_H

#include "parseur.h"

/* la grille de jeu */
extern int nb_lignes;
extern int nb_colonnes;
extern s_case** grille;

/* les variables */
extern int nb_variables;
extern s_variable** variables;

/* statistiques */
extern int nb_tests_contrainte;
extern int nb_noeuds_arbre_recherche;

#endif
