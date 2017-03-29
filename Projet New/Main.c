#include <SDL.h>
#include "Struct.c"

#define screen_hight 480
#define screen_lenth 640
#define ship_size 32
#define ship_angle 0
/* comande de compilation:
gcc -Wall -g -std=c99 Main.c `sdl-config --cflags --libs` -lm -o Main
*/

int main(int argc, char* argv[]){
	SDL_Surface *ship, *temp, *background, *barspeed, *cadrespeed;
	SDL_Init(SDL_INIT_VIDEO);
	SDL_WM_SetCaption("Space","Space");
	SDL_Surface* screen = SDL_SetVideoMode(screen_lenth, screen_hight, 0, 0);
	int colorkey;
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
	sprite_init(&bateau,npr,npi,ship_angle, 0, *ship, ship_size);
	int gameover = 0;
	while(!gameover){
		SDL_Event event;
		SDL_BlitSurface(background,NULL,screen,NULL);
		{
			SDL_Rect shipImage;
			shipImage.y = 0;
			shipImage.w = ship_size;
			shipImage.h = ship_size;
			shipImage.x = ship_size*bateau.angle;
			SDL_BlitSurface(&(bateau.image), &shipImage, screen, &bateau.pi);
		}
		SDL_UpdateRect(screen, 0, 0, 0, 0);
	}
	SDL_FreeSurface(background);

//cleanup SDL
	SDL_Quit();
	return 0;
}










