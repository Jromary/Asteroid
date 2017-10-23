#include <SDL.h>
#include <time.h>
#include <string.h>
#include <unistd.h>

#include "globales.h"
#include "collider.h"
#include "menu.c"
#include "asteroid.h"
#include "sprite.h"
#include "shoot.h"
#include "anim.h"
#include "event.h"
#include "bonus.h"

extern int nbAsteroid;
extern sprite_t asteroidTab[ASTEROID_MAX];
extern sprite_t shootTab[SHOOT_MAX];

extern int nbShoot;

extern int gameover;
extern int gameover_menu;

extern int nbAnim;
extern sprite_t animTab[ANIM_MAX];

extern SDL_Surface *screen;

extern int jeu;

extern pickable_t pickableTab[PICKABLE_MAX];
extern int nbBonus;
extern int invincible;
extern int tripletir;
extern int confuse;

extern int score;

extern int nb_vie;

int main(int argc, char *argv[])
{
	/* Initialisation du module time pour les tirages aléatoires.
        et de SDL */
	SDL_Init(SDL_INIT_VIDEO);
	srand(time(NULL));

	/* Boucle de programme */
	while (!jeu)
	{
		/* Initialisation des composants de SDL */
		SDL_WM_SetCaption("Space_Shooter", "Space_Shooter");
		SDL_EnableKeyRepeat(10, 100);
		screen = SDL_SetVideoMode(screen_length, screen_height, 0, 0);

		/* Declaration et chargement des images vaisseau, fond */
		SDL_Surface *ship, *temp, *background, *vie_image;
		temp = SDL_LoadBMP("vaisseau_v2.bmp");
		ship = SDL_DisplayFormat(temp);
		SDL_FreeSurface(temp);
		temp = SDL_LoadBMP("acc.bmp");
		background = SDL_DisplayFormat(temp);
		SDL_FreeSurface(temp);

		/* Initialisation du vaisseau */

		sprite_t vaisseau;

		point_reel_t npr;
		npr.rx = screen_length / 2;
		npr.ry = screen_height / 2;
		SDL_Rect npi;
		npi.x = screen_length / 2;
		npi.y = screen_height / 2;
		point_reel_t speed_vaisseau;
		speed_vaisseau.rx = 0;
		speed_vaisseau.ry = 0;
		point_reel_t accel_vaisseau;
		accel_vaisseau.rx = 0.001;
		accel_vaisseau.ry = 0.001;
		int colorkey_vaisseau = SDL_MapRGB(screen->format, 255, 0, 255);
		sprite_init(&vaisseau, npr, npi, ship_angle, 0.0, ship_rotation,
               speed_vaisseau, accel_vaisseau, ship, ship_size, colorkey_vaisseau);
		SDL_SetColorKey(vaisseau.image, SDL_SRCCOLORKEY | SDL_RLEACCEL, vaisseau.colorkey);

        /* Declaration des variables necessaire au jeu */
		int i, j;
		int pu = 0;
		int pv = 0;

		int anim_gameover_encours = 0;
		sprite_t anim_mort;
		float delai_anim_mort = 0.;

		int delai_asteroid = time(0);
		int comp_delai_asteroid;

		int delai_invincible;
		int comp_delai_invincible;
		int delai_tripletir;
		int comp_delai_tripletir;
		int delai_confuse;
		int comp_delai_confuse;

		tripletir = 0;
		invincible = 0;
		confuse = 0;

		/* Initialisation du compteur de vie */
		temp = SDL_LoadBMP("1up_sprite.bmp");
		vie_image = SDL_DisplayFormat(temp);
		SDL_FreeSurface(temp);
		SDL_Rect image_vie;
		image_vie.x = 0;
		image_vie.y = 0;
		image_vie.h = 32;
		image_vie.w = 32;
		SDL_Rect viepi;
		viepi.x = 0;
		viepi.y = 0;

		accueil(argc, argv);
		gameover_menu = 0;

		/* Declaration du tableau de touches */
		char key[SDLK_LAST] = {0};

		/* Boucle de jeu */
		while (!gameover)
		{
			if (!anim_gameover_encours)
			{
				update_events(key);
				if (!confuse)
				{
					eventact(key, &vaisseau);
				}
				else
				{
					eventact_malus(key, &vaisseau);
				}
			}

			// Déplacement, mise à jour et blit et différents objets

			// Background
			SDL_BlitSurface(background, NULL, screen, NULL);
			SDL_SetColorKey(vie_image, SDL_SRCCOLORKEY | SDL_RLEACCEL, colorkey_vaisseau);
			SDL_BlitSurface(vie_image, &image_vie, screen, &viepi);

			// Ajout ou non d'asteroid
			comp_delai_asteroid = time(0);
			if (comp_delai_asteroid - delai_asteroid >= DELAI_APPARITION_ASTEROID)
			{
				plusAsteroid();
				delai_asteroid = time(0);
			}

			// Arret ou non des bonus

			if (invincible)
			{
				comp_delai_invincible = time(0);
				if (comp_delai_invincible - delai_invincible >= DELAI_INVINCIBILITE)
				{
					invincible = 0;
					temp = SDL_LoadBMP("vaisseau_v2.bmp");
					ship = SDL_DisplayFormat(temp);
					SDL_FreeSurface(temp);
					vaisseau.image = ship;
					SDL_SetColorKey(vaisseau.image, SDL_SRCCOLORKEY | SDL_RLEACCEL, vaisseau.colorkey);
				}
			}

			if (tripletir)
			{
				comp_delai_tripletir = time(0);
				if (comp_delai_tripletir - delai_tripletir >= DELAI_TRIPLETIR)
				{
					tripletir = 0;
				}
			}

			if (confuse)
			{
				comp_delai_confuse = time(0);
				if (comp_delai_confuse - delai_confuse >= DELAI_CONFUSE)
				{
					confuse = 0;
				}
			}

            /* Blit */

			// Bonus
			for (i = 0; i < nbBonus; i++)
			{
				animBonus(i);
				SDL_SetColorKey(pickableTab[i].sprite.image, SDL_SRCCOLORKEY | SDL_RLEACCEL, pickableTab[i].sprite.colorkey);

				SDL_Rect pickableImage;
				pickableImage.x = pickableTab[i].sprite.angle * pickableTab[i].sprite.spritetaille;
				pickableImage.y = 0;
				pickableImage.w = pickableTab[i].sprite.spritetaille;
				pickableImage.h = pickableTab[i].sprite.spritetaille;
				SDL_BlitSurface((pickableTab[i].sprite.image), &pickableImage, screen, &pickableTab[i].sprite.pi);
			}

			// Asteroids
			for (i = 0; i < nbAsteroid; i++)
			{
				//colorkeyAsteroid = SDL_MapRGB(screen->format, 0, 255, 255);
				SDL_SetColorKey(asteroidTab[i].image, SDL_SRCCOLORKEY | SDL_RLEACCEL, asteroidTab[i].colorkey);
				asteroid_turn_right(&asteroidTab[i]);
				asteroid_move(&asteroidTab[i]);

				exitscreen(&asteroidTab[i]);

				SDL_Rect asteroidImage;
				asteroidImage.x = asteroidTab[i].angle * asteroidTab[i].spritetaille;
				asteroidImage.y = 0;
				asteroidImage.w = asteroidTab[i].spritetaille;
				asteroidImage.h = asteroidTab[i].spritetaille;
				SDL_BlitSurface((asteroidTab[i].image), &asteroidImage, screen, &asteroidTab[i].pi);
			}

			// Shoots
			for (i = 0; i < nbShoot; i++)
			{
				//colorkeyShoot = SDL_MapRGB(screen->format, 255, 125, 0);
				SDL_SetColorKey(shootTab[i].image, SDL_SRCCOLORKEY | SDL_RLEACCEL, shootTab[i].colorkey);

				sprite_move(&shootTab[i]);
				exitscreen(&shootTab[i]);

				SDL_Rect shootImage;

				shootImage.x = 0;
				shootImage.y = 0;
				shootImage.w = shootTab[i].spritetaille;
				shootImage.h = shootTab[i].spritetaille;
				SDL_BlitSurface((shootTab[i].image), &shootImage, screen, &shootTab[i].pi);
			}
			exit_shoot();

            // Blit et tests classique
			if (!anim_gameover_encours)
			{
				sprite_move(&vaisseau);
				exitscreen(&vaisseau);
				{
					SDL_Rect shipImage;
					shipImage.y = 0;
					shipImage.w = ship_size;
					shipImage.h = ship_size;
					shipImage.x = ship_size * vaisseau.angle;
					SDL_BlitSurface((vaisseau.image), &shipImage, screen, &vaisseau.pi);
				}

				/* Collisions */
				for (i = 0; i < nbBonus; i++)
				{
				    // Vaisseau - bonus
					if (collide_test(&vaisseau, &pickableTab[i].sprite, screen->format, &pu, &pv))
					{
						if (pickableTab[i].type == 0)
						{
							invincible = 1;
							delai_invincible = time(0);
							temp = SDL_LoadBMP("vaisseau_v2_invincible.bmp");
							ship = SDL_DisplayFormat(temp);
							SDL_FreeSurface(temp);
							vaisseau.image = ship;
							SDL_SetColorKey(vaisseau.image, SDL_SRCCOLORKEY | SDL_RLEACCEL, vaisseau.colorkey);
							moinsBonus(i);
						}
						else if (pickableTab[i].type == 1)
						{
							tripletir = 1;
							moinsBonus(i);
							delai_tripletir = time(0);
						}
						else if (pickableTab[i].type == 2)
						{
							confuse = 1;
							delai_confuse = time(0);
							moinsBonus(i);
						}
						else if (pickableTab[i].type == 3)
						{
							nb_vie++;
							moinsBonus(i);
						}
					}
				}
				if (!invincible)
				{
					for (i = 0; i < nbAsteroid; i++)
					{
					    // Vaisseau - Asteroids
						if (collide_test(&vaisseau, &asteroidTab[i], screen->format, &pu, &pv))
						{
							nb_vie--;
							tripletir = 0;
							confuse = 0;
							if (nb_vie < 1)
							{
								anim_gameover_encours = 1;
								anim_mort = plusAnimMort(vaisseau);
							}
							else
							{
								vaisseau.pi.x = screen_length / 2;
								vaisseau.pi.x = screen_height / 2;
								vaisseau.pr.rx = screen_length / 2;
								vaisseau.pr.ry = screen_height / 2;
								{
									invincible = 1;
									delai_invincible = time(0);
									temp = SDL_LoadBMP("vaisseau_v2_invincible.bmp");
									ship = SDL_DisplayFormat(temp);
									SDL_FreeSurface(temp);
									vaisseau.image = ship;
									SDL_SetColorKey(vaisseau.image, SDL_SRCCOLORKEY | SDL_RLEACCEL, vaisseau.colorkey);
								}
							}
						}
					}
				}

				for (i = 0; i < nbShoot; i++)
				{
					for (j = 0; j < nbAsteroid; j++)
					{
                        // Tir - Asteroids
						if (collide_test(&shootTab[i], &asteroidTab[j], screen->format, &pu, &pv))
						{
							score += 1024 / asteroidTab[j].spritetaille;
							int rd = rand() % 20;
							if (rd == 0)
							{
								plusBonus(asteroidTab[j].pi.x, asteroidTab[j].pi.y);
							}
							plusAnim(shootTab[i]);
							moinsShoot(i);
                            SDL_UpdateRect(screen, 0, 0, 0, 0);

							splitAsteroid(j);
							break;
						}
					}
				}
			}
			else // Blit special si le vaisseau meurt
			{
				SDL_SetColorKey(anim_mort.image, SDL_SRCCOLORKEY | SDL_RLEACCEL, anim_mort.colorkey);

				SDL_Rect anim_mort_image;

				anim_mort_image.x = anim_mort.angle * anim_mort.spritetaille;
				anim_mort_image.y = 0;
				anim_mort_image.w = 32;
				anim_mort_image.h = 32;
				SDL_BlitSurface((anim_mort.image), &anim_mort_image, screen, &anim_mort.pi);
				delai_anim_mort += 0.03;
				if (delai_anim_mort >= 1.)
				{
					anim_mort.angle += 1;
					delai_anim_mort = 0.;
				}
                // Animation de mort terminee, fin du jeu
				if (anim_mort.angle >= 8)
				{
					gameover = 1;
				}
			}

			// Animations des explosions
			anim_cycle();
			for (i = 0; i < nbAnim; i++)
			{
				SDL_SetColorKey(animTab[i].image, SDL_SRCCOLORKEY | SDL_RLEACCEL, animTab[i].colorkey);

				SDL_Rect animImage;

				animImage.x = animTab[i].angle * animTab[i].spritetaille;
				animImage.y = 0;
				animImage.w = animTab[i].spritetaille;
				animImage.h = animTab[i].spritetaille;
				SDL_BlitSurface((animTab[i].image), &animImage, screen, &animTab[i].pi);
			}
			affiche_nombre(32, 0, nb_vie);
			affiche_nombre(screen_length - 150, screen_height - 34, score);

			SDL_UpdateRect(screen, 0, 0, 0, 0);
		}

		// Enregistrement des scores
		if (score != 0)
		{
			FILE *fichier = fopen("scores.txt", "a");
			if (fichier != NULL)
			{
				fprintf(fichier, "%d\n", score);
			}
			fclose(fichier);

			printf("Score : %d\n", score);
		}

		// Nettoyage des éléments
		gameover = 0;
		nb_vie = 3;
		while (nbAsteroid > 0)
		{
			moinsAsteroid(0);
		}
		while (nbShoot > 0)
		{
			moinsShoot(0);
		}
		while (nbAnim > 0)
		{
			anim_cycle();
		}

		while (nbBonus > 0)
		{
			moinsBonus(0);
		}

		//Ecran de gameover
		if (!jeu)
		{
			temp = SDL_LoadBMP("gameover_bg.bmp");
			background = SDL_DisplayFormat(temp);
			SDL_FreeSurface(temp);
			SDL_BlitSurface(background, NULL, screen, NULL);
			affiche_nombre(screen_length / 2 - 32, screen_height / 2 + 64, score);
			SDL_UpdateRect(screen, 0, 0, 0, 0);
			sleep(2);
			SDL_FreeSurface(background);
			SDL_FreeSurface(vaisseau.image);
			score = 0;
		}
	}

	//Fermeture de SDL
	SDL_Quit();
	return 0;
}
