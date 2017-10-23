/**********************************************************/
/********* FONCTIONS SUR LES GESTION DU MENU **************/
/**********************************************************/

#include <SDL.h>

#include "globales.h"
#include "menu.h"
#include "event.h"
#include "sprite.h"

extern int nbAsteroid;
extern sprite_t asteroidTab[ASTEROID_MAX];
extern sprite_t shootTab[SHOOT_MAX];
extern int nbShoot;
extern int gameover_menu;
extern int nbAnim;
extern sprite_t animTab[ANIM_MAX];
extern SDL_Surface *screen;
extern int gameover;
extern int press_again;
extern int press_again1;
extern int press_again2;
extern int press_again3;
extern int jeu;

void accueil(int argc, char *argv[])
{
	SDL_WM_SetCaption("Space", "Space");
	SDL_EnableKeyRepeat(10, 100);
	screen = SDL_SetVideoMode(screen_length, screen_height, 0, 0);
	SDL_Surface *temp, *background, *fleche_image;

	temp = SDL_LoadBMP("accueil_bg.bmp");
	background = SDL_DisplayFormat(temp);
	SDL_FreeSurface(temp);

	temp = SDL_LoadBMP("fleche.bmp");
	fleche_image = SDL_DisplayFormat(temp);
	SDL_FreeSurface(temp);

	sprite_t fleche;

	point_reel_t npr;
	npr.rx = 0;
	npr.ry = 0;
	SDL_Rect npi;
	npi.x = 500;
	npi.y = 90;
	point_reel_t speed_fleche;
	speed_fleche.rx = 0;
	speed_fleche.ry = 0;
	point_reel_t accel_fleche;
	accel_fleche.rx = 0;
	accel_fleche.ry = 0;
	int colorkey_fleche = SDL_MapRGB(screen->format, 255, 0, 255);
	sprite_init(&fleche, npr, npi, 0, 0, 0, speed_fleche, accel_fleche, fleche_image, 32, colorkey_fleche);
	SDL_SetColorKey(fleche.image, SDL_SRCCOLORKEY | SDL_RLEACCEL, fleche.colorkey);
	int fenetre = 1;
	int maj = 0;

	char key[SDLK_LAST] = {0};

	while (!gameover_menu)
	{
		update_events(key);
		eventact_menu(key, &fleche, &fenetre, &background);
		if (maj == 0)
		{
			SDL_BlitSurface(background, NULL, screen, NULL);
			{
				SDL_Rect flecheImage;
				flecheImage.y = 0;
				flecheImage.w = fleche.spritetaille;
				flecheImage.h = fleche.spritetaille;
				flecheImage.x = 0;

				SDL_BlitSurface((fleche.image), &flecheImage, screen, &fleche.pi);
			}
		}
		if (fenetre == 3 && maj == 0)
		{
			affiche_score();
			maj = 1;
		}
		if (!(fenetre == 3) && (maj == 1))
		{
			maj = 0;
		}
		SDL_UpdateRect(screen, 0, 0, 0, 0);
	}
	SDL_FreeSurface(background);
}

void fleche_up(sprite_t *sp, int fenetre)
{
	switch (fenetre)
	{
	case 1:
		if ((*sp).pi.y == 90)
		{
			(*sp).pi.y = 550;
			break;
		}
		if ((*sp).pi.y == 200)
		{
			(*sp).pi.y = 90;
			break;
		}
		if ((*sp).pi.y == 300)
		{
			(*sp).pi.y = 200;
			break;
		}
		if ((*sp).pi.y == 420)
		{
			(*sp).pi.y = 300;
			break;
		}
		if ((*sp).pi.y == 550)
		{
			(*sp).pi.y = 420;
			break;
		}
		break;
	default:
		break;
	}
}
void fleche_down(sprite_t *sp, int fenetre)
{
	switch (fenetre)
	{
	case 1:
		if ((*sp).pi.y == 90)
		{
			(*sp).pi.y = 200;
			break;
		}
		if ((*sp).pi.y == 200)
		{
			(*sp).pi.y = 300;
			break;
		}
		if ((*sp).pi.y == 300)
		{
			(*sp).pi.y = 420;
			break;
		}
		if ((*sp).pi.y == 420)
		{
			(*sp).pi.y = 550;
			break;
		}
		if ((*sp).pi.y == 550)
		{
			(*sp).pi.y = 90;
			break;
		}
		break;
	default:
		break;
	}
}
void fleche_action(sprite_t *sp, int *fenetre, SDL_Surface **background)
{
	switch (*fenetre)
	{
	case 1:
		if ((*sp).pi.y == 90)
		{
			gameover_menu = 1;
			break;
		}
		else
		{
			if ((*sp).pi.y == 550)
			{
				gameover_menu = 1;
				gameover = 1;
				jeu = 1;
				break;
			}
			if ((*sp).pi.y == 200)
			{
			    SDL_Surface *temp;
                temp = SDL_LoadBMP("options_bg.bmp");
                (*background) = SDL_DisplayFormat(temp);
                SDL_FreeSurface(temp);
				(*fenetre) = 2;
				(*sp).pi.y = 550;
				break;
			}
			if ((*sp).pi.y == 300)
			{
			    SDL_Surface *temp;
                temp = SDL_LoadBMP("score_bg.bmp");
                (*background) = SDL_DisplayFormat(temp);
                SDL_FreeSurface(temp);
				(*fenetre) = 3;
				trie_score();
				(*sp).pi.y = 550;
				break;
			}
			if ((*sp).pi.y == 420)
			{
                SDL_Surface *temp;
                temp = SDL_LoadBMP("credits_bg.bmp");
                (*background) = SDL_DisplayFormat(temp);
                SDL_FreeSurface(temp);
				(*fenetre) = 4;
				(*sp).pi.y = 550;
				break;
			}
		}
		break;
	case 2:
		if ((*sp).pi.y == 550)
		{
			SDL_Surface *temp;
			temp = SDL_LoadBMP("accueil_bg.bmp");
			(*background) = SDL_DisplayFormat(temp);
			SDL_FreeSurface(temp);
			(*sp).pi.y = 200;
			(*fenetre) = 1;
		}
		break;
	case 3:
		if ((*sp).pi.y == 550)
		{
			SDL_Surface *temp;
			temp = SDL_LoadBMP("accueil_bg.bmp");
			(*background) = SDL_DisplayFormat(temp);
			SDL_FreeSurface(temp);
			(*sp).pi.y = 300;
			(*fenetre) = 1;
		}
		break;
	case 4:
		if ((*sp).pi.y == 550)
		{
			SDL_Surface *temp;
			temp = SDL_LoadBMP("accueil_bg.bmp");
			(*background) = SDL_DisplayFormat(temp);
			SDL_FreeSurface(temp);
			(*sp).pi.y = 420;
			(*fenetre) = 1;
		}
		break;
	default:
		break;
	}
}

