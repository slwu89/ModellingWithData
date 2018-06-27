/* ##################################################
#   Examples and exercises from SMfSB Ch6
#   Sean Wu
################################################## */

#include "smfsb6.h"


 /* ##################################################
#   Euler scheme for ODEs
################################################## */
SEXP C_simpleEuler(SEXP call, SEXP tR, SEXP dtR, SEXP icR, SEXP rho){

  /* SEXP to C object */
  double t = asReal(tR);
  double dt = asReal(dtR);

  /* initialize some variables we need */
  int p = length(icR);
  int n = (int)t/dt;
  SEXP xmat = PROTECT(allocMatrix(REALSXP,n,p));

  /* set current state of system to initial conditions vector */
  SEXP x = PROTECT(allocVector(REALSXP,p));
  double* x_ptr = REAL(x);
  double* ic = REAL(icR);

  for(int i=0; i<p; i++){
    x_ptr[i] = ic[i];
    SET_REAL_ELT(xmat,0+i*n,x_ptr[i]); /* set xmat */
  }
  t = 0.;

  /* allows access to ... */
  SEXP args = CDR(call);

  /* get the symbol-value association between the 'fun' symbol */
  SEXP fun = install("fun");
  args = CDR(args);

  /* make the call  */
  SEXP tt = PROTECT(allocVector(REALSXP,1));
  double* ttp = REAL(tt);
  *ttp = t;
  SEXP funcall = PROTECT(LCONS(fun, LCONS(x, LCONS(tt, LCONS(R_DotsSymbol, R_NilValue)))));
  SEXP funout = PROTECT(allocVector(REALSXP,p));

  /* numerical solution of ODEs */
  for(int j=1; j<n; j++){
    t += dt;
    *ttp = t;
    funout = eval(funcall,rho);
    for(int i=0; i<p; i++){
      x_ptr[i] += REAL(funout)[i]*dt;
      SET_REAL_ELT(xmat,j+i*n,x_ptr[i]); /* set xmat */
    }
  }

  UNPROTECT(5);
  return xmat;
};


/* ##################################################
#   Gillespie algorithm for Stochastic Petri Net (SPN)
################################################## */
SEXP C_gillespie(SEXP call, SEXP N, SEXP S, SEXP n, SEXP seed, SEXP rho){

  /* alloc rng */
  gsl_rng * rng = gsl_rng_alloc(gsl_rng_mt19937);
  gsl_rng_set(rng,asInteger(seed));

  /* initialize variables */
  SEXP tt = Rf_ScalarReal(0.); /* system time */
  double* tt_p = REAL(tt);

  SEXP x = PROTECT(Rf_duplicate(getListElement(N,"M"))); /* current marking */
  int* x_p = INTEGER(x); /* pointer to current marking */

  int* S_p = INTEGER(S);
  int u = nrows(S); /* number of species */
  int v = ncols(S); /* number of reactions */

  /* outputs */
  SEXP tvec = PROTECT(allocVector(REALSXP,asInteger(n)));
  double* tvec_p = REAL(tvec);

  int nrow = asInteger(n)+1;
  SEXP xmat = PROTECT(allocMatrix(INTSXP,nrow,u));
  int* xmat_p = INTEGER(xmat);

  /* set initial marking of net */
  for(int k=0; k<u; k++){
    xmat_p[0 + nrow*k] = x_p[k];
  }

  SEXP hFun = PROTECT(getListElement(N,"h")); /* function to compute reaction propensities */
  SEXP hCall = PROTECT(LCONS(hFun, LCONS(x, LCONS(tt, LCONS(R_DotsSymbol, R_NilValue)))));
  SEXP h = PROTECT(allocVector(REALSXP,v)); /* vector of propensities */

  gsl_ran_discrete_t* v_table; /* lookup table for determining which reaction happened (PMF) */

  int zeros = 0;

  /* simulate SPN */
  for(int i=0; i<asInteger(n); i++){

    /* check if any species are exhausted */
    for(int k=0; k<u; k++){
      if(x_p[k]==0){
        zeros = 1;
      }
    }
    if(zeros){
      break;
    }

    /* evaluate reaction propensities */
    h = eval(hCall,rho);
    double* h_p = REAL(h);

    /* time until next reaction */
    double Lambda = 0.;
    for(int j=0; j<v; j++){
      Lambda += h_p[j];
    }

    /* advance time */
    *tt_p += gsl_ran_exponential(rng,Lambda);

    /* multinomial sampling to determine the reaction that took place */
    v_table = gsl_ran_discrete_preproc(v,REAL(h));
    int v_rxn = gsl_ran_discrete(rng,v_table);

    /* update current marking */
    for(int k=0; k<u; k++){
      x_p[k] += S_p[k + u*v_rxn];
    }

    /* record time and marking */
    tvec_p[i] = *tt_p;
    for(int k=0; k<u; k++){
      xmat_p[(i+1) + nrow*k] = x_p[k];
    }
  }

  /* free rng */
  gsl_rng_free(rng);
  gsl_ran_discrete_free(v_table);

  /* return output */
  SEXP names = PROTECT(Rf_allocVector(STRSXP,2));
  SET_STRING_ELT(names,0,Rf_mkChar("t"));
  SET_STRING_ELT(names,1,Rf_mkChar("x"));

  SEXP out = PROTECT(allocVector(VECSXP, 2));
  Rf_namesgets(out,names);

  SET_VECTOR_ELT(out,0,tvec);
  SET_VECTOR_ELT(out,1,xmat);

  UNPROTECT(8);
  return out;
};
