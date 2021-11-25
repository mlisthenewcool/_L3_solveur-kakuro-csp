#ifndef __UTIL_H
#define __UTIL_H

#include "parseur.h"

// affichage du CSP
void afficher_CSP(s_variable** vars);

// gestion des erreurs
void erreur(const char* msg);
void erreur_1s(const char* msg, const char* arg);

//
int get_num_from_nouvelle_place(int nouvelle_place);
void reorganiser_indices_contraintes(s_variable** vars);

// heuristiques
void filtrer_domaines();

// consistances
int test_contrainte_difference(s_variable* var);
int test_contrainte_somme_H(s_variable* var);
int test_contrainte_somme_V(s_variable* var);
int test_contrainte_somme(s_variable* var);
int est_consistant(s_variable* var);

// gestion mémoire
void nettoyage_memoire();

#endif
