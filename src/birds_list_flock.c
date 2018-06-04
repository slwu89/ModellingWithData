// /* ##################################################
// #   Birds ABM from ch6
// #   doubly linked-list implementation
// #   Sean Wu
// ################################################## */
//
// #include "birds.h"
// #include <glib.h>
//
// /* any functions not declared in birds.h should be declared static here
//  * because they are specific to the linked-list implementation */
//
// /* local stuff we need here */
// static GList* flock;
// static int hawks, doves;
//
// void flock_plays(){
//   g_list_foreach(flock, bird_plays, NULL);
// }
//
// void add_to_flock(bird* b){
//   flock = g_list_prepend(flock, b);
// }
//
// void free_bird(bird* b){
//   flock   = g_list_remove(flock, b);
//   free(b);
// }
//
// bird* find_opponent(int n){
//   return g_list_nth_data(flock, n);
// }
//
// static void wealth_foreach(void *in, void *total){
//   bird* a_bird = in; /* void to pointer to bird */
//   int* total_int = total; /* void to pointer to int */
//   *total += a_bird->wealth;
// }
//
// int flock_wealth(){
//   int total = 0;
//   g_list_foreach(flock, wealth_foreach, &total);
//   return total;
// }
//
// int flock_size(){
//   return g_list_length(flock);
// }
//
// static void bird_count(void *in, void *v){
//   bird *b = in;
//   if(b->type == 'h'){
//     hawks++;
//   } else {
//     doves++;
//   }
// }
//
// double count(int period){
//   int total_wealth = flock_wealth();
//   hawks   = doves   = 0;
//   g_list_foreach(flock, birth_or_death, &total_wealth);
//   g_list_foreach(flock, bird_count, NULL);
//   printf("%i\t%i\t%i\n", period, hawks, doves);
//   return (doves+0.0)/hawks;
// }
//
// void flock_init(){
//   flock  = NULL;
// }
//
// void run_birds(int initial_pop, int periods, SEXP outObj){
//   startup(initial_pop);
//   for(int i=0; i< periods; i++){
//     flock_plays();
//     count(i);
//     SET_INTEGER_ELT(outObj,  i + 0 * periods, hawks);
//     SET_INTEGER_ELT(outObj,  i + 1 * periods, doves);
//   }
// }
//
// // SEXP C_birds_tree(SEXP initial_popR, SEXP periodsR, SEXP resourceR, SEXP costR){
// //
// //   /* parameters for simulation */
// //   resource = asReal(resourceR);
// //   cost = asReal(costR);
// //
// //   /* R side output */
// //   int initial_pop = asReal(initial_popR);
// //   int periods = asReal(periodsR);
// //   SEXP outObj = PROTECT(Rf_allocMatrix(INTSXP, periods, 2));
// //
// //   /* run simulation */
// //   run_birds(initial_pop,periods,outObj);
// //
// //   /* return to R */
// //   UNPROTECT(1);
// //   return outObj;
// // };
