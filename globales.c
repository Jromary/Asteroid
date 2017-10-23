#include "globales.h"
#include <SDL.h>

SDL_Surface *screen;

int gameover = 0;
int gameover_menu = 0;
int jeu = 0;

sprite_t asteroidTab[ASTEROID_MAX];
int nbAsteroid = 0;

sprite_t shootTab[SHOOT_MAX];
int nbShoot = 0;

sprite_t animTab[ANIM_MAX];
int nbAnim = 0;

int press_again = 1;
int press_again1 = 1;
int press_again2 = 1;
int press_again3 = 1;

int score = 0;

int nb_vie = 3;

pickable_t pickableTab[PICKABLE_MAX];
int nbBonus = 0;
int invincible = 0;
int tripletir = 0;
int confuse = 0;
