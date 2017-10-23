#ifndef ASTEROID_H
#define ASTEROID_H

#include "globales.h"

/* Recupere un objet sprite et l'initialise en asteroid */
extern void asteroid_init(sprite_t *sp);

/*  Ajoute un asteroid au tableau d'asteroid global et incremente le nombre
d'asteroid présent */
extern void plusAsteroid();

/* Retire un asteroid ayant l'id id_cible du tableau d'asteroid global */
extern void moinsAsteroid(int id_cible);

/* Déplacement d'un sprite sans frottement (ne s'arrete jamais) */
extern void asteroid_move(sprite_t *sp);

/* Fonction de rotation des asteroid (particuliere à ceux-ci car animés sur 32
frames et pas 36) */
extern void asteroid_turn_right(sprite_t *sp);

/* Fonction pour aider le développement */
extern void affichagePositionAsteroid();

/* Fonction qui ajoute entre 1 et 3 fragments de la moitié de la taille de la cible */
extern void fragmentationAsteroid(int id_cible);

/* Appel fragmentationAsteroid et supprime l'asteroid ciblé */
extern void splitAsteroid();

#endif /* ASTEROID_H */
