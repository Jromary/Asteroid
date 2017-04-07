#include <SDL.h>
#include <SDL/SDL_mixer.h>
#include <time.h>

#include "Struct.c"


#define ship_size 32
#define ship_angle 0
/* comande de compilation:
gcc -Wall -g -std=c99 Main.c `sdl-config --cflags --libs` -lm -o Main
*/




/************** DEBUG ****/
void eventact(SDL_Event event, int* exit, Sprite *sp,Mix_Chunk *son){
	switch (event.type){
//evenement croix en haut a gauche
		case SDL_QUIT:
			*exit=1;
			break;
			
		case SDL_KEYDOWN:
            switch (event.key.keysym.sym) {
//event escape
				case SDLK_ESCAPE:
					*exit=1;
					break;
				// Asteroid
				case SDLK_p:
						plusAsteroid();
					break;
				case SDLK_o:
						splitAsteroid();
					break;
				// MOUVEMENT
				case SDLK_z:
					sprite_boost(sp);
					break;
				case SDLK_q:
					sprite_turn_left(sp);
					break;
				case SDLK_d:
					sprite_turn_right(sp);
					break;
				case SDLK_x:
					playSound(son);
					break;
				case SDLK_m:
					affichagePositionAsteroid();
					break;
				default:
					break;
			}
			break;
	}
}
/************* FIN DEBUG ****/

int main(int argc, char* argv[]){
// INIT RANDOM //
	srand(time(NULL));


	// *barspeed, *cadrespeed,
	SDL_Surface *ship, *temp, *background;
	SDL_Init(SDL_INIT_VIDEO);
	SDL_WM_SetCaption("Space","Space");
	SDL_Surface* screen = SDL_SetVideoMode(screen_length, screen_height, 0, 0);
	
	if(Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 1024) == -1) //Initialisation de l'API Mixer
	{
	   printf("%s", Mix_GetError());
	}
	Mix_Music *musique;
	musique = Mix_LoadMUS("asteroid.mp3");
	Mix_PlayMusic(musique, -1); //Jouer infiniment la musique

	Mix_AllocateChannels(32); //Allouer 32 canaux
	Mix_Chunk *bruitTir;//Créer un pointeur pour stocker un .WAV
	bruitTir = Mix_LoadWAV("laser.wav"); //Charger un wav dans un pointeur
	Mix_VolumeChunk(bruitTir, MIX_MAX_VOLUME); //Mettre un volume pour ce wav
	// Init de SDL dans un autre fichier ? 
	// pour avoir une colorkey globale et detourer tout les spritos
	int colorkey, colorkeyAsteroid;
	SDL_EnableKeyRepeat(10, 10);
	temp = SDL_LoadBMP("greenship-v1.bmp");
	ship = SDL_DisplayFormat(temp);
	SDL_FreeSurface(temp);
	colorkey = SDL_MapRGB(screen->format, 255, 0, 255);
	SDL_SetColorKey(ship, SDL_SRCCOLORKEY | SDL_RLEACCEL, colorkey);
	temp = SDL_LoadBMP("backgroundlvl1.bmp");
	background = SDL_DisplayFormat(temp);
	SDL_FreeSurface(temp);

	Sprite bateau;

	Pointreel npr;
	npr.rx=100;
	npr.ry=100;
	SDL_Rect npi;
	npi.x=100;
	npi.y=100;
	Pointreel speed_bateau;
	speed_bateau.rx = 0;
	speed_bateau.ry = 0;
	Pointreel accel_bateau;
	accel_bateau.rx = 0.001;
	accel_bateau.ry = 0.001;

	sprite_init(&bateau,npr,npi,ship_angle, speed_bateau, accel_bateau, *ship, ship_size);
	
	int gameover = 0;

	/* DEBUG */
	int i;
	plusAsteroid();
	/************/

	while(!gameover){

		SDL_Event event;

		/* JUSTE POUR EVITER WHILE 1 */

		if (SDL_PollEvent(&event)){
			eventact(event,&gameover,&bateau,bruitTir);
		}

		// printf("%f ---- %f\n",bateau.pr.rx,bateau.pr.ry);

		sprite_move(&bateau);
		exitscreen(&bateau);

		SDL_BlitSurface(background,NULL,screen,NULL);
		{
			SDL_Rect shipImage;
			shipImage.y = 0;
			shipImage.w = ship_size;
			shipImage.h = ship_size;
			shipImage.x = ship_size*bateau.angle;
			SDL_BlitSurface(&(bateau.image), &shipImage, screen, &bateau.pi);
		}



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










