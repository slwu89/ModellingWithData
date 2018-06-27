#ifndef UTILITIES_H
#define UTILITIES_H

#include <R.h>
#include <Rinternals.h>
#include <R_ext/Rdynload.h>
#include <math.h>
#include <gsl/gsl_matrix.h>

/* get named element of a VECSXP */
SEXP getListElement(SEXP list, const char *str);

/* get GSL matrix from SEXP (borrowed from RcppGSL) */
gsl_matrix* SEXP_2_GSL_matrix(SEXP matR);

#endif
