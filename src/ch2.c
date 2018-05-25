/* ##################################################
#   Examples and exercises from Ch2
#   Sean Wu
################################################## */

/* C headers */
#include <stdio.h>
#include <math.h>

/* R's C API */
#include <R.h>
#include <Rinternals.h>

/* the birthday problem */
SEXP C_birthday(SEXP maxR){
  int max = asReal(maxR);
  double no_match = 1;
  double matches_me;
  int ct;
  printf("People\t Matches me\t Any match\n");
  for (ct=2; ct<=max; ct++){
    matches_me = 1 - pow(364/365., ct - 1);
    no_match *= (1 - (ct-1)/365.);
    printf("%i\t %.3f\t\t %.3f\n", ct, matches_me, (1-no_match));
  }
  return R_NilValue;
}

/* Fibonacci */
int fibonacci(int i){
  int ans;
  if(i == 0 || i == 1){
    ans = i;
  } else {
    ans = (fibonacci(i-1) + fibonacci(i-2));
  }
  return ans;
}

SEXP C_fibonacci(SEXP maxR){
  int max = asReal(maxR);
  SEXP out = PROTECT(allocVector(REALSXP,max));
  double* pout = REAL(out);
  for(int i=0; i<max; i++){
    pout[i] = fibonacci(i);
  }

  UNPROTECT(1);
  return out;
}
