#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <SDL.h>
#include <SDL/SDL_mixer.h>
#include "variables.h"

// Fonctions sur les Sprites
extern void sprite_init(Sprite *sp, Pointreel npr, SDL_Rect npi, int ang, Pointreel spee, Pointreel acc, SDL_Surface surface, int spriteta);
extern void sprite_turn_right(Sprite* sp);
extern void sprite_turn_left(Sprite* sp);
extern void sprite_move(Sprite* sp);
extern void sprite_boost(Sprite* sp, float boost);
extern void exitscreen(Sprite *sp);

// Fonctions sur les asteroids
extern void asteroid_init(Sprite *sp);
extern void plusAsteroid();// Ajoute un asteroid au tableau d'asteroid global et incremente le nombre d'asteroid présent
extern void moinsAsteroid(int id_cible);
extern void asteroid_move(Sprite* sp); // Déplacement sans frottement (ne s'arrete jamais)
extern void affichagePositionAsteroid(); // Fonction pour le développement
extern void fragmentationAsteroid(int id_cible); // Fonction qui ajoute entre 1 et 3 fragments de la moitié de la taille de la cible
extern void splitAsteroid(); // Créer les fragments et supprime l'asteroid ciblé

// Shoot
extern void sprite_shoot_init(Sprite *sp1, Sprite *sp2);
extern void sprite_shoot(Sprite *sp);
extern void exit_shoot();
extern void moinsShoot(int id_cible);

// Sons
extern void playSound(Mix_Chunk *son);

// Gestion d'evenements
extern void eventact(SDL_Event event, int* exit, Sprite *sp,Mix_Chunk *son);
extern long int distanceEntreSprites(Sprite objet1, Sprite objet2); // long int pour stocker les carrés


#endif /* FUNCTIONS_H */