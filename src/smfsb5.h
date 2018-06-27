/* ##################################################
#   Examples and exercises from SMfSB Ch5
#   Sean Wu
################################################## */

#ifndef SMFSB5_H
#define SMFSB5_H

 /* C headers */
#include <stdio.h>
#include <math.h>

/* GSL headers */
#include <gsl/gsl_rng.h>
#include <gsl/gsl_matrix.h>
#include <gsl/gsl_randist.h>

 /* R's C API */
#include <R.h>
#include <Rinternals.h>

/* Simulate DTMC */
SEXP C_rfmc(SEXP n, SEXP P, SEXP pi0, SEXP seed);

/* Simulate AR(1) */
SEXP C_ar1(SEXP n, SEXP alpha, SEXP sigma, SEXP seed);

/* Simulate CTMC */
SEXP C_ctmc(SEXP n, SEXP Q, SEXP pi0, SEXP seed);

/* Simulate Immigration-Death Process */
SEXP C_imdeath(SEXP nR, SEXP x0R, SEXP lambdaR, SEXP muR, SEXP seed);

/* Simulate Diffusion Process via Euler-Maruyama Approximation */
SEXP C_rdiff(SEXP call, SEXP x0, SEXP t, SEXP dtR, SEXP seed, SEXP rho);

#endif
