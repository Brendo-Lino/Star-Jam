#ifndef GAME_H
#define GAME_H

#include <stdio.h>
#include <time.h>
#include <math.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>

#include "structures.h"
#include "utils.h"
#include "planets.h"
#include "collisions.h"
#include "shots.h"
#include "entities.h"
#include "ship.h"
#include "backgrounds.h"
#include "stats.h"
#include "drops.h"
#include "motion.h"

#define UPS 100

/* Structural Variables */
extern int running;
extern int playing;
extern int selecting;
extern int waiting;
extern int lost;
extern int boss;
extern int op_shots;

extern ALLEGRO_DISPLAY *display;
extern ALLEGRO_EVENT_QUEUE *event_queue;
extern ALLEGRO_TIMER *timer;

/* Fonts */
extern ALLEGRO_FONT *size_64;
extern ALLEGRO_FONT *size_32;
extern ALLEGRO_FONT *size_24;
extern ALLEGRO_FONT *size_16;

/* Initializes allegro stuff */
int game_setup_allegro(void);

/* Loads the fonts */
void game_load_fonts(void);

/* Starts the game */
void game_start(void);

/* Stops the game */
void game_stop(void);

void game_lose(void);

/* Resets the game */
void game_reset(void);

/* Runs the game */
void game_run(void);

/* Loads the game */
void game_load(void);

/* Destroys the game */
void game_destroy(void);

/* Reset the timers */
int game_reset_timers(void);

/* Loads images and audios */
void game_load_assets(void);

/* Loads the backgrounds */
void game_load_backgrounds(void);

/* Updates the backgrounds */
void game_update_background(Background *bg);

/* Creates a marble of the given size */
void game_create_marble(int size);

/* Keyboard Inpus */
void game_process_keyboard(ALLEGRO_EVENT *event);

/* Adds score points */
void game_add_score(int points);

#endif