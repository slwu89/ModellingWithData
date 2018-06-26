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
  int t = asInteger(tR);
  double dt = asReal(dtR);

  /* initialize some variables we need */
  int p = length(icR);
  int n = (int)t/dt;
  SEXP xmat =

};

p= length(ic)
  n = t/dt
  xmat = matrix(0,ncol=p,nrow=n) x = ic
  t=0
xmat[1,] = x
  for (iin2:n){
    t = t + dt
    x = x + fun(x,t,...) * dt
    xmat[i,] = x
  }
  t s (xmat, start=0, deltat=dt)
