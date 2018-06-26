/* ##################################################
#   Examples and exercises from SMfSB Ch5
#   Sean Wu
################################################## */

#include "smfsb5.h"


/* ##################################################
#   get GSL matrix from SEXP (borrowed from RcppGSL)
################################################## */
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


/* ##################################################
#   Simulate DTMC
################################################## */
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


/* ##################################################
#   Simulate AR(1)
################################################## */
SEXP C_ar1(SEXP n, SEXP alpha, SEXP sigma, SEXP seed){

  /* alloc rng */
  gsl_rng * rng = gsl_rng_alloc(gsl_rng_mt19937);
  gsl_rng_set(rng,asInteger(seed));

  /* output object and a pointer to it */
  SEXP v = PROTECT(allocVector(REALSXP,asInteger(n)));
  double* pv = REAL(v);

  /* simulate trajectory */
  pv[0] = 0.;
  for(int i=1; i<asInteger(n); i++){
    pv[i] = asReal(alpha)*pv[i-1] + gsl_ran_gaussian(rng,asReal(sigma));
  }

  /* free memory */
  gsl_rng_free(rng);

  /* unprotect SEXPs and return to R */
  UNPROTECT(1);
  return v;
};



/* ##################################################
#   Simulate CTMC
################################################## */
 SEXP C_ctmc(SEXP n, SEXP Q, SEXP pi0, SEXP seed){

  /* output objects and pointers to them */
  SEXP xvec = PROTECT(allocVector(INTSXP,asInteger(n)));
  int* xvec_p = INTEGER(xvec);

  SEXP tvec = PROTECT(allocVector(REALSXP,asInteger(n)));
  double* tvec_p = REAL(tvec);

  /* alloc rng */
  gsl_rng * rng = gsl_rng_alloc(gsl_rng_mt19937);
  gsl_rng_set(rng,asInteger(seed));

  /* set lookups */
  int nStates = length(pi0);
  gsl_ran_discrete_t* lookups[nStates];
  double* q = REAL(Q);
  for(int i=0; i<nStates; i++){
    double rowi[nStates];
    for(int j=0; j<nStates; j++){
      rowi[j] = q[i+nStates*j];
    }
    rowi[i] = 0.;
    lookups[i] = gsl_ran_discrete_preproc(nStates,rowi);
  }

  /* sample initial state of CTMC */
  gsl_ran_discrete_t* pi0_l;
  pi0_l = gsl_ran_discrete_preproc(nStates,REAL(pi0));
  xvec_p[0] = gsl_ran_discrete(rng,pi0_l);
  double t = 0.;
  tvec_p[0] = t;

  /* sample trajectory of CTMC */
  for(int i=1; i<asInteger(n); i++){
    t += gsl_ran_exponential(rng,-q[xvec_p[i-1]+nStates*xvec_p[i-1]]);
    xvec_p[i] = gsl_ran_discrete(rng,lookups[xvec_p[i-1]]);
    tvec_p[i] = t;
  }

  /* free memory */
  gsl_rng_free(rng);
  gsl_ran_discrete_free(pi0_l);
  for(int i=0; i<nStates; i++){
    gsl_ran_discrete_free(lookups[i]);
  }

  /* return output */
  SEXP names = PROTECT(Rf_allocVector(STRSXP,2));
  SET_STRING_ELT(names,0,Rf_mkChar("states"));
  SET_STRING_ELT(names,1,Rf_mkChar("times"));

  SEXP out = PROTECT(allocVector(VECSXP, 2));
  Rf_namesgets(out,names);

  SET_VECTOR_ELT(out,0,xvec);
  SET_VECTOR_ELT(out,1,tvec);

  UNPROTECT(4);
  return out;
 };


 SEXP C_rdiff(SEXP call, SEXP x0, SEXP t, SEXP dtR, SEXP seed, SEXP rho){

   /* alloc rng */
   gsl_rng * rng = gsl_rng_alloc(gsl_rng_mt19937);
   gsl_rng_set(rng,asInteger(seed));

   double dt = asReal(dtR); /* size of grid */
   int n = (int)(asReal(t)/dt); /* total number of steps */
   SEXP xvec = PROTECT(allocVector(REALSXP,n)); /* output vector */
   double* xvec_p = REAL(xvec); /* pointer to output */
   double x = asReal(x0);/* current value of process */
   xvec_p[0] = x;
   double sdt = sqrt(dt); /* standard deviation of gaussian white noise (derivative of Brownian motion) */

   /* allows access to ... */
   SEXP args = CDR(call);

   /* get the symbol-value association between the 'afun' and 'bfun' symbols */
   SEXP afun = install("afun");
   args = CDR(args);

   SEXP bfun = install("bfun");
   args = CDR(args);

   /* grab the 'x' argument for the afun and bfun we want to call */
   SEXP a1 = PROTECT(allocVector(REALSXP,1));
   SEXP b1 = PROTECT(allocVector(REALSXP,1));
   REAL(a1)[0] = x;
   REAL(b1)[0] = x;

   /* make the call  */
   SEXP a1call = PROTECT(LCONS(afun, LCONS(a1, LCONS(R_DotsSymbol, R_NilValue))));
   SEXP b1call = PROTECT(LCONS(bfun, LCONS(b1, LCONS(R_DotsSymbol, R_NilValue))));

   double tt;
   for(int i=1;i<=n;i++){
     tt = i*dt; /* time on the grid of points */
     REAL(a1)[0] = x;
     REAL(b1)[0] = x;
     SEXP aRes = R_forceAndCall(a1call, 1, rho);
     SEXP bRes = R_forceAndCall(b1call, 1, rho);
     x += asReal(aRes)*dt + asReal(bRes)*gsl_ran_gaussian(rng,sdt);
     xvec_p[i] = x;
   }

   gsl_rng_free(rng);

   UNPROTECT(5);

   return xvec;
 }
