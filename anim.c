/***********************************************/
/********* FONCTIONS SUR LES ANIMATIONS ********/
/***********************************************/

#include <SDL.h>

#include "sprite.h"
#include "globales.h"
#include "anim.h"

extern int nbAnim;
extern sprite_t animTab[ANIM_MAX];

extern SDL_Surface *screen;

void plusAnim(sprite_t cible)
{
	if (nbAnim < ANIM_MAX)
	{
		sprite_t animation;
		point_reel_t nul;
		nul.rx = 0.;
		nul.ry = 0.;
		SDL_Surface *temp, *animImage;
		temp = SDL_LoadBMP("explosion2.bmp");
		animImage = SDL_DisplayFormat(temp);
		SDL_FreeSurface(temp);
		int colorkey = SDL_MapRGB(screen->format, 255, 0, 255);
		sprite_init(&animation, cible.pr, cible.pi, 0, 0.0, 0.0, nul, nul,
              animImage, 32, colorkey);
		animTab[nbAnim] = animation;
		nbAnim++;
	}
	else
	{
		printf("Nombre d'animation dépassé. Pas de nouvelle animation\n");
	}
	return;
}

void anim_cycle()
{
	if (nbAnim != 0)
	{
		int i;
		for (i = 0; i < nbAnim; i++)
		{
			if ((animTab[i].angle) == 73)
			{
                SDL_FreeSurface(animTab[i].image);
				int j;
				for (j = i; j < nbAnim - 1; j++)
				{
					animTab[j] = animTab[j + 1];
				}
				nbAnim--;
			}
			else
			{
				animTab[i].angle++;
			}
		}
	}
	return;
}

sprite_t plusAnimMort(sprite_t cible)
{
	sprite_t animation;
	point_reel_t nul;
	nul.rx = 0.;
	nul.ry = 0.;
	SDL_Surface *temp, *animImage;
	temp = SDL_LoadBMP("mort_anim.bmp");
	animImage = SDL_DisplayFormat(temp);
	SDL_FreeSurface(temp);
	/* colorkey */
	int colorkey = SDL_MapRGB(screen->format, 255, 0, 255);
	sprite_init(&animation, cible.pr, cible.pi, 0, 0.0, 0.0, nul, nul,
             animImage, 32, colorkey);
	return animation;
}
