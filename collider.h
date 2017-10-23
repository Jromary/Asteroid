#ifndef COLLIDER_H
#define COLLIDER_H

#include <stdbool.h>
#include "globales.h"

/* Verifie si un sprite est superposé avec un autre */
extern bool collide_test(sprite_t *sprite1, sprite_t *sprite2, SDL_PixelFormat *format, int *cu, int *cv);

#endif /* COLLIDER_H */
