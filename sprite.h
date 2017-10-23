#ifndef SPRITE_H
#define SPRITE_H

#include "globales.h"

/* Fonction d'initialisation d'un objet de type sprite_t */
extern void sprite_init(sprite_t *sp, point_reel_t npr, SDL_Rect npi, int ang,
						float angre, float rota, point_reel_t spee, point_reel_t acc,
						SDL_Surface *surface, int spriteta, int colork);

/* Fonction pour décrementer l'angle d'un sprite_t */
extern void sprite_turn_right(sprite_t *sp);

/* Fonction pour incrémenter l'angle d'un sprite_t */
extern void sprite_turn_left(sprite_t *sp);

/* Modifie la position d'un sprite_t en fonction de sa vitesse et son
   acceleration sur les axes x y */
extern void sprite_move(sprite_t *sp);

/* Modifie l'acceleration d'un sprite_t en fonction d'un paramètre boost */
extern void sprite_boost(sprite_t *sp, float boost);

/* Empeche le dépassage de l'écran pour un sprite_t */
extern void exitscreen(sprite_t *sp);

#endif /* SPRITE_H */
