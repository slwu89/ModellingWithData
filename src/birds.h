/* C libraries */
#include <apop.h>
#include <time.h>

/* R's C API */
#include <R.h>
#include <Rinternals.h>

/* bird object */
typedef struct {
    char type;
    int wealth;
    int id;
} bird;

/* functions for birds */
void play_pd_game(bird *row, bird *col); /* birds.c */
bird *new_chick(bird *parent); /* birds.c */
void birth_or_death(void *in, void *v); /* birds.c */
void startup(int initial_flock_size); /* birds.c */
void bird_plays(void *in, void *v); /* birds.c */

/* functions for flock management */
void flock_init();
void free_bird(bird*);
void add_to_flock(bird*);
void flock_plays();
int flock_size();
int flock_wealth();
void cull_flock();
double count(int);
bird * find_opponent(int);
void out(int i);
