################################################################################
#   Examples and exercises from birds ABM
#   Sean Wu
################################################################################

#' @useDynLib ModellingWithData C_birds_tree
#' @export
birds_tree <- function(initial_popR,periodsR){
  .Call(C_birds_tree,initial_popR,periodsR)
}
