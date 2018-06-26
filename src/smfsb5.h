/* ##################################################
#   Examples and exercises from SMfSB Ch5
#   Sean Wu
################################################## */

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

/* get GSL matrix from SEXP (borrowed from RcppGSL) */
gsl_matrix* SEXP_2_GSL_matrix(SEXP matR);

/* Simulate DTMC */
SEXP C_rfmc(SEXP n, SEXP P, SEXP pi0, SEXP seed);

/* Simulate AR(1) */
SEXP C_ar1(SEXP n, SEXP alpha, SEXP sigma, SEXP seed);

/* Simulate CTMC */
SEXP C_ctmc(SEXP n, SEXP Q, SEXP pi0, SEXP seed);

/* Simulate Diffusion Process via Euler-Maruyama Approximation */
SEXP C_rdiff(SEXP call, SEXP x0, SEXP t, SEXP dtR, SEXP seed, SEXP rho);
