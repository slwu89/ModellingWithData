################################################################################
#   Examples and exercises from Ch2
#   Sean Wu
################################################################################

#' @useDynLib ModellingWithData C_birthday
#' @export
birthday <- function(max){
  .Call(C_birthday,max)
}

#' @useDynLib ModellingWithData C_fibonacci
#' @export
fibonacci <- function(max){
  .Call(C_fibonacci,max)
}

#' @useDynLib ModellingWithData C_birthday_struct
#' @export
birthday_struct <- function(upto){
  .Call(C_birthday_struct,upto)
}

#' @useDynLib ModellingWithData C_triangular
#' @export
triangular <- function(inR){
  .Call(C_triangular,inR)
}

#' @useDynLib ModellingWithData C_find_next_triangular
#' @export
find_next_triangular <- function(iR){
  .Call(C_find_next_triangular,iR)
}

#' @useDynLib ModellingWithData C_find_prev_triangular
#' @export
find_prev_triangular <- function(iR){
  .Call(C_find_prev_triangular,iR)
}

#' @useDynLib ModellingWithData C_find_triplet
#' @export
find_triplet <- function(iR){
  .Call(C_find_triplet,iR)
}

#' @useDynLib ModellingWithData C_swap
#' @export
swap <- function(aR,bR){
  .Call(C_swap,aR,bR)
}
