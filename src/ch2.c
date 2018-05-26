/* ##################################################
#   Examples and exercises from Ch2
#   Sean Wu
################################################## */

#include "ch2.h"


/* ##################################################
# the birthday problem
################################################## */

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


/* ##################################################
# Fibonacci
################################################## */

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


/* ##################################################
# The birthday problem with structs
################################################## */

void calculate_days(bday_struct days[],int upto){
  int ct;
  days[1].none_match = 1.;
  for (ct=2; ct<=upto; ct ++){
    days[ct].one_match = 1 - pow(364/365., ct-1);
    days[ct].none_match = days[ct-1].none_match * (1 - (ct-1)/365.);
  }
};

void print_days(bday_struct days[], int upto){
  int ct;
  printf("People\t Matches me\t Any match\n");
  for(ct=2; ct<=upto; ct++){
    printf("%i\t %.3f\t\t %.3f\n", ct, days[ct].one_match, 1-days[ct].none_match);
  }
};

SEXP C_birthday_struct(SEXP uptoR){
  int upto = asReal(uptoR);
  bday_struct days[upto+1];
  calculate_days(days,upto);
  print_days(days,upto);
  return R_NilValue;
};


/* ##################################################
# triangular numbers
################################################## */

int triangular(int i){
  int out = 0;
  for(int j = 0; j <= i; j++){
    out += j;
  }
  return out;
}

SEXP C_triangular(SEXP inR){
  int in = asReal(inR);
  int out = triangular(in);
  return ScalarReal(out);
};

int find_next_triangular(int i){
  int notFound = 1;
  int j = 0;
  int tri = 0;
  while(notFound){
    tri = triangular(j);
    if(tri > i){
      notFound = 0;
    }
    j++;
  }
  return tri;
};

int find_prev_triangular(int i){
  int notFound = 1;
  int j = i;
  int tri = 0;
  while(notFound){
    tri = triangular(j);
    if(tri < i){
      notFound = 0;
    }
    j--;
  }
  return tri;
};

SEXP C_find_next_triangular(SEXP iR){
  int i = asReal(iR);
  int out = find_next_triangular(i);
  return ScalarReal(out);
}

SEXP C_find_prev_triangular(SEXP iR){
  int i = asReal(iR);
  int out = find_prev_triangular(i);
  return ScalarReal(out);
}

void find_triplet(int in, int out[]){
  int max = find_prev_triangular(in); /* upper bound */
  out[0] = max;
  for(int i = max; i > 0; i--){
    for(int j = i; j > 0; j--){
      int mid = find_prev_triangular(j);
      for(int k = j; k > 0; k--){
        int low = find_prev_triangular(k);
        int sum = max+mid+low;
        if(sum == in){
          out[1]= mid;
          out[2] = low;
          break;
        }
      }
    }
  }
};

SEXP C_find_triplet(SEXP inR){

  /* find triplet */
  int in = asReal(inR);
  int out[] = {0,0,0};
  find_triplet(in,out);

  /* return stuff to R */
  SEXP outR = PROTECT(allocVector(INTSXP,3));
  int* pout = INTEGER(outR);
  pout[0] = out[0];
  pout[1] = out[1];
  pout[2] = out[2];
  UNPROTECT(1);
  return outR;
};
