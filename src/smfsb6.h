/* ##################################################
#   Examples and exercises from SMfSB Ch6
#   Sean Wu
################################################## */

#ifndef SMFSB6_H
#define SMFSB6_H

 /* C headers */
#include <stdio.h>
#include <math.h>

/* GSL headers */
#include <gsl/gsl_rng.h>
#include <gsl/gsl_randist.h>

 /* R's C API */
#include <R.h>
#include <Rinternals.h>

/* others */
#include "utilities.h"

/* Euler scheme for ODEs */
SEXP C_simpleEuler(SEXP call, SEXP tR, SEXP dtR, SEXP icR, SEXP rho);

/* Gillespie algorithm for Stochastic Petri Net (SPN) */
SEXP C_gillespie(SEXP call, SEXP N, SEXP S, SEXP n, SEXP seed, SEXP rho);

#endif
