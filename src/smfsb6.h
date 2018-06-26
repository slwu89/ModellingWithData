/* ##################################################
#   Examples and exercises from SMfSB Ch6
#   Sean Wu
################################################## */

 /* C headers */
#include <stdio.h>
#include <math.h>

/* GSL headers */
#include <gsl/gsl_rng.h>
#include <gsl/gsl_randist.h>

 /* R's C API */
#include <R.h>
#include <Rinternals.h>

/* Euler scheme for ODEs */
SEXP C_simpleEuler(SEXP call, SEXP tR, SEXP dtR, SEXP icR, SEXP rho);
