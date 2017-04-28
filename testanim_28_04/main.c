#include <SDL.h>
#include <SDL/SDL_mixer.h>
#include <time.h>

#include "functions.h"
#include "variables.h"



/* comande de compilation:
gcc -Wall -g -std=c99 Main.c `sdl-config --cflags --libs` -lm -o Main
*/

extern int nbAsteroid;
extern Sprite asteroidTab[ASTEROID_MAX];
extern Sprite shootTab[SHOOT_MAX];
extern int nbShoot;
extern int gameover;
extern int nbAnim;
extern Sprite animTab[ANIM_MAX];

int main(int argc, char* argv[]){
	// Initialisation du module time pour les tirages aléatoires.
	srand(time(NULL));

	// Initialisation de SDL
	SDL_Init(SDL_INIT_VIDEO);
	SDL_WM_SetCaption("Space","Space");
	SDL_EnableKeyRepeat(10, 10);
	SDL_Surface* screen = SDL_SetVideoMode(screen_length, screen_height, 0, 0);
	// Initialisation du module audio
	if(Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 1024) == -1) //Initialisation de l'API Mixer
	{
	   printf("%s", Mix_GetError());
	}

	// Declaration et chargement des images vaisseau, fond
	SDL_Surface *ship, *temp, *background;
	int colorkey, colorkeyShoot, colorkeyAsteroid, colorkeyAnim;
	temp = SDL_LoadBMP("greenship-v1.bmp");
	ship = SDL_DisplayFormat(temp);
	SDL_FreeSurface(temp);
	colorkey = SDL_MapRGB(screen->format, 255, 0, 255);
	SDL_SetColorKey(ship, SDL_SRCCOLORKEY | SDL_RLEACCEL, colorkey);
	temp = SDL_LoadBMP("backgroundlvl1.bmp");
	background = SDL_DisplayFormat(temp);
	SDL_FreeSurface(temp);

	// Bruitages
	Mix_AllocateChannels(32); //Allouer 32 canaux
	Mix_Chunk *bruitTir;//Créer un pointeur pour stocker un .WAV
	bruitTir = Mix_LoadWAV("laser.wav"); //Charger un wav dans un pointeur
	Mix_VolumeChunk(bruitTir, MIX_MAX_VOLUME); //Mettre un volume pour ce wav
	// Musique de fond
	Mix_Music *musique;
	musique = Mix_LoadMUS("asteroid.mp3");
	Mix_PlayMusic(musique, -1); //Jouer infiniment la musique

	// Déclaration et initialisation de la struct Sprite vaisseau

	Sprite vaisseau;

	Pointreel npr;
	npr.rx=100;
	npr.ry=100;
	SDL_Rect npi;
	npi.x=100;
	npi.y=100;
	Pointreel speed_vaisseau;
	speed_vaisseau.rx = 0;
	speed_vaisseau.ry = 0;
	Pointreel accel_vaisseau;
	accel_vaisseau.rx = 0.001;
	accel_vaisseau.ry = 0.001;

	sprite_init(&vaisseau,npr,npi,ship_angle, speed_vaisseau, accel_vaisseau, *ship, ship_size);
	

	/* DEBUG */
	int i;
	plusAsteroid();
	/************/

	while(!gameover)
	{

		SDL_Event event;

		if (SDL_PollEvent(&event)){
			eventact(event,&vaisseau,bruitTir);
		}

		/*** Déplacement, mise à jour et blit et différents objets ***/

		// Background
		SDL_BlitSurface(background,NULL,screen,NULL);

		// Asteroids
		for(i=0; i < nbAsteroid; i++)
		{
			colorkeyAsteroid = SDL_MapRGB(screen->format, 0, 255, 255);
			SDL_SetColorKey(&asteroidTab[i].image, SDL_SRCCOLORKEY | SDL_RLEACCEL, colorkeyAsteroid);

			asteroid_move(&asteroidTab[i]);

			exitscreen(&asteroidTab[i]);

			SDL_Rect asteroidImage;
			asteroidImage.x=asteroidTab[i].angle * asteroidTab[i].spritetaille;
			asteroidImage.y=0;
			asteroidImage.w = asteroidTab[i].spritetaille;
			asteroidImage.h = asteroidTab[i].spritetaille;
			SDL_BlitSurface(&(asteroidTab[i].image), &asteroidImage, screen, &asteroidTab[i].pi);
		}

		// Shoots 
		for(i=0;i<nbShoot;i++)
		{
			colorkeyShoot = SDL_MapRGB(screen->format, 255, 125, 0);
			SDL_SetColorKey(&shootTab[i].image, SDL_SRCCOLORKEY | SDL_RLEACCEL, colorkeyShoot);
			
			sprite_move(&shootTab[i]);
			exitscreen(&shootTab[i]);
			
			SDL_Rect shootImage;

			shootImage.x=0;
			shootImage.y=0;
			shootImage.w = shootTab[i].spritetaille;
			shootImage.h = shootTab[i].spritetaille;
			SDL_BlitSurface(&(shootTab[i].image), &shootImage, screen, &shootTab[i].pi);
		}
		exit_shoot();


		// Vaisseau
		sprite_move(&vaisseau);
		exitscreen(&vaisseau);
		{
			SDL_Rect shipImage;
			shipImage.y = 0;
			shipImage.w = ship_size;
			shipImage.h = ship_size;
			shipImage.x = ship_size*vaisseau.angle;
			SDL_BlitSurface(&(vaisseau.image), &shipImage, screen, &vaisseau.pi);
		}
	
		testColision();

		// Animations
		anim_cycle();
		for(i=0;i<nbAnim;i++)
		{
			colorkeyAnim = SDL_MapRGB(screen->format, 255, 0, 255);
			SDL_SetColorKey(&animTab[i].image, SDL_SRCCOLORKEY | SDL_RLEACCEL, colorkeyAnim);
			
			SDL_Rect animImage;

			animImage.x=animTab[i].angle*animTab[i].spritetaille;
			animImage.y=0;
			animImage.w = animTab[i].spritetaille;
			animImage.h = animTab[i].spritetaille;
			SDL_BlitSurface(&(animTab[i].image), &animImage, screen, &animTab[i].pi);
		}

		SDL_UpdateRect(screen, 0, 0, 0, 0);

	}

	SDL_FreeSurface(background);
	Mix_FreeChunk(bruitTir);//Libération du son 1
	Mix_FreeMusic(musique);
	Mix_CloseAudio(); //Fermeture de l'API

	
//cleanup SDL
	SDL_Quit();
	return 0;
}