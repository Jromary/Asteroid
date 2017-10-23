#ifndef EVENT_H
#define EVENT_H

#include "globales.h"

/* Gestion des evenements clavier classiques */
extern void eventact(char *key, sprite_t *sp);

/* Gestion de la pression de touche(s) */
extern void update_events(char *keys);

/* Gestion des evenements clavier malusse */
extern void eventact_malus(char *key, sprite_t *sp);



#endif /* EVENT_H */
