/**********************************************************/
/********* FONCTIONS SUR LES GESTION D'EVENEMENT **********/
/**********************************************************/

#include "sprite.h"
#include "globales.h"
#include "asteroid.h"
#include "shoot.h"
#include "bonus.h"
#include "event.h"

extern int gameover;
extern int gameover_menu;
extern int press_again;
extern int press_again1;
extern int press_again2;
extern int press_again3;
extern float ship_ang;
extern int jeu;

extern int nbAsteroid;

extern int tripletir;

void eventact(char *key, sprite_t *sp)
{
	SDLKey tabkey[NBPLAYERS][10] = {{SDLK_z, SDLK_q, SDLK_s, SDLK_d, SDLK_SPACE,
									 SDLK_p, SDLK_o, SDLK_m}};

	int k = -1;
	if (!(nbAsteroid == 0))
	{
		k = rand() % nbAsteroid;
	}

	int i;
	for (i = 0; i < NBPLAYERS; i++)
	{
		if (key[tabkey[i][0]])
		{ // Z
			sprite_boost(sp, COEFF_BOOST);
		}
		if (key[tabkey[i][1]])
		{ // Q
			sprite_turn_left(sp);
		}
		if (key[tabkey[i][2]])
		{ // S
			sprite_boost(sp, -COEFF_BOOST);
		}
		if (key[tabkey[i][3]])
		{ // D
			sprite_turn_right(sp);
		}
		if (key[tabkey[i][4]])
		{ // SPACE
			if (press_again)
			{
				if (!tripletir)
				{
					sprite_shoot(sp);
				}
				else
				{
					sprite_multi_shoot(sp);
				}
				press_again = 0;
			}
		}
		else
		{
			press_again = 1;
		}
		if (key[tabkey[i][5]])
		{ // p
			plusAsteroid();
		}
		if (key[tabkey[i][6]])
		{ // o
			if (k != -1)
			{
				splitAsteroid(k);
			}
		}
		if (key[tabkey[i][7]])
		{ // m
			plusBonus(rand() % screen_length, rand() % screen_height);
		}
	}
}

void update_events(char *keys)
{
	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
		case SDL_QUIT:
			gameover = 1;
			gameover_menu = 1;
			jeu = 1;
			break;
		case SDL_KEYUP:
			keys[event.key.keysym.sym] = 0;
			break;
		case SDL_KEYDOWN:
			switch (event.key.keysym.sym)
			{
			case SDLK_ESCAPE:
				gameover = 1;
				gameover_menu = 1;
				jeu = 1;
				break;
			default:
				break;
			}
			keys[event.key.keysym.sym] = 1;
			break;
		default:
			break;
		}
	}
}

void eventact_malus(char *key, sprite_t *sp)
{
	SDLKey tabkey[NBPLAYERS][10] = {{SDLK_z, SDLK_q, SDLK_s, SDLK_d, SDLK_SPACE,
									 SDLK_p, SDLK_o, SDLK_m}};

	int k = -1;
	if (!(nbAsteroid == 0))
	{
		k = rand() % nbAsteroid;
	}

	int i;
	for (i = 0; i < NBPLAYERS; i++)
	{
		if (key[tabkey[i][0]])
		{ // Z
			sprite_turn_right(sp);
		}
		if (key[tabkey[i][1]])
		{ // Q
			sprite_boost(sp, COEFF_BOOST);
		}
		if (key[tabkey[i][2]])
		{ // S
			sprite_turn_left(sp);
		}
		if (key[tabkey[i][3]])
		{ // D
			sprite_boost(sp, -COEFF_BOOST);
		}
		if (key[tabkey[i][4]])
		{ // SPACE
			if (press_again)
			{
				if (!tripletir)
				{
					sprite_shoot(sp);
				}
				else
				{
					sprite_multi_shoot(sp);
				}
				press_again = 0;
			}
		}
		else
		{
			press_again = 1;
		}
		if (key[tabkey[i][5]])
		{ // p
			plusAsteroid();
		}
		if (key[tabkey[i][6]])
		{ // o
			if (k != -1)
			{
				splitAsteroid(k);
			}
		}
		if (key[tabkey[i][7]])
		{ // m
			plusBonus(rand() % screen_length, rand() % screen_height);
		}
	}
}
