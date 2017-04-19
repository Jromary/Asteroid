#include <SDL.h>
#include <SDL/SDL_mixer.h>
#include <math.h>
#include <time.h>
#include <unistd.h>

#include "functions.h"
#include "variables.h"

extern Sprite asteroidTab[ASTEROID_MAX];
extern int nbAsteroid;
extern Sprite shootTab[SHOOT_MAX];
extern int nbShoot;
extern int gameover;

/********* FONCTIONS SUR LES SPRITES *********/
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
	(*sp).speed.rx += (*sp).accel.rx - (*sp).speed.rx * COEFF_FROTTEMENT;
	(*sp).speed.ry += (*sp).accel.ry - (*sp).speed.ry * COEFF_FROTTEMENT;
	(*sp).accel.rx=0;
	(*sp).accel.ry=0;
	(*sp).pr.rx += (*sp).speed.rx;
	(*sp).pr.ry += (*sp).speed.ry;
	(*sp).pi.x = (int)(*sp).pr.rx;
	(*sp).pi.y = (int)(*sp).pr.ry;
}

void sprite_boost(Sprite* sp, float boost){
	float tmp;
	tmp = (((*sp).angle)*10) * (PI/180.0);
	(*sp).accel.rx = cos(tmp) * boost;
	(*sp).accel.ry = -sin(tmp) * boost;
}

