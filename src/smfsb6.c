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
