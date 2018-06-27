/* ##################################################
#   Examples and exercises from Ch2
#   Sean Wu
################################################## */

#ifndef MODWITHDATA4_H
#define MODWITHDATA4_H

/* C headers */
#include <stdio.h>
#include <math.h>

/* R's C API */
#include <R.h>
#include <Rinternals.h>

/* ##################################################
# the birthday problem
################################################## */

SEXP C_birthday(SEXP maxR);

/* ##################################################
# Fibonacci
################################################## */
int fibonacci(int i);

SEXP C_fibonacci(SEXP maxR);

/* ##################################################
# the birthday problem with stucts
################################################## */
typedef struct {
  double one_match;
  double none_match;
} bday_struct;

void calculate_days(bday_struct days[],int upto);

void print_days(bday_struct days[], int upto);

SEXP C_birthday_struct(SEXP uptoR);

/* ##################################################
# triangular numbers
################################################## */

int triangular(int i);

SEXP C_triangular(SEXP inR);

int find_next_triangular(int i);

int find_prev_triangular(int i);

SEXP C_find_next_triangular(SEXP iR);

SEXP C_find_prev_triangular(SEXP iR);

void find_triplet(int in, int out[]);

SEXP C_find_triplet(SEXP inR);

/* ##################################################
# swap ints
################################################## */

void swap(int* a, int* b);

SEXP C_swap(SEXP aR, SEXP bR);

#endif
