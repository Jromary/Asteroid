/***********************************************/
/********* FONCTIONS SUR LES SPRITES ***********/
/***********************************************/

#include <SDL.h>
#include <math.h>
#include "globales.h"
#include "sprite.h"

void sprite_init(sprite_t *sp, point_reel_t npr, SDL_Rect npi, int ang, float angre,
				 float rota, point_reel_t spee, point_reel_t acc, SDL_Surface *surface,
				 int spriteta, int colork)
{
	(*sp).pr.rx = npr.rx;
	(*sp).pr.ry = npr.ry;
	(*sp).pi.x = npi.x;
	(*sp).pi.y = npi.y;
	(*sp).angle = ang;
	(*sp).anglereel = angre;
	(*sp).rotationspeed = rota;
	(*sp).accel.rx = acc.rx;
	(*sp).accel.ry = acc.ry;
	(*sp).speed.rx = spee.rx;
	(*sp).speed.ry = spee.ry;
	(*sp).image = surface;
	(*sp).spritetaille = spriteta;
	(*sp).colorkey = colork;
}

void sprite_turn_right(sprite_t *sp)
{
	if ((*sp).anglereel > 0)
	{
		(*sp).anglereel -= (*sp).rotationspeed;
		(*sp).angle = (int)(*sp).anglereel;
	}
	else
	{
		(*sp).angle = 35;
		(*sp).anglereel = 35;
	}
}

void sprite_turn_left(sprite_t *sp)
{
	if ((*sp).anglereel < 35)
	{
		(*sp).anglereel += (*sp).rotationspeed;
		(*sp).angle = (int)(*sp).anglereel;
	}
	else
	{
		(*sp).angle = 0;
		(*sp).anglereel = 0;
	}
}
void asteroid_turn_right(sprite_t *sp)
{
	if ((*sp).anglereel > 0)
	{
		(*sp).anglereel -= (*sp).rotationspeed;
		(*sp).angle = (int)(*sp).anglereel;
	}
	else
	{
		(*sp).angle = 31;
		(*sp).anglereel = 31;
	}
}
void sprite_move(sprite_t *sp)
{
	(*sp).speed.rx += (*sp).accel.rx - (*sp).speed.rx * COEFF_FROTTEMENT;
	(*sp).speed.ry += (*sp).accel.ry - (*sp).speed.ry * COEFF_FROTTEMENT;
	(*sp).accel.rx = 0;
	(*sp).accel.ry = 0;
	(*sp).pr.rx += (*sp).speed.rx;
	(*sp).pr.ry += (*sp).speed.ry;
	(*sp).pi.x = (int)(*sp).pr.rx;
	(*sp).pi.y = (int)(*sp).pr.ry;
}

void sprite_boost(sprite_t *sp, float boost)
{
	float tmp;
	tmp = (((*sp).angle) * 10) * (PI / 180.0);
	(*sp).accel.rx = cos(tmp) * boost;
	(*sp).accel.ry = -sin(tmp) * boost;
}

void exitscreen(sprite_t *sp)
{
	if ((*sp).pr.rx > (screen_length - (*sp).spritetaille))
	{
		(*sp).pr.rx = 0;
	}
	if ((*sp).pr.rx < 0)
	{
		(*sp).pr.rx = screen_length - (*sp).spritetaille;
	}
	if ((*sp).pr.ry > (screen_height - (*sp).spritetaille))
	{
		(*sp).pr.ry = 0;
	}
	if ((*sp).pr.ry < 0)
	{
		(*sp).pr.ry = screen_height - (*sp).spritetaille;
	}
}
