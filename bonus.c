/***********************************************/
/********* FONCTIONS SUR LES BONUS *************/
/***********************************************/

#include <SDL.h>
#include "bonus.h"
#include "globales.h"
#include "sprite.h"

extern pickable_t pickableTab[PICKABLE_MAX];
extern int nbBonus;
extern SDL_Surface *screen;

void pickable_init(pickable_t *pick, int type, int x, int y)
{
	SDL_Surface *temp, *bonusImage;
	(*pick).type = type;
	switch (type)
	{
	case 0: // Invincibilité
		temp = SDL_LoadBMP("bonus1_sprite.bmp");
		bonusImage = SDL_DisplayFormat(temp);
		SDL_FreeSurface(temp);
		break;
	case 1: // Triple tir
		temp = SDL_LoadBMP("bonus2_sprite.bmp");
		bonusImage = SDL_DisplayFormat(temp);
		SDL_FreeSurface(temp);
		break;
	case 2: // Confusion
		temp = SDL_LoadBMP("malus1_sprite.bmp");
		bonusImage = SDL_DisplayFormat(temp);
		SDL_FreeSurface(temp);
		break;
	case 3: // Vie
		temp = SDL_LoadBMP("1up_sprite.bmp");
		bonusImage = SDL_DisplayFormat(temp);
		SDL_FreeSurface(temp);
		break;
	}
	SDL_Rect position;
	if (x > screen_length - 32)
	{
		x = screen_length - 32;
	}
	if (y > screen_height - 32)
	{
		y = screen_height - 32;
	}
	position.x = x;
	position.y = y;

	int colorkeyBonus = SDL_MapRGB(screen->format, 255, 0, 255);

	point_reel_t pr;
	pr.rx = position.x;
	pr.ry = position.y;

	point_reel_t prNul;
	prNul.rx = 0.;
	prNul.ry = 0.;

	sprite_init(&((*pick).sprite), pr, position, 1, 1., 0., prNul, prNul, bonusImage,
				32, colorkeyBonus);
}

void plusBonus(int x, int y)
{
	if (nbBonus < PICKABLE_MAX)
	{
		pickable_t new;
		int type = rand() % 4;
		pickable_init(&new, type, x, y);
		pickableTab[nbBonus] = new;
		nbBonus++;
	}
	else
	{
		printf("Nombre de bonus dépassé. Pas de nouveau bonus\n");
	}
	return;
}
void moinsBonus(int id_cible)
{
	if (nbBonus == 0)
	{
		printf("ERREUR : appel de moinsBonus alors que le tableau est vide\n");
		printf("Aucun bonus a supprimer.\n");
	}
	else
	{
		SDL_FreeSurface(pickableTab[id_cible].sprite.image);
		int i;
		for (i = id_cible; i < nbBonus - 1; i++)
		{
			pickableTab[i] = pickableTab[i + 1];
		}
		nbBonus--;
	}
	return;
}
void animBonus(int id_cible)
{
	if ((pickableTab[id_cible].sprite.anglereel + 0.02) < 6)
	{
		pickableTab[id_cible].sprite.anglereel += 0.02;
	}
	else
	{
		pickableTab[id_cible].sprite.anglereel = 1;
	}
	pickableTab[id_cible].sprite.angle = (int)pickableTab[id_cible].sprite.anglereel;
}
