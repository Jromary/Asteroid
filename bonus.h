#ifndef BONUS_H
#define BONUS_H

#include "globales.h"

/* Initialise un bonus/malus aleatoire */
extern void pickable_init(pickable_t *pick, int type, int x, int y);

/* Ajoute un bonus a bonusTab, ayant les coordonnees (x,y) */
extern void plusBonus(int x, int y);

/* Supprime un bonus ayant un id id_cible de bonusTab */
extern void moinsBonus(int id_cible);

/* Execute l'animation d'un bonus */
extern void animBonus(int id_cible);

#endif /* BONUS_H */
