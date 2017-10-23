game: globales.c anim.c shoot.c bonus.c collider.c event.c sprite.c main.c shoot.h bonus.h globales.h event.h collider.h sprite.h anim.h menu.c menu.h asteroid.c asteroid.h
	gcc -Wall -g -std=c99 globales.c anim.c sprite.c event.c collider.c shoot.c bonus.c asteroid.c main.c `sdl-config --cflags --libs` -lm  -o game
