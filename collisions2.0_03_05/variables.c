#include "variables.h"
#include <SDL.h>
int gameover = 0;
Sprite asteroidTab[ASTEROID_MAX];
int nbAsteroid = 0;
Sprite shootTab[SHOOT_MAX];
int nbShoot = 0;
Sprite animTab[ANIM_MAX];
int nbAnim = 0;
int shoot_again = 1;
SDL_Surface* screen;
float ship_angle = 0.0;