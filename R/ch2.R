################################################################################
#   Examples and exercises from Ch2
#   Sean Wu
################################################################################

#' @useDynLib ModellingWithData C_birthday
#' @export
birthday <- function(max){
  .Call(C_birthday,max)
}
