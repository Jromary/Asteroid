#ifndef SHOOT_H
#define SHOOT_H

/* Initialisation d'un sprite_t avec les propriétés d'un tir */
extern void sprite_shoot_init(sprite_t *sp1, sprite_t *sp2);

/* Ajoute un shoot au tableau de shoot */
extern void sprite_shoot(sprite_t *sp);

/* Initialise trois shoot dont les angles different au tableau */
extern void sprite_shoot_init_multi(sprite_t *sp1, sprite_t *sp2, int differenceAngle);

/* Ajoute les trois shoot a shootTab */
extern void sprite_multi_shoot(sprite_t *sp);

/* Retire les tirs inférieurs à une certaine vitesse */
extern void exit_shoot();

/* Supprime le shoot avec l'id id_cible du tableau de shoot */
extern void moinsShoot(int id_cible);

#endif /* SHOOT_H */