void exitscreen(Sprite *sp)
{
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


/********* FONCTIONS SUR LES ASTEROIDS *********/
void asteroid_init(Sprite *sp)
{
	SDL_Surface* temp, *asteroidImage;
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

void plusAsteroid()
{
	if(nbAsteroid < ASTEROID_MAX)
	{
		Sprite new;
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

//POUR LE MOMENT, retire un asteroid aleatoire du tableau d'asteroid global
// et décremente le nb d'asteroid présent
// BUT : A la collision avec un tir, supprimer l'asteroid correspondant.
void moinsAsteroid(int id_cible)
{
	if(nbAsteroid == 0)
	{
		printf("ERREUR : appel de moinsAsteroid alors que le tableau est vide\n");
		printf("Aucun asteroide a supprimer.\n");
	}
	else
	{
		if(id_cible == nbAsteroid-1)
		{
			// libération de la mémoire du sprite
			// SDL_FreeSurface(&asteroidTab[k].image);
			nbAsteroid--;
		}
		else
		{
			// SDL_FreeSurface(&asteroidTab[k].image);
			int i;
			for(i = id_cible; i<= nbAsteroid; i++)
			{
				asteroidTab[i] = asteroidTab[i+1];
			}
			nbAsteroid--;
		}
	}
	return;
}

void asteroid_move(Sprite* sp)
{
	(*sp).speed.rx += (*sp).accel.rx;
	(*sp).speed.ry += (*sp).accel.ry;
	(*sp).accel.rx=0;
	(*sp).accel.ry=0;
	(*sp).pr.rx += (*sp).speed.rx;
	(*sp).pr.ry += (*sp).speed.ry;
	(*sp).pi.x = (int)(*sp).pr.rx;
	(*sp).pi.y = (int)(*sp).pr.ry;
}

void affichagePositionAsteroid()
{
	printf(">>>>>>>>>>>>>>>>>>>>>> NB ASTEROID : %d\n",nbAsteroid);
	int l;
	for(l=0; l<nbAsteroid; l++)
	{
		printf("position ast [%d] = %d - %d\n",l,asteroidTab[l].pi.x,asteroidTab[l].pi.y);
		printf("speed ast [%d] = %f - %f\n",l,asteroidTab[l].speed.rx,asteroidTab[l].speed.ry);
		printf("accel ast [%d] = %f - %f\n",l,asteroidTab[l].accel.rx,asteroidTab[l].accel.ry);
		printf("taille ast [%d] = %d\n",l,asteroidTab[l].spritetaille);
		printf("angle ast [%d] = %d\n",l,asteroidTab[l].angle);
	}
	printf(">>>>>>>>>>>>>>>>>>>>>>\n\n");
}

void fragmentationAsteroid(int id_cible)
{
	int nbFragment = (rand()%3)+1;
	if(nbAsteroid + nbFragment < ASTEROID_MAX)
	{
		int j;
		for (j = 0; j < nbFragment; j++)
		{
			Sprite new;
			SDL_Surface* temp, *asteroidImage;
			switch(asteroidTab[id_cible].spritetaille)
			{
				case 32:
					temp = SDL_LoadBMP("asteroid16.bmp");
					asteroidImage = SDL_DisplayFormat(temp);
					SDL_FreeSurface(temp);
					break;
				case 64:
					temp = SDL_LoadBMP("asteroid32.bmp");
					asteroidImage = SDL_DisplayFormat(temp);
					SDL_FreeSurface(temp);
					break;
				default:
					printf("Erreur fragmentation \n");
					break;
			}
			/* Direction Haut bas droite gauche aleatoire */
			int testDirection = rand()%4;
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
			//sprite_init(sp, npr, position, rand()%36, asteroidSpeed, asteroidAccel,*asteroidImage, taille);
			sprite_init(&new, asteroidTab[id_cible].pr, asteroidTab[id_cible].pi,0,asteroidSpeed, asteroidAccel,*asteroidImage, asteroidTab[id_cible].spritetaille/2);
			asteroidTab[nbAsteroid] = new;
			nbAsteroid++;

			//affichagePositionAsteroid();
			// sleep(5);
		}
	}
	else
	{
		printf("Nombre d'asteroides dépassé, pas de nouveaux fragments.\n");
	}
	return;
}

//POUR LE MOMENT, pred un asteroid au hasard et le supprime si taille min
// ou le casse en plusieurs morceaux si taille>min.
// void sprite_init(Sprite *sp, Pointreel npr, SDL_Rect npi, int ang, Pointreel spee, Pointreel acc, SDL_Surface surface, int spriteta)
void splitAsteroid(int k)
{
	/* debug : sinon modulo 0 quand 0 asteroid */
	/* dans les fait on n'appelera jamais cette fonction puisque il n'y aura aucun asteroid
	a toucher */
	if(nbAsteroid == 0)
	{
		return;
	}
	else
	{
		//int k = rand()%nbAsteroid;
		if (asteroidTab[k].spritetaille > 16)
		{
			fragmentationAsteroid(k);
			moinsAsteroid(k);	
		}
		else{
			moinsAsteroid(k);
		}
	}
}

/********* SHOOT *********/
void sprite_shoot_init(Sprite *sp1, Sprite *sp2){
	SDL_Surface *temp, *shootImage;
	Pointreel npr;
	Pointreel posr;
	SDL_Rect plop;
	/* ERREUR */
	npr.rx=0;
	npr.ry=0;
	Pointreel npr2;
	npr2.rx=0;
	npr2.ry=0;
	/* ERREUR */
	temp = SDL_LoadBMP("bullet02.bmp");
	shootImage = SDL_DisplayFormat(temp);
	SDL_FreeSurface(temp);
	posr.rx = (*sp2).pr.rx+(ship_size)/2;
	posr.ry = (*sp2).pr.ry+(ship_size)/2;
	plop.x = (int)posr.rx;
	plop.y = (int)posr.ry;
	sprite_init(sp1, posr, plop, (*sp2).angle, npr2, npr, *shootImage, bullet_size);
	sprite_boost(sp1, 0.5);
}

void sprite_shoot(Sprite *sp)
{	
	if(nbShoot < SHOOT_MAX)
	{
		Sprite shoot;
		sprite_shoot_init(&shoot,sp);
		shootTab[nbShoot] = shoot;
		nbShoot++;	
	}
	else
	{
		//printf("max shoot\n");
	}
	return;
}
void exit_shoot()
{
	int i,j;
	for (i=0; i<nbShoot; i++){
		if (fabs(shootTab[i].speed.rx)<0.1 && fabs(shootTab[i].speed.ry)<0.1){
			for (j=i; j<nbShoot; j++){
				shootTab[j] = shootTab[j+1];
			}
			nbShoot = nbShoot - 1;
		}
	}
}
void moinsShoot(int id_cible)
{
	if(nbShoot == 0)
	{
		printf("ERREUR : appel de moinsAsteroid alors que le tableau est vide\n");
		printf("Aucun asteroide a supprimer.\n");
	}
	else
	{
		if(id_cible == nbShoot-1)
		{
			// libération de la mémoire du sprite
			// SDL_FreeSurface(&shootTab[k].image);
			nbShoot--;
		}
		else
		{
			// SDL_FreeSurface(&shootTab[k].image);
			int i;
			for(i = id_cible; i<= nbShoot; i++)
			{
				shootTab[i] = shootTab[i+1];
			}
			nbShoot--;
		}
	}
	return;
}

/********* SONS *********/

void playSound(Mix_Chunk *son)
{
	Mix_PlayChannel(1, son, 0);
}

/********* GESTION D'EVENEMENTS *********/
void eventact(SDL_Event event, Sprite *sp,Mix_Chunk *son)
{
	int k = rand()%nbAsteroid;
	switch (event.type)
	{
		case SDL_QUIT:
			gameover=1;
			break;
			
		case SDL_KEYDOWN:
            switch (event.key.keysym.sym) 
            {
				case SDLK_ESCAPE:
					gameover=1;
					break;

				// Asteroids
				case SDLK_p:
					plusAsteroid();
					break;
				case SDLK_o:
					splitAsteroid(k);
					break;

				// Mouvements
				case SDLK_z:
					sprite_boost(sp, COEFF_BOOST);
					break;
				case SDLK_q:
					sprite_turn_left(sp);
					break;
				case SDLK_d:
					sprite_turn_right(sp);
					break;

				// Sons
				case SDLK_x:
					playSound(son);
					break;
				// Affichage
				case SDLK_m:
					affichagePositionAsteroid();
					break;

				// Shoot
				case SDLK_SPACE:
					sprite_shoot(sp);
					break;

				default:
					break;
			}
			break;
	}
}


/*** GESTION DES COLISIONS ***/
/* calcule des distances */
long int distanceEntreSprites(Sprite objet1, Sprite objet2)
{
	int rayon1 = objet1.spritetaille/2;
	int rayon2 = objet2.spritetaille/2;
	int ax,ay;
	ax = objet1.pi.x + rayon1;
	ay = objet1.pi.y + rayon1;
	int bx,by;
	bx = objet2.pi.x + rayon2;
	by = objet2.pi.y + rayon2;
	// La distance totale entre les deux centres d'objets est
	// Le rayon du premier objet plus une distance X plus le deuxième rayon
	// donc ce qui nous interesse est la distance X.
	return (sqrt( pow((bx-ax),2) + pow((by-ay),2) ) - rayon1 - rayon2);
}



void testColision(){
	int i,j;
	if((nbAsteroid>0) && (nbShoot>0)){
		for (i=0;i<nbShoot;i++){
			for (j=0;j<nbAsteroid;j++){
				if (distanceEntreSprites(shootTab[i],asteroidTab[j]) == 0){
					moinsShoot(i);
					splitAsteroid(j);
				}
			}
		}
	}
}