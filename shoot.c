/***********************************************/
/********* FONCTIONS SUR LES SHOOTS ************/
/***********************************************/

#include <SDL.h>
#include <math.h>
#include "globales.h"
#include "shoot.h"
#include "sprite.h"

extern int nbShoot;
extern sprite_t shootTab[SHOOT_MAX];

extern SDL_Surface *screen;

void sprite_shoot_init(sprite_t *sp1, sprite_t *sp2)
{
	SDL_Surface *temp, *shootImage;
	point_reel_t npr;
	point_reel_t posr;
	SDL_Rect plop;
	npr.rx = 0;
	npr.ry = 0;
	point_reel_t npr2;
	npr2.rx = 0;
	npr2.ry = 0;
	temp = SDL_LoadBMP("bullet02.bmp");
	shootImage = SDL_DisplayFormat(temp);
	SDL_FreeSurface(temp);
	posr.rx = (*sp2).pr.rx + (ship_size) / 2;
	posr.ry = (*sp2).pr.ry + (ship_size) / 2;
	plop.x = (int)posr.rx;
	plop.y = (int)posr.ry;
	int colorkey = SDL_MapRGB(screen->format, 255, 125, 0);
	sprite_init(sp1, posr, plop, (*sp2).angle, 0.0, 0.0, npr2, npr, shootImage, bullet_size, colorkey);
	sprite_boost(sp1, BOOST_SHOOT);
}
void sprite_shoot_init_multi(sprite_t *sp1, sprite_t *sp2, int differenceAngle)
{
	SDL_Surface *temp, *shootImage;
	point_reel_t npr;
	point_reel_t posr;
	SDL_Rect plop;
	npr.rx = 0;
	npr.ry = 0;
	point_reel_t npr2;
	npr2.rx = 0;
	npr2.ry = 0;
	temp = SDL_LoadBMP("bullet02.bmp");
	shootImage = SDL_DisplayFormat(temp);
	SDL_FreeSurface(temp);
	posr.rx = (*sp2).pr.rx + (ship_size) / 2;
	posr.ry = (*sp2).pr.ry + (ship_size) / 2;
	plop.x = (int)posr.rx;
	plop.y = (int)posr.ry;
	int colorkey = SDL_MapRGB(screen->format, 255, 125, 0);
	sprite_init(sp1, posr, plop, ((*sp2).angle + differenceAngle) % 36, 0.0, 0.0,
             npr2, npr, shootImage, bullet_size, colorkey);
	sprite_boost(sp1, BOOST_SHOOT);
}
void sprite_shoot(sprite_t *sp)
{
	if (nbShoot < SHOOT_MAX)
	{
		sprite_t shoot;
		sprite_shoot_init(&shoot, sp);
		shootTab[nbShoot] = shoot;
		nbShoot++;
	}
	else
	{
		printf("max shoot\n");
	}
	return;
}

void sprite_multi_shoot(sprite_t *sp)
{
	int diffAngle = -2;

	for (int i = 0; i < 3; i++)
	{
		if (nbShoot < SHOOT_MAX)
		{
			sprite_t shoot;
			sprite_shoot_init_multi(&shoot, sp, diffAngle);
			shootTab[nbShoot] = shoot;
			nbShoot++;
			diffAngle += 2;
		}
		else
		{
			printf("max shoot\n");
		}
	}

	return;
}

void moinsShoot(int id_cible)
{
	if (nbShoot == 0)
	{
		printf("ERREUR : appel de moinsShoot alors que le tableau est vide\n");
		printf("Aucun shoot a supprimer.\n");
	}
	else
	{
		SDL_FreeSurface(shootTab[id_cible].image);
		int i;
		for (i = id_cible; i < nbShoot - 1; i++)
		{
			shootTab[i] = shootTab[i + 1];
		}
		nbShoot--;
	}
	return;
}

void exit_shoot()
{
	int i;
	for (i = 0; i < nbShoot; i++)
	{
		if (fabs(shootTab[i].speed.rx) < 0.5 && fabs(shootTab[i].speed.ry) < 0.5)
		{
			moinsShoot(i);
		}
	}
}