void eventact_menu(char *key, sprite_t *sp, int *fenetre, SDL_Surface **background)
{
	SDLKey tabkey[5] = {SDLK_z, SDLK_q, SDLK_s, SDLK_d, SDLK_SPACE};

	if (key[tabkey[0]])
	{ // Z
		switch (*fenetre)
		{
		case 1:
			if (press_again1)
			{
				fleche_up(sp, *fenetre);
				press_again1 = 0;
			}
			break;
		case 2:
			fleche_up(sp, *fenetre);
			break;
		default:
			break;
		}
	}
	else
	{
		press_again1 = 1;
	}

	if (key[tabkey[1]])
	{ // Q
	}
	if (key[tabkey[2]])
	{ // S
		switch (*fenetre)
		{
		case 1:
			if (press_again2)
			{
				fleche_down(sp, *fenetre);
				press_again2 = 0;
			}
			break;
		case 2:
			fleche_down(sp, *fenetre);
			break;
		default:
			break;
		}
	}
	else
	{
		press_again2 = 1;
	}

	if (key[tabkey[3]])
	{ // D
	}
	if (key[tabkey[4]])
	{ // SPACE
		if (press_again3)
		{
			fleche_action(sp, fenetre, background);
			press_again3 = 0;
		}
	}
	else
	{
		press_again3 = 1;
	}
}

void trie_score_aux(int *tab, int n)
{
	int i, min, j, tmp;
	for (i = 0; i < n; i++)
	{
		min = i;
		for (j = i + 1; j < n; j++)
		{
			if (tab[j] > tab[min])
			{
				min = j;
			}
		}
		if (min != i)
		{
			tmp = tab[i];
			tab[i] = tab[min];
			tab[min] = tmp;
		}
	}
}

void trie_score()
{
	FILE *fichier = fopen("scores.txt", "r+");
	int i, tmp;
	int nb_ligne = 0;
	while (fscanf(fichier, "%d", &tmp) != EOF)
	{
		nb_ligne++;
	}
	int score[nb_ligne];
	rewind(fichier);
	i = 0;
	while (fscanf(fichier, "%d", &score[i++]) != EOF)
		;
	fclose(fichier);
	fichier = fopen("scores.txt", "w");
	trie_score_aux(score, nb_ligne);
	for (i = 0; i < nb_ligne && i < 10; i++)
	{
		fprintf(fichier, "%d\n", score[i]);
	}
	fclose(fichier);
}
void affiche_nombre(int x, int y, int nb)
{
	SDL_Surface *temp, *nombre;
	temp = SDL_LoadBMP("nombre_v2.bmp");
	nombre = SDL_DisplayFormat(temp);
	SDL_FreeSurface(temp);

	int colorkey_nb = SDL_MapRGB(screen->format, 255, 0, 255);
	SDL_SetColorKey(nombre, SDL_SRCCOLORKEY | SDL_RLEACCEL, colorkey_nb);

	SDL_Rect pi;
	pi.x = x;
	pi.y = y;
	int nb_chifre = 0;
	int nb_aux = nb;
	while (nb_aux > 9)
	{
		nb_aux = nb_aux / 10;
		nb_chifre++;
	}

	int nb_split[nb_chifre];
	for (int i = 0; i <= nb_chifre; i++)
	{
		nb_split[i] = nb % 10;
		nb = nb / 10;
	}
	SDL_Rect nombreImage;
	for (int i = 0; i <= nb_chifre; i++)
	{
		nombreImage.x = nb_split[nb_chifre - i] * 32;
		nombreImage.y = 0;
		nombreImage.w = 32;
		nombreImage.h = 34;
		SDL_BlitSurface(nombre, &nombreImage, screen, &pi);
		pi.x += 32;
	}
}
void affiche_score()
{
	FILE *fichier = fopen("scores.txt", "r");
	int nb_ligne = 0;
	int i, tmp;
	while (fscanf(fichier, "%d", &tmp) != EOF)
	{
		nb_ligne++;
	}
	int score[nb_ligne];
	rewind(fichier);
	while (fscanf(fichier, "%d", &score[i++]) != EOF)
		;
	for (i = 0; i < nb_ligne; i++)
	{
		affiche_nombre((screen_length / 2) - 64, i * (screen_height / (nb_ligne + 5)), score[i]);
	}
	fclose(fichier);
}
