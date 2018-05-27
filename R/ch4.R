################################################################################
#   Examples and exercises from Ch4
#   Sean Wu
################################################################################

#' @useDynLib ModellingWithData C_GSL_Mat_Vec
#' @export
GSL_Mat_Vec <- function(){
  .Call(C_GSL_Mat_Vec)
}
