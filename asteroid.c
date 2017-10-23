/***********************************************/
/********* FONCTIONS SUR LES ASTEROIDS *********/
/***********************************************/
#include <SDL.h>
#include <stdio.h>
#include "asteroid.h"
#include "globales.h"
#include "sprite.h"

extern int nbAsteroid;
extern sprite_t asteroidTab[ASTEROID_MAX];

extern SDL_Surface *screen;

void asteroid_init(sprite_t *sp)
{
	SDL_Surface *temp, *asteroidImage;
	int taille;
	int testTaille = (rand() % 3) + 1;
	switch (testTaille)
	{
	case 1:
		taille = size_asteroid_min;
		temp = SDL_LoadBMP("asteroid_16x16.bmp");
		asteroidImage = SDL_DisplayFormat(temp);
		SDL_FreeSurface(temp);
		break;
	case 2:
		taille = size_asteroid_med;
		temp = SDL_LoadBMP("asteroid_32x32.bmp");
		asteroidImage = SDL_DisplayFormat(temp);
		SDL_FreeSurface(temp);
		break;
	case 3:
		taille = size_asteroid_max;
		temp = SDL_LoadBMP("asteroid_64x64.bmp");
		asteroidImage = SDL_DisplayFormat(temp);
		SDL_FreeSurface(temp);
		break;
	default:
		break;
	}

	/* Position sur les bords de l'ecran */
	int testPosition = rand() % 4;
	SDL_Rect position;
	switch (testPosition)
	{

	case 0:
		position.x = 0;
		position.y = rand() % screen_height;
		break;
	case 1:
	    /* On evite que le sprite déborde de l'ecran au spawn */
		position.x = rand() % (screen_length - (taille));
		position.y = 0;
		break;
	case 2:
		position.x = screen_length - (taille);
		position.y = rand() % (screen_height - (taille));
		break;
	case 3:
		position.x = rand() % (screen_length - (taille));
		position.y = screen_height - (taille);
		break;
	default:
		position.x = 0;
		position.y = 0;
		break;
	}
	point_reel_t npr;
	npr.rx = position.x;
	npr.ry = position.y;

	int testDirection = rand() % 4;
	point_reel_t direction;
	switch (testDirection)
	{
	/* */
	case 0:
		direction.rx = 1.;
		direction.ry = 1.;
		break;
	case 1:
		direction.rx = -1.;
		direction.ry = 1.;
		break;
	case 2:
		direction.rx = -1.;
		direction.ry = -1.;
		break;
	case 3:
		direction.rx = 1.;
		direction.ry = -1.;
		break;
	default:
		direction.rx = 1.;
		direction.ry = 1.;
		break;
	}
	point_reel_t asteroidAccel;
	asteroidAccel.rx = direction.rx * (rand() % 100) / ASTEROID_SPEED;
	asteroidAccel.ry = direction.ry * (rand() % 100) / ASTEROID_SPEED;
	point_reel_t asteroidSpeed;
	asteroidSpeed.rx = 0;
	asteroidSpeed.ry = 0;

	int colorkey = SDL_MapRGB(screen->format, 0, 255, 255);

	int angle = rand() % 36;
	float angfloat = (float)angle;
	sprite_init(sp, npr, position, angle, angfloat, asteroid_rota,
             asteroidSpeed, asteroidAccel, asteroidImage, taille, colorkey);
	return;
}

void plusAsteroid()
{
	if (nbAsteroid < ASTEROID_MAX)
	{
		sprite_t new;
		asteroid_init(&new);
		asteroidTab[nbAsteroid] = new;
		nbAsteroid++;
	}
	else
	{

		printf("Nombre d'asteroides dépassé. Pas de nouveau asteroid\n");
	}
	return;
}

void moinsAsteroid(int id_cible)
{
	if (nbAsteroid == 0)
	{
		printf("ERREUR : appel de moinsAsteroid alors que le tableau est vide\n");
		printf("Aucun asteroide a supprimer.\n");
	}
	else
	{
		SDL_FreeSurface(asteroidTab[id_cible].image);
		int i;
		for (i = id_cible; i < nbAsteroid - 1; i++)
		{
			asteroidTab[i] = asteroidTab[i + 1];
		}
		nbAsteroid--;
	}
	return;
}

