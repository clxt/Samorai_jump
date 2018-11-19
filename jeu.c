#include "jeu.h"

void UpdateEvents(input_t* in){
	SDL_Event event;
	while(SDL_PollEvent(&event)){
		switch (event.type){
			case SDL_KEYDOWN:
				in->key[event.key.keysym.sym]=1;
				break;
			case SDL_KEYUP:
				in->key[event.key.keysym.sym]=0;
				break;
			default:
				break;
		}
	}
}

platform_t* createPF(){
	platform_t* p=(platform_t*)malloc(sizeof(platform_t));
	p->type=rand()%3;
		if(p->type==UNIQUE) p->pf=SDL_LoadBMP("img/neige.bmp");
		else if(p->type==TRAMPOLINE) p->pf=SDL_LoadBMP("img/branche.bmp");
		else if(p->type==NORMALE) p->pf=SDL_LoadBMP("img/pierre.bmp");
	SDL_SetColorKey(p->pf, SDL_SRCCOLORKEY, SDL_MapRGB(p->pf->format, 255, 255, 255));
	p->pos.x=rand()%(LARGEURFENETRE-90);
	p->pos.y=0;
	p->speed=2; //rand()%2+1;
	p->touch=0; 
	p->next=NULL;
	return p;
}

character_t* createCharacter(){
	character_t* character=(character_t*)malloc(sizeof(character_t));
	character->sprite=SDL_LoadBMP("img/perso1a.bmp");
	SDL_SetColorKey(character->sprite, SDL_SRCCOLORKEY, SDL_MapRGB(character->sprite->format, 0, 255, 0));
	character->pos.x=LARGEURFENETRE/2.-50;
	character->pos.y=10;
	character->vx=VITESSEX;
	character->vy=100;
	character->t=0;
	character->masse=3;
	return character;
}

void jump(character_t* character, float f){
	character->t=-HAUTEURSAUT*f;
}

void moveCharacter(input_t* in, character_t* character){
	character->pos.y+=character->vy;
	if(in->key[SDLK_RIGHT])
		character->pos.x+=character->vx;
	if(in->key[SDLK_LEFT])
 		character->pos.x-=character->vx;

	if(character->pos.x>LARGEURFENETRE-character->pos.w) character->pos.x=LARGEURFENETRE-character->pos.w;
}

void fallPF(platform_t *lpf){
	platform_t *tmp=lpf;
	while(tmp){
		tmp->pos.y+=tmp->speed;
		tmp=tmp->next;
	}
}

platform_t* isOnPF(character_t* character,platform_t *lpf){
	platform_t *tmp=lpf, *is;
	if( character->t>0){
		while(tmp){
			if( (character->pos.y + character->pos.h) > (tmp->pos.y-10)
			 && (character->pos.y + character->pos.h) <= tmp->pos.y
			 && (character->pos.x + 3./10*character->pos.w) < (tmp->pos.x + tmp->pos.w)
			 && (character->pos.x + 3./4*character->pos.w) > tmp->pos.x ){
				is=tmp;
				if(tmp->type==UNIQUE) tmp->touch=1;
			}
			tmp=tmp->next;
		}}
	return is;
}

char pos_y_lt_HAUTEUR(const platform_t const *platform){
			return platform->pos.y < HAUTEURFENETRE;
}
char touch_pf(const platform_t const *platform){
			return platform->touch!=1;
}
char destroy_pf(const platform_t const *platform){
			return platform!=NULL;
}

platform_t *filter(char (*pred)(const platform_t const *), platform_t *current){
	platform_t *first=current, *prev=NULL, *next;
	while (current) {
		next=current->next;
		if (!(*pred)(current)) {
			//libérer
			SDL_FreeSurface(current->pf);
			free(current);

			if (prev)
				prev->next=next;
			else
				first=next;
		} else
			prev=current;

		current=next;
	}
	return first;
}

platform_t *chain(platform_t *lpf, platform_t *pf){
	pf->next=lpf;
	return pf;
}

void BlitCharacter(character_t* character, SDL_Surface* src){
	static int sprite=0;
	int DELTA=8;
	if(++sprite >= 4*DELTA) sprite=0;

	if(character->t < 30-HAUTEURSAUT){ // jusquà -40
		if(sprite < 1*DELTA) character->sprite=SDL_LoadBMP("img/perso1a.bmp");
		else if((sprite >= 1*DELTA && sprite < 2*DELTA) || sprite >= 3*DELTA)
			character->sprite=SDL_LoadBMP("img/perso1b.bmp");
		else character->sprite=SDL_LoadBMP("img/perso1c.bmp");
		
	}
	else if(character->t < 60 &&character->t >= -10){
		if(sprite < 1*DELTA) character->sprite=SDL_LoadBMP("img/perso3a.bmp");
		else if((sprite >= 1*DELTA && sprite < 2*DELTA) || sprite >= 3*DELTA)
			character->sprite=SDL_LoadBMP("img/perso3b.bmp");
		else character->sprite=SDL_LoadBMP("img/perso3c.bmp");
	}


	else{ // de -40 à -10 de 60 à 
		if(sprite < 1*DELTA) character->sprite=SDL_LoadBMP("img/perso2a.bmp");
		else if((sprite >= 1*DELTA && sprite < 2*DELTA) || sprite >= 3*DELTA)
			character->sprite=SDL_LoadBMP("img/perso2b.bmp");
		else character->sprite=SDL_LoadBMP("img/perso2c.bmp");
	}

	SDL_SetColorKey(character->sprite, SDL_SRCCOLORKEY, SDL_MapRGB(character->sprite->format, 255, 255, 255));
	SDL_BlitSurface(character->sprite, NULL, src, &(character->pos));

}

void BlitPlatform(platform_t *lpf, SDL_Surface* src){
	platform_t *tmp=lpf;
	while(tmp){
		SDL_BlitSurface(tmp->pf, NULL, src, &(tmp->pos));
		tmp=tmp->next;
	}
}

platform_t* init_PF(){
	platform_t *lpf=createPF();
	platform_t *lpf2=createPF();
	platform_t *lpf3=createPF();
	lpf->speed=1;
	lpf->pos.x=150;
	lpf->pos.y=HAUTEURFENETRE-320;
	lpf2->speed=1;
	lpf2->pos.x=100;
	lpf2->pos.y=HAUTEURFENETRE-220;
	lpf3->speed=1;
	lpf3->pos.x=200;
	lpf3->pos.y=HAUTEURFENETRE-420;
	lpf->next=lpf2;
	lpf2->next=lpf3;
	
	return lpf;
}

char lose(character_t* character){
	return character->pos.y>HAUTEURFENETRE;
}
