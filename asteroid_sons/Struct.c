#include <SDL.h>
#include <math.h>
#include <time.h>

#define screen_height 480
#define screen_length 640
#define PI 3.1416
#define COEFF_FROTTEMENT 0.001
#define COEFF_BOOST 0.01
#define ASTEROID_MAX 10
#define size_asteroid_min 16
#define size_asteroid_max 64
#define size_asteroid_med 32
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
	Pointreel accel;
	Pointreel speed;
	SDL_Surface image;
	int spritetaille;
};
typedef struct Sprite Sprite;
void sprite_init(Sprite *sp, Pointreel npr, SDL_Rect npi, int ang, Pointreel spee, Pointreel acc, SDL_Surface surface, int spriteta)
{
	(*sp).pr.rx=npr.rx;
	(*sp).pr.ry=npr.ry;
	(*sp).pi.x=npi.x;
	(*sp).pi.y=npi.y;
	(*sp).angle=ang;
	(*sp).accel.rx=acc.rx;
	(*sp).accel.ry=acc.ry;
	(*sp).speed.rx=spee.rx;
	(*sp).speed.ry=spee.ry;
	(*sp).image=surface;
	(*sp).spritetaille=spriteta;
}
void sprite_turn_right(Sprite* sp)
{
	if ((*sp).angle>0){
		(*sp).angle = (*sp).angle - 1;
	}
	else {
		(*sp).angle = 35;
	}
}
void sprite_turn_left(Sprite* sp)
{
	if ((*sp).angle<35){
		(*sp).angle = (*sp).angle + 1;
	}
	else {
		(*sp).angle = 0;
	}
}

void sprite_move(Sprite* sp)
{
	//	temp = (((*sp).angle)*10) * (PI/180.0);
	(*sp).speed.rx += (*sp).accel.rx - (*sp).speed.rx * COEFF_FROTTEMENT;
	(*sp).speed.ry += (*sp).accel.ry - (*sp).speed.ry * COEFF_FROTTEMENT;
	(*sp).accel.rx=0;
	(*sp).accel.ry=0;
	(*sp).pr.rx += (*sp).speed.rx;
	(*sp).pr.ry += (*sp).speed.ry;
	//	(*sp).pr.rx = ((*sp).pr.rx) + (*sp).speed * cos(temp) - (*sp).speed * COEFF_FROTTEMENT;
	//	(*sp).pr.ry = ((*sp).pr.ry) + (*sp).speed * sin(temp) - (*sp).speed * COEFF_FROTTEMENT;
	(*sp).pi.x = (int)(*sp).pr.rx;
	(*sp).pi.y = (int)(*sp).pr.ry;
}

void sprite_boost(Sprite* sp){
	float tmp;
	tmp = (((*sp).angle)*10) * (PI/180.0);
	(*sp).accel.rx = cos(tmp) * COEFF_BOOST;
	(*sp).accel.ry = -sin(tmp) * COEFF_BOOST;
}

///////////////
// EXITSCREEN
void exitscreen(Sprite *sp){
	if ((*sp).pr.rx > screen_length){
		(*sp).pr.rx = 0;
	}
	if ((*sp).pr.rx < 0){
		(*sp).pr.rx = screen_length;
	}
	if ((*sp).pr.ry > screen_height){
		(*sp).pr.ry = 0;
	}
	if ((*sp).pr.ry < 0){
		(*sp).pr.ry = screen_height;
	}
}



