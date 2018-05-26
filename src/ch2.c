/* ##################################################
#   Examples and exercises from Ch2
#   Sean Wu
################################################## */

#include "ch2.h"

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

  /* names of output */
  SEXP names = PROTECT(allocVector(STRSXP,2));
  SET_STRING_ELT(names,0,mkChar("fibonacci sequence"));
  SET_STRING_ELT(names,1,mkChar("ratio of [n]/[n-1]"));

  /* return us a list */
  SEXP out = PROTECT(allocVector(VECSXP, 2));
  namesgets(out,names);

  /* elements of list */
  SEXP seq = PROTECT(allocVector(REALSXP,max));
  SEXP ratio = PROTECT(allocVector(REALSXP,max));

  /* pointers to the elements */
  double* pseq = REAL(seq);
  double* pratio = REAL(ratio);

  /* avoid division by 0 */
  pratio[0] = 0.;
  pratio[1] = 1.;

  /* actual calculation */
  for(int i=0; i<max; i++){
    pseq[i] = fibonacci(i);
    if(i>1){
      pratio[i] = pseq[i]/pseq[i-1];
    }
  }

  /* set return list and return */
  SET_VECTOR_ELT(out,0,seq);
  SET_VECTOR_ELT(out,1,ratio);

  UNPROTECT(4);
  return out;
}
