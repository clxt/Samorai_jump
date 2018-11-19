#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <SDL.h>
#include <SDL_ttf.h>
#include <time.h>
#include <math.h>
#include "jeu.h"
#include "menu.h"

int main(int argc, char *argv[]){
	srand(time(NULL));

	/* Vars */
	int apparition=0, difficulty=0, aleatoire=50, score=0, choix;
	character_t *sam=NULL; 
	platform_t *lpf=NULL, *on=NULL;
	SDL_Surface *ecran=NULL;
	TTF_Font *police=NULL;
	input_t in;
	memset(&in,0,sizeof(in));

	/* SDL & TTF */
	SDL_Init(SDL_INIT_VIDEO);
	ecran = SDL_SetVideoMode(LARGEURFENETRE, HAUTEURFENETRE, 32, SDL_HWSURFACE | SDL_DOUBLEBUF);
	SDL_WM_SetCaption("Gestion des événements en SDL", NULL);

	TTF_Init();
	police = TTF_OpenFont("ttf/go3v2.ttf",25);
	SDL_Color couleurNoire = {0, 0, 0};
	SDL_Rect text_pos;

	text_pos.x=1;
	text_pos.y=0;
	char text_score[12];
	int continuer=1;

	sam=createCharacter();
	while(continuer){
		choix=menu(ecran);

		if(choix==0){

			apparition=0;
			difficulty=0;
			aleatoire=50;
			score=0;
			sam->pos.y=20;
			sam->pos.x=20;
			sam->t=0;
			memset(&in,0,sizeof(in));

			lpf=init_PF();

			/* Game Loop */
			while(!in.key[SDLK_ESCAPE] && !lose(sam)){
				UpdateEvents(&in);
				SDL_Delay(15);

				sprintf(text_score,"Score : %d", score++);

				fallPF(lpf);
				on=isOnPF(sam,lpf);

				if(on){
					if(on->type == TRAMPOLINE){
						jump(sam,1.4);
						on->speed=3;
						on->pf=SDL_LoadBMP("img/branche_touch.bmp");
						SDL_SetColorKey(on->pf, SDL_SRCCOLORKEY, SDL_MapRGB(on->pf->format, 255, 255, 255));
					}
					else jump(sam,1);
				}
				else{
					sam->vy=.1*sam->t;
					(sam->t)+=2;
				}

				moveCharacter(&in, sam);
				if(++apparition > aleatoire){
					apparition=0;
					lpf=chain(lpf,createPF());
					if(++difficulty > 60 ) difficulty=60;
					aleatoire=rand()%20+20+difficulty;
				}

				// Update platform list
				lpf=filter(pos_y_lt_HAUTEUR, lpf);
				lpf=filter(touch_pf, lpf);

				// Blit functions
				SDL_FillRect(ecran, NULL, SDL_MapRGB(ecran->format, 158, 158, 158)); /* On efface l'écran */
				BlitPlatform(lpf, ecran);
				BlitCharacter(sam, ecran);
				SDL_BlitSurface(TTF_RenderText_Blended(police, text_score, couleurNoire),NULL,ecran,&text_pos);
				SDL_Flip(ecran);
			}
			lpf=filter(destroy_pf,lpf);
		}
		else if(choix==1)
			credits(ecran);
		else
			continuer=0;
	}
	TTF_CloseFont(police);
	TTF_Quit();

	SDL_FreeSurface(ecran);
	SDL_Quit();

	return EXIT_SUCCESS;
}
