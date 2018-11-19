#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_image.h>
#include <time.h>
#include <math.h>

#define	LARGEURFENETRE 320 
#define	HAUTEURFENETRE 680
#define	HAUTEURSAUT 70
#define	VITESSEX 3

/* Structures */
enum platformtype{NORMALE, TRAMPOLINE, UNIQUE};

typedef struct input{
	char key[SDLK_LAST];
} input_t;

typedef struct platform{
	enum platformtype type;
    SDL_Surface *pf;
	SDL_Rect pos;
	int speed;
	char touch;
	struct platform *next;
} platform_t;

typedef struct character{
    SDL_Surface *sprite;
	SDL_Rect pos;
	int vx;	
	int vy;
	int t;
	int masse;
} character_t;

/* Events */
void UpdateEvents(input_t *in);		// update events tab

/* Allocation & Memory */
platform_t* createPF();								// allocate a platform
character_t* createCharacter();						// allocate a character
platform_t* chain(platform_t* lpf, platform_t *pf); // chain a platform to platform list

/* Game */
platform_t *filter(char (*pred)(const platform_t const *), platform_t *current); // filter de platform list with a predicat
char pos_y_lt_HAUTEUR(const platform_t const *platform);	// free platform if too high
char touch_pf(const platform_t const *platform);			// change platform touch
char destroy_pf(const platform_t const *platform);			// free all list

platform_t *isOnPF(character_t* character,platform_t* lpf); // test if character is on a platform
void jump(character_t* character, float f);					// make character jump
void moveCharacter(input_t *in, character_t* character);	// update character position
void fallPF(platform_t* lpf);								// make plateform fall
char lose();												// test if lose
platform_t* init_PF();										// init platform

/* Blit functions */
void BlitCharacter(character_t* character, SDL_Surface* src);
void BlitPlatform(platform_t* lpf, SDL_Surface* src);
