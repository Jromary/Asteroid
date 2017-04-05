#include <SDL.h>
#include <math.h>
#include <time.h>

#define screen_height 480
#define screen_length 640
#define PI 3.1416
#define COEFF_FROTTEMENT 0.001
#define COEFF_BOOST 0.01
#define ASTEROID_MAX 10
#define SHOOT_MAX 1000
#define bullet_size 8

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
	Pointreel pr;//position reel
	SDL_Rect pi;//position image
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

/* Bullet */
void sprite_shoot_init(Sprite *sp1, Sprite *sp2){
	SDL_Surface *temp, *shootImage;
	Pointreel npr;
	/* ERREUR */
	npr.rx=(*sp2).accel.rx;
	npr.ry=(*sp2).accel.ry;
	Pointreel npr2;
	npr2.rx=(*sp2).speed.rx * 2;
	npr2.ry=(*sp2).speed.ry * 2;
	/* ERREUR */
	temp = SDL_LoadBMP("bullet02.bmp");
	shootImage = SDL_DisplayFormat(temp);
	SDL_FreeSurface(temp);
	sprite_init(sp1, (*sp2).pr, (*sp2).pi, (*sp2).angle, npr2, npr, *shootImage, bullet_size);
}

Sprite shootTab[SHOOT_MAX];
int nbShoot = 0;
void sprite_shoot(Sprite *sp){

	
	if(nbShoot < SHOOT_MAX)
	{
		Sprite shoot;
		sprite_shoot_init(&shoot,sp);
		shootTab[nbShoot] = shoot;
		nbShoot++;	
	}
	else
	{
		printf("max shoot\n");
	}
	return;
}
void exit_shoot(){
	int i,j;
	for (i=0; i<nbShoot; i++){
		if (shootTab[i].speed.rx<0.01 && shootTab[i].speed.ry<0.01){
			for (j=i; j<nbShoot; j++){
				shootTab[j] = shootTab[j+1];
			}
			nbShoot = nbShoot - 1;
		}
	}
}



/* Partie Asteroide  */
const float VITESSE_ASTEROID = 0.3;
void asteroid_init(Sprite *sp,int taille)
{
	int testPosition = rand()%4;
	SDL_Rect position;
	switch(testPosition)
	{
		/* Apparition sur les bords de l'ecran */
		case 0:
			position.x = 0;
			position.y = rand()%screen_height;
			break;
		case 1:
			position.x = rand()%(screen_length-(taille*32)); // On evite que le sprite déborde de l'ecran au spawn
			position.y = 0;
			break;
		case 2:
			position.x = screen_length-(taille*32);
			position.y = rand()%(screen_height-(taille*32));
			break;
		case 3:
			position.x = rand()%(screen_length-(taille*32));
			position.y = screen_height-(taille*32);
			break;
		default:
			position.x = 0;
			position.y = 0;
			break;
	}
	Pointreel npr;
	npr.rx = position.x;
	npr.ry = position.y;

	int testDirection;
	Pointreel direction;
	testDirection = rand()%4;
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
	Pointreel asteroidAccel;
	asteroidAccel.rx = direction.rx * (rand()%100)/1000.;
	asteroidAccel.ry = direction.ry * (rand()%100)/1000.;
	Pointreel asteroidSpeed;
	asteroidSpeed.rx = 0;
	asteroidSpeed.ry = 0;

	SDL_Surface* temp, *asteroidImage;
	//test de taille a faire
	temp = SDL_LoadBMP("asteroid.bmp");
	asteroidImage = SDL_DisplayFormat(temp);
	SDL_FreeSurface(temp);

	sprite_init(sp, npr, position, rand()%36, asteroidSpeed, asteroidAccel,*asteroidImage, 32*taille);
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
		asteroid_init(&new,1);
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
			nbAsteroid--;
		}
		else
		{
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

