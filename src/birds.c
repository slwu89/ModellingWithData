/* ##################################################
#   Birds ABM from ch6
#   Sean Wu
################################################## */

#include "birds.h"

gsl_rng *r;
int     id_count            = 0;
double resource = 2.0;
double cost = 2.01;

/* encounter between two birds */
void play_hd_game(bird *row, bird *col){
  if(row->type == 'd' && col->type == 'h'){
    col->wealth += resource;
  } else if(row->type == 'h' && col->type == 'd'){
    row->wealth += resource;
  } else if(row->type == 'd' && col->type == 'd'){
    col->wealth += resource/2;
    row->wealth += resource/2;
  } else { // hawk v hawk
    col->wealth += (resource-cost)/2;
    row->wealth += (resource-cost)/2;
  }
}

/* function to be mapped across the flock */
void bird_plays(void *in, void *dummy_param){
  bird *other;
  while(!(other = find_opponent(gsl_rng_uniform_int(r,id_count))) && (in != other));
  play_hd_game(in, other);
}

/* parent gives birth to a chick, or if NULL spawn a chick */
bird *new_chick(bird *parent){
  bird  *out    = malloc(sizeof(bird));
  if(parent){
    out->type  = parent->type;
  } else {
    if(gsl_rng_uniform(r) > 0.5){
      out->type  = 'd';
    } else {
      out->type  = 'h';
    }
  }
  out->wealth = 5 * gsl_rng_uniform(r);
  out->id     = id_count;
  id_count    ++;
  return out;
}

/* in: parent or NULL, t: total wealth of flock */
void birth_or_death(void *in, void *t){
  bird  *b              = in; /* cast void to bird */
  int   *total_wealth   = t;
  if(b->wealth*20./ *total_wealth >= gsl_rng_uniform(r)){
    add_to_flock(new_chick(b));
  }
  if (b->wealth*800./ *total_wealth <= gsl_rng_uniform(r)){
    free_bird(b);
  }
}

/* initialize a flock */
void startup(int initial_flock_size){
  flock_init();
  r   = apop_rng_alloc(time(NULL));
  for(int i=0; i< initial_flock_size; i++){
    add_to_flock(new_chick(NULL));
  }
}
