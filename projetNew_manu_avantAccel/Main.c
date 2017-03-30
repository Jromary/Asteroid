#include <SDL.h>
#include <time.h>

#include "Struct.c"


#define ship_size 32
#define ship_angle 0
/* comande de compilation:
gcc -Wall -g -std=c99 Main.c `sdl-config --cflags --libs` -lm -o Main
*/


/************** DEBUG ****/
void eventact(SDL_Event event, int* exit){
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
				case SDLK_p:
						plusAsteroid();
					break;
				case SDLK_o:
						moinsAsteroid();
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
	SDL_Surface *ship, *temp, *background, *surf_asteroid;
	SDL_Init(SDL_INIT_VIDEO);
	SDL_WM_SetCaption("Space","Space");
	SDL_Surface* screen = SDL_SetVideoMode(screen_length, screen_height, 0, 0);
	
	// Init de SDL dans un autre fichier ? 
	// pour avoir une colorkey globale et detourer tout les spritos
	int colorkey;
	SDL_EnableKeyRepeat(10, 10);
	temp = SDL_LoadBMP("vesso.bmp");
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


	int i;
	plusAsteroid();
	plusAsteroid();
	printf("nb asteroid : %d\n",nbAsteroid);
	
	sprite_init(&bateau,npr,npi,ship_angle, 0, *ship, ship_size);
	int gameover = 0;
	while(!gameover){
		SDL_Event event;


		/******************* JUSTE POUR EVITER WHILE 1 */
		if (SDL_PollEvent(&event)){
			eventact(event,&gameover);
		}


		//printf("%d ---- %d\n",asteroid.pi.x,asteroid.pi.y);




		SDL_BlitSurface(background,NULL,screen,NULL);
		{
			SDL_Rect shipImage;
			shipImage.y = 0;
			shipImage.w = ship_size;
			shipImage.h = ship_size;
			shipImage.x = ship_size*bateau.angle;
			SDL_BlitSurface(&(bateau.image), &shipImage, screen, &bateau.pi);
		}



		for(i=0;i<nbAsteroid;i++)
		{
			//printf("%d----------%d\n",asteroidTab[i].pi.x,asteroidTab[i].pi.y);
			printf("nb asteroid : %d\n",nbAsteroid);

			SDL_Rect asteroidImage;
			asteroidImage.x=0;
			asteroidImage.y=0;
			asteroidImage.w = asteroidTab[i].spritetaille;
			asteroidImage.h = asteroidTab[i].spritetaille;
			SDL_BlitSurface(&(asteroidTab[i].image), &asteroidImage, screen, &asteroidTab[i].pi);
		}
		SDL_UpdateRect(screen, 0, 0, 0, 0);
	}



	 SDL_FreeSurface(background);

//cleanup SDL
	SDL_Quit();
	return 0;
}










