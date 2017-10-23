#ifndef GLOBALES_H
#define GLOBALES_H

/* Taille de la fenetre */
#define screen_height 720
#define screen_length 1280

/* Physique globale */
#define COEFF_FROTTEMENT 0.001
#define COEFF_BOOST 0.003

/* Asteroids */
#define ASTEROID_MAX 100
#define asteroid_rota 0.007
#define ASTEROID_SPEED 500
#define DELAI_APPARITION_ASTEROID 3
#define size_asteroid_min 16
#define size_asteroid_max 64
#define size_asteroid_med 32

/* Vaisseau */
#define ship_size 32
#define ship_angle 0
#define ship_rotation 0.04

/* Shoot */
#define bullet_size 8
#define SHOOT_MAX 250
#define BOOST_SHOOT 1

/* Animation */
#define ANIM_MAX 10

/* Bonus - Malus */
#define PICKABLE_MAX 10
#define DELAI_INVINCIBILITE 5
#define DELAI_TRIPLETIR 5
#define DELAI_CONFUSE 5

/* Autre */
#define PI 3.1416
#define NBPLAYERS 1

#include <SDL.h>

struct s_point_reel
{
	float rx;
	float ry;
};
typedef struct s_point_reel point_reel_t;

struct s_sprite
{
	point_reel_t pr;
	SDL_Rect pi;
	int angle;
	float anglereel;
	float rotationspeed;
	point_reel_t accel;
	point_reel_t speed;
	SDL_Surface *image;
	int spritetaille;
	int colorkey;
};
typedef struct s_sprite sprite_t;

struct s_pickable
{
	sprite_t sprite;
	int type;
};
typedef struct s_pickable pickable_t;

#endif /* GLOBALES_H */
