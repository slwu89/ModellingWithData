/* ##################################################
#   Examples and exercises from SMfSB Ch5
#   Sean Wu
################################################## */

#include "smfsb5.h"

/* get GSL matrix from SEXP (borrowed from RcppGSL) */
gsl_matrix* SEXP_2_GSL_matrix(SEXP matR){

  /* extract the "dim" attribute */
  SEXP dim = getAttrib(matR,R_DimSymbol) ;
  int nrow = INTEGER(dim)[0];
  int ncol = INTEGER(dim)[1];

  /* extracting the pointer just once */
  double * p = REAL(matR);

  /* fill GSL matrix */
  gsl_matrix * m = gsl_matrix_alloc (nrow,ncol);

  for(int i=0; i<nrow; i++){
    for(int j=0; j<ncol; j++){
      gsl_matrix_set(m,i,j,p[i+nrow*j]);
    }
  };

  return m;
};

/* Simulate DTMC */
SEXP C_rfmc(SEXP n, SEXP P, SEXP pi0, SEXP seed){

  /* output object and a pointer to it */
  SEXP v = PROTECT(allocVector(INTSXP,asInteger(n)));
  int* pv = INTEGER(v);

  /* alloc rng */
  gsl_rng * rng = gsl_rng_alloc(gsl_rng_mt19937);
  gsl_rng_set(rng,asInteger(seed));

  /* setup lookup tables for random number generator */
  SEXP dim = getAttrib(P,R_DimSymbol) ;
  int nrow = INTEGER(dim)[0];
  int ncol = INTEGER(dim)[1];

  /* first element is pi0 */
  gsl_ran_discrete_t* lookups[nrow+1];
  lookups[0] = gsl_ran_discrete_preproc(ncol,REAL(pi0));

  /* set matrix lookups */
  double* p = REAL(P);
  for(int i=0; i<nrow; i++){
    double rowi[ncol];
    for(int j=0; j<ncol; j++){
      rowi[j] = p[i+nrow*j];
    }
    lookups[i+1] = gsl_ran_discrete_preproc(ncol,rowi);
  }

  /* sample initial state of DTMC */
  pv[0] = gsl_ran_discrete(rng,lookups[0]);

  /* sample trajectory of DTMC */
  for(int i=1; i<asInteger(n); i++){
    pv[i] = gsl_ran_discrete(rng,lookups[pv[i-1]+1]);
  }

  /* free memory */
  gsl_rng_free(rng);
  for(int i=0; i<(nrow+1); i++){
    gsl_ran_discrete_free(lookups[i]);
  }

  /* unprotect SEXPs and return to R */
  UNPROTECT(1);
  return v;
};
