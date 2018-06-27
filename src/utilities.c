#include "utilities.h"

/* get an element of a list, return null if it doesn't exist */
SEXP getListElement(SEXP list, const char *str) {
  SEXP elmt = R_NilValue, names = getAttrib(list, R_NamesSymbol);
  int i;
  for (i = 0; i < length(list); i++)
    if (strcmp(CHAR(STRING_ELT(names, i)), str) == 0) {
      elmt = VECTOR_ELT(list, i);
      break;
    }

    if (elmt == R_NilValue)
      return NULL;
    else
      return elmt;
}

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
