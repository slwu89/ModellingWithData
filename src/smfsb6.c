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
  for(int i=0; i<p; i++){
    REAL(x)[i] = REAL(icR)[i];
    SET_REAL_ELT(xmat,0+i*n,REAL(x)[i]); /* set xmat */
  }
  t = 0.;

  /* numerical solution of ODEs */
  for(int j=1; j<n; j++){
    t += dt;

  }

  return xmat;
};

p= length(ic)
  n = t/dt
  xmat = matrix(0,ncol=p,nrow=n)
  x = ic
  t=0
xmat[1,] = x
  for (iin2:n){
    t = t + dt
    x = x + fun(x,t,...) * dt
    xmat[i,] = x
  }
  t s (xmat, start=0, deltat=dt)
    SEXP outObj = PROTECT(Rf_allocMatrix(INTSXP, periods, 2));