void asteroid_move(sprite_t *sp)
{
	(*sp).speed.rx += (*sp).accel.rx;
	(*sp).speed.ry += (*sp).accel.ry;
	(*sp).accel.rx = 0;
	(*sp).accel.ry = 0;
	(*sp).pr.rx += (*sp).speed.rx;
	(*sp).pr.ry += (*sp).speed.ry;
	(*sp).pi.x = (int)(*sp).pr.rx;
	(*sp).pi.y = (int)(*sp).pr.ry;
}

void affichagePositionAsteroid()
{
	printf(">>>>>>>>>>>>>>>>>>>>>> NB ASTEROID : %d\n", nbAsteroid);
	int l;
	for (l = 0; l < nbAsteroid; l++)
	{
		printf("position ast [%d] = %d - %d\n", l, asteroidTab[l].pi.x, asteroidTab[l].pi.y);
		printf("speed ast [%d] = %f - %f\n", l, asteroidTab[l].speed.rx, asteroidTab[l].speed.ry);
		printf("accel ast [%d] = %f - %f\n", l, asteroidTab[l].accel.rx, asteroidTab[l].accel.ry);
		printf("taille ast [%d] = %d\n", l, asteroidTab[l].spritetaille);
		printf("angle ast [%d] = %d\n", l, asteroidTab[l].angle);
	}
	printf(">>>>>>>>>>>>>>>>>>>>>>\n\n");
}

void fragmentationAsteroid(int id_cible)
{
	int nbFragment = (rand() % 3) + 1;
	if (nbAsteroid + nbFragment < ASTEROID_MAX)
	{
		int j;
		for (j = 0; j < nbFragment; j++)
		{
			sprite_t new;
			SDL_Surface *temp, *asteroidImage;
			switch (asteroidTab[id_cible].spritetaille)
			{
			case 32:
				temp = SDL_LoadBMP("asteroid_16x16.bmp");
				asteroidImage = SDL_DisplayFormat(temp);
				SDL_FreeSurface(temp);
				break;
			case 64:
				temp = SDL_LoadBMP("asteroid_32x32.bmp");
				asteroidImage = SDL_DisplayFormat(temp);
				SDL_FreeSurface(temp);
				break;
			default:
				printf("Erreur fragmentation \n");
				break;
			}

			int testDirection = rand() % 4;
			point_reel_t direction;
			switch (testDirection)
			{
			case 0:
				direction.rx = 1.;
				direction.ry = 1.;
				break;
			case 1:
				direction.rx = -1.;
				direction.ry = 1.;
				break;
			case 2:
				direction.rx = -1.;
				direction.ry = -1.;
				break;
			case 3:
				direction.rx = 1.;
				direction.ry = -1.;
				break;
			default:
				direction.rx = 1.;
				direction.ry = 1.;
				break;
			}
			/* Acceleration et vitesse */
			point_reel_t asteroidAccel;

			asteroidAccel.rx = direction.rx * (rand() % 100) / 1000.;
			asteroidAccel.ry = direction.ry * (rand() % 100) / 1000.;
			point_reel_t asteroidSpeed;
			asteroidSpeed.rx = 0;
			asteroidSpeed.ry = 0;

			/* colorkey */
			int colorkey = SDL_MapRGB(screen->format, 0, 255, 255);
			int angle = rand() % 36;
			float angreel = (float)angle;

			sprite_init(&new, asteroidTab[id_cible].pr, asteroidTab[id_cible].pi,
               angle, angreel, asteroid_rota, asteroidSpeed, asteroidAccel, asteroidImage,
               asteroidTab[id_cible].spritetaille / 2, colorkey);
			asteroidTab[nbAsteroid] = new;
			nbAsteroid++;
		}
	}
	else
	{
		printf("Nombre d'asteroides dépassé, pas de nouveaux fragments.\n");
	}
	return;
}

void splitAsteroid(int k)
{
	if (asteroidTab[k].spritetaille > 16)
	{
		fragmentationAsteroid(k);
		moinsAsteroid(k);
	}
	else
	{
		moinsAsteroid(k);
	}
}
