#ifndef VARIABLES_H
#define VARIABLES_H

#define screen_height 480
#define screen_length 640

#define PI 3.1416

#define COEFF_FROTTEMENT 0.001
#define COEFF_BOOST 0.01

#define ASTEROID_MAX 1000

#define size_asteroid_min 16
#define size_asteroid_max 64
#define size_asteroid_med 32
#define ship_size 32
#define ship_angle 0

#define bullet_size 8
#define SHOOT_MAX 1000


#include <SDL.h>

struct Pointreel
{
	float rx;
	float ry;
};
typedef struct Pointreel Pointreel;

struct Sprite
{
	Pointreel pr;
	SDL_Rect pi;
	int angle;
	Pointreel accel;
	Pointreel speed;
	SDL_Surface image;
	int spritetaille;
};
typedef struct Sprite Sprite;



#endif /* VARIABLES_H */