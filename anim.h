#ifndef ANIM_H
#define ANIM_H

#include "globales.h"

/* Ajoute une animation à animTab */
extern void plusAnim(sprite_t cible);

/* Supprime une animation de animTab */
extern void moinsAnim(int id_cible);

/* Execute un cycle d'animation */
extern void anim_cycle();

/* Lance l'animation de mort du vaisseau */
extern sprite_t plusAnimMort(sprite_t cible);

#endif /* ANIM_H */