//const float VITESSE_ASTEROID = 0.3;
void asteroid_init(Sprite *sp)
{
	SDL_Surface* temp, *asteroidImage;
	/* Taille de l'asteroid, 16, 32, ou 64 */
	int taille;
	int testTaille =(rand()%3)+1;
	switch(testTaille)
	{
		case 1:
			taille = size_asteroid_min;
			temp = SDL_LoadBMP("asteroid16.bmp");
			asteroidImage = SDL_DisplayFormat(temp);
			SDL_FreeSurface(temp);
			break;
		case 2:
			taille = size_asteroid_med;
			temp = SDL_LoadBMP("asteroid32.bmp");
			asteroidImage = SDL_DisplayFormat(temp);
			SDL_FreeSurface(temp);
			break;
		case 3:
			taille = size_asteroid_max;
			temp = SDL_LoadBMP("asteroid64.bmp");
			asteroidImage = SDL_DisplayFormat(temp);
			SDL_FreeSurface(temp);
			break;
		default:
			temp = SDL_LoadBMP("asteroid.bmp");
			asteroidImage = SDL_DisplayFormat(temp);
			SDL_FreeSurface(temp);
			break;
	}


	/* Position sur les bords de l'ecran */
	int testPosition = rand()%4;
	SDL_Rect position;
	switch(testPosition)
	{
		
		case 0:
			position.x = 0;
			position.y = rand()%screen_height;
			break;
		case 1:
			position.x = rand()%(screen_length-(taille)); // On evite que le sprite déborde de l'ecran au spawn
			position.y = 0;
			break;
		case 2:
			position.x = screen_length-(taille);
			position.y = rand()%(screen_height-(taille));
			break;
		case 3:
			position.x = rand()%(screen_length-(taille));
			position.y = screen_height-(taille);
			break;
		default:
			position.x = 0;
			position.y = 0;
			break;
	}
	Pointreel npr;
	npr.rx = position.x;
	npr.ry = position.y;

	/* Direction Haut bas droite gauche aleatoire */
	int testDirection = rand()%4;;
	Pointreel direction;
	switch(testDirection)
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
	/* Acceleration et vitesse */
	Pointreel asteroidAccel;
	asteroidAccel.rx = direction.rx * (rand()%100)/1000.;
	asteroidAccel.ry = direction.ry * (rand()%100)/1000.;
	Pointreel asteroidSpeed;
	asteroidSpeed.rx = 0;
	asteroidSpeed.ry = 0;
	

	/*
	temp = SDL_LoadBMP("asteroid.bmp");
	asteroidImage = SDL_DisplayFormat(temp);
	SDL_FreeSurface(temp);
	*/
	sprite_init(sp, npr, position, rand()%36, asteroidSpeed, asteroidAccel,*asteroidImage, taille);
	return;
}


///////////////
Sprite asteroidTab[ASTEROID_MAX];
int nbAsteroid = 0;

// Ajoute un asteroid au tableau d'asteroid global 
//et incremente le nombre d'asteroid présent
void plusAsteroid()
{
	if(nbAsteroid < ASTEROID_MAX)
	{
		// Taille a 1 en attendant implabntation des differentes tailles
		Sprite new;
		asteroid_init(&new);
		asteroidTab[nbAsteroid] = new;
		nbAsteroid++;	
	}
	else
	{
		printf("Nombre d'asteroides dépassé 11 11\n");
	}
	return;
}

//POUR LE MOMENT, retire un asteroid aleatoire du tableau d'asteroid global
// et décremente le nb d'asteroid présent
// BUT : Au collision avec un tir, supprimer l'asteroid correspondant.
void moinsAsteroid()
{
	if(nbAsteroid == 0)
	{
		printf("Aucun asteroide a supprimer.\n");
	}
	else
	{
		int k = rand()%nbAsteroid;
		if(k==nbAsteroid-1)
		{
			// libération de la mémoire du sprite
			// SDL_FreeSurface(&asteroidTab[k].image);
			nbAsteroid--;
		}
		else
		{
			// SDL_FreeSurface(&asteroidTab[k].image);
			asteroidTab[k]=asteroidTab[k+1];
			nbAsteroid--;
		}
	}
	return;
}

void asteroid_move(Sprite* sp)
{
	//	temp = (((*sp).angle)*10) * (PI/180.0);
	(*sp).speed.rx += (*sp).accel.rx;
	(*sp).speed.ry += (*sp).accel.ry;
	(*sp).accel.rx=0;
	(*sp).accel.ry=0;
	(*sp).pr.rx += (*sp).speed.rx;
	(*sp).pr.ry += (*sp).speed.ry;
	//	(*sp).pr.rx = ((*sp).pr.rx) + (*sp).speed * cos(temp) - (*sp).speed * COEFF_FROTTEMENT;
	//	(*sp).pr.ry = ((*sp).pr.ry) + (*sp).speed * sin(temp) - (*sp).speed * COEFF_FROTTEMENT;
	(*sp).pi.x = (int)(*sp).pr.rx;
	(*sp).pi.y = (int)(*sp).pr.ry;
}



/////////////
// SONS

void playSound(Mix_Chunk *son)
{
	Mix_PlayChannel(1, son, 0);
}