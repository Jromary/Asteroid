#include <SDL.h>
#include <math.h>
#define PI 3.1416
#define COEFF_FROTTEMENT 0.001
//header:

//
struct Pointreel
{
	float rx;
	float ry;
};
typedef struct Pointreel Pointreel;
struct Sprite
{
	Pointreel pr;
	SDL_Rect pi;
	int angle;
	//float accel;
	float speed;
	SDL_Surface image;
	int spritetaille;
};
typedef struct Sprite Sprite;
void sprite_init(Sprite *sp, Pointreel npr, SDL_Rect npi, int ang, float spee, SDL_Surface surface, int spriteta)
{
	(*sp).pr.rx=npr.rx;
	(*sp).pr.ry=npr.ry;
	(*sp).pi.x=npi.x;
	(*sp).pi.y=npi.y;
	(*sp).angle=ang;
	//(*sp).accel=acc;
	(*sp).speed=spee;
	(*sp).image=surface;
	(*sp).spritetaille=spriteta;
}
void sprite_turn_right(Sprite* sp)
{
	(*sp).angle=((*sp).angle-1)%36;
}
void sprite_turn_left(Sprite* sp)
{
	(*sp).angle=((*sp).angle+1)%36;
}
void sprite_move(Sprite* sp)
{
	float temp;
	temp = (((*sp).angle)*10) * (PI/180.0);
	(*sp).pr.rx=((*sp).pr.rx) + (*sp).speed * cos(temp) - (*sp).speed * COEFF_FROTTEMENT;
	(*sp).pr.ry=((*sp).pr.ry) + (*sp).speed * sin(temp) - (*sp).speed * COEFF_FROTTEMENT;
	(*sp).pi.x += (int)(*sp).pr.rx;
	(*sp).pi.y += (int)(*sp).pr.ry;
	
}




