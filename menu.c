#include "menu.h"

int menu(SDL_Surface *src){
	SDL_Event event;
	int continuer=1;
	SDL_Rect pos;
	pos.x=0;
	pos.y=0;
	int cursor=0;

	SDL_Surface *thecursor;
	thecursor=SDL_LoadBMP("img/cursor.bmp");
	SDL_SetColorKey(thecursor, SDL_SRCCOLORKEY, SDL_MapRGB(thecursor->format, 255, 255, 255));
	SDL_Rect cursor_pos;
	cursor_pos.x=70;

	while(continuer){
		if(cursor>=2) cursor=2;
		if(cursor<=0) cursor=0;
		cursor_pos.y=370+cursor*70;
		SDL_BlitSurface(SDL_LoadBMP("img/title_screen2.bmp"), NULL, src, &pos); /* On efface l'écran */
		SDL_BlitSurface(thecursor, NULL, src, &cursor_pos);
		SDL_Flip(src);
		SDL_WaitEvent(&event);
		switch(event.type){
			case SDL_KEYDOWN:
				switch(event.key.keysym.sym){
					case SDLK_RETURN:
						continuer=0;
						break;
					case SDLK_UP:
						cursor--;
						break;
					case SDLK_DOWN:
						cursor++;
						break;
					default: break;
							
				}
		}
	}
	return cursor;
}

void credits(SDL_Surface *src){
	SDL_Event event;
	int continuer=1;
	SDL_Rect pos;
	pos.x=0;
	pos.y=0;

	SDL_Surface *thecursor;
	thecursor=SDL_LoadBMP("img/cursor.bmp");
	SDL_SetColorKey(thecursor, SDL_SRCCOLORKEY, SDL_MapRGB(thecursor->format, 255, 255, 255));
	SDL_Rect cursor_pos;
	cursor_pos.x=70;

	while(continuer){
		cursor_pos.y=597;
		SDL_BlitSurface(SDL_LoadBMP("img/credits.bmp"), NULL, src, &pos); /* On efface l'écran */
		SDL_BlitSurface(thecursor, NULL, src, &cursor_pos);
		SDL_Flip(src);
		SDL_WaitEvent(&event);
		switch(event.type){
			case SDL_KEYDOWN:
				switch(event.key.keysym.sym){
					case SDLK_RETURN:
						continuer=0;
						break;
					default: break;
							
				}
		}
	}
}
