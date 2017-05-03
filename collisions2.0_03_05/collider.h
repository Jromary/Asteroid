#ifndef COLLIDER_H
#define COLLIDER_H

#include <stdbool.h>
#include "variables.h"

extern bool collide_test(Sprite* sprite1, Sprite* sprite2, SDL_PixelFormat* format, int * cu, int * cv);

#endif /* COLLIDER_H */
