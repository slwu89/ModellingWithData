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
SEXP C_birthday(SEXP maxR);

/* Fibonacci */
int fibonacci(int i);

SEXP C_fibonacci(SEXP maxR);

/* the birthday problem with stucts and functions (pg.35) */
