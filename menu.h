#ifndef MENU_H
#define MENU_H

#include <SDL.h>
#include "globales.h"

/* Affichage du menu classique */
extern void accueil(int argc, char *argv[]);

/* Deplace la fleche dans le menu */
extern void fleche_up(sprite_t *sp, int fenetre);
extern void fleche_down(sprite_t *sp, int fenetre);

/* Effectue l'action du bouton */
extern void fleche_action(sprite_t *sp, int *fenetre, SDL_Surface **background);

/* Repartie les differentes actions */
extern void eventact_menu(char *key, sprite_t *sp, int *fenetre,
						  SDL_Surface **background);

/* Trie les scores dans le fichier texte */
extern void trie_score_aux(int *tab, int n);
extern void trie_score();

/* Affiche un nombre a l'ecran */
extern void affiche_nombre(int x, int y, int nb);

/* Affiche les scores a l'ecran */
extern void affiche_score();

#endif /* MENU_H */
