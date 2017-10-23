/***********************************************/
/********* FONCTIONS SUR LES COLLISIONS ********/
/***********************************************/

#include <stdbool.h>
#include <SDL.h>

#include "collider.h"

bool collide_test(sprite_t *sprite1, sprite_t *sprite2,
				  SDL_PixelFormat *format, int *cu, int *cv)
{
		/* rough test using the bounding boxes (bb) only */
		bool test = !(sprite2->pi.x > sprite1->pi.x + sprite1->spritetaille ||
					  sprite2->pi.x + sprite2->spritetaille < sprite1->pi.x ||
					  sprite2->pi.y > sprite1->pi.y + sprite1->image->h ||
					  sprite2->pi.y + sprite2->image->h < sprite1->pi.y);

		/* if the rough test succeeded, a fine test is performed using colorkeys
		 (transparency colors) of the sprites (may be optimized!) */
		if (test)
		{
			Uint32 *bmp1 = (Uint32 *)malloc(sizeof(Uint32) * sprite1->spritetaille * sprite1->image->h), *sprite_it, *bmp_it;
			Uint32 *bmp2 = (Uint32 *)malloc(sizeof(Uint32) * sprite2->spritetaille * sprite2->image->h);
			int u, v, v1 = 0;

			/* lock the video memory and copy the sprite bitmaps into cpu memory */
			// printf("SDL_MUSTLOCK(sprite1->sprite) : %d\n", SDL_MUSTLOCK(sprite1->sprite));
			SDL_LockSurface((sprite1->image));
			SDL_LockSurface((sprite2->image));
			bmp_it = bmp1;
			sprite_it = (Uint32 *)(sprite1->image->pixels) + sprite1->angle * sprite1->spritetaille;
			for (v = 0; v < sprite1->image->h; v++)
			{
				for (u = 0; u < sprite1->spritetaille; u++)
				{
					*bmp_it++ = *sprite_it++;
				}
				sprite_it += (sprite1->image->w - sprite1->spritetaille);
			}
			bmp_it = bmp2;
			sprite_it = (Uint32 *)(sprite2->image->pixels) + sprite2->angle * sprite2->spritetaille;
			for (v = 0; v < sprite2->image->h; v++)
			{
				for (u = 0; u < sprite2->spritetaille; u++)
				{
					*bmp_it++ = *sprite_it++;
				}
				sprite_it += (sprite2->image->w - sprite2->spritetaille);
			}
			SDL_UnlockSurface((sprite1->image));
			SDL_UnlockSurface((sprite2->image));

			bmp_it = bmp1;
			test = false;

			/* for each pixel p1 in bmp1, until test = true... */
			while (!test && v1 < sprite1->image->h)
			{
				int u1 = 0;
				while (!test && u1 < sprite1->spritetaille)
				{
					/* get the screen coordinates of pixel p1 */
					int screen_u = u1 + sprite1->pi.x;
					int screen_v = v1 + sprite1->pi.y;

					/* if the screen coordinates of p1 are inside the bb of sprite2... */
					if (screen_u >= sprite2->pi.x &&
						screen_u < sprite2->pi.x + sprite2->spritetaille &&
						screen_v >= sprite2->pi.y &&
						screen_v < sprite2->pi.y + sprite2->image->h)
					{
						Uint32 pixel1 = *bmp_it;
						int col1;
						Uint8 r, g, b;

						/* get the color col1 of p1*/
						SDL_GetRGB(pixel1, format, &r, &g, &b);
						col1 = SDL_MapRGB(format, r, g, b);

						/* if col1 is a NON transparent color... */
						if (col1 != sprite1->colorkey)
						{
							Uint32 pixel2;
							int col2;
							Uint8 r, g, b;
							int u2, v2;

							/* get the local coordinates of pixel p2 in sprite2 corresponding to the screen coordinates of p1 */
							u2 = screen_u - sprite2->pi.x;
							v2 = screen_v - sprite2->pi.y;

							/* get the color col2 of p2 */
							pixel2 = *(bmp2 + u2 + v2 * sprite2->spritetaille);
							SDL_GetRGB(pixel2, format, &r, &g, &b);
							col2 = SDL_MapRGB(format, r, g, b);

							/* if col2 is also a non transparent color, a collision occurs  */
							if (col2 != sprite2->colorkey)
							{
								test = true;
								// printf("COLLISION\n");
								// printf("screen_u : %d ---- screen_v : %d\n",screen_u,screen_v);
								*cu = screen_u;
								*cv = screen_v;
								//printf("Apres affectation\n");
							}
						}
					}
					u1++;
					bmp_it++;
				}
				v1++;
			}
			free(bmp1);
			free(bmp2);
		}

		return test;
}
