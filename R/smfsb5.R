
#' Simulate DTMC (in R)
#' @examples
#' P=matrix(c(0.9 ,0.1 ,0.2 ,0.8) ,ncol=2,byrow=TRUE)
#' pi0=c(0.5 ,0.5)
#' samplepath=rfmc (200 ,P , pi0 )
#' plot(samplepath)
#' @export
rfmc <- function(n, P, pi0) {
  v= vector("numeric",n)
  r = length(pi0)
  v[1] = sample(r,1,prob=pi0)
  for (i in 2:n){
    v[i] = sample(r,1,prob=P[v[i-1],])
  }
  ts(v)
}

#' @useDynLib ModellingWithData C_rfmc
#' @export
rfmc_C <- function(n,P,pi0,seed){
  if(!is.matrix(P)){
    stop("P must be a numeric matrix")
  }
  if(!is.numeric(pi0)){
    stop("pi0 must be a numeric vector")
  }
  if(length(pi0) != nrow(P)){
    stop("pi0 must be length equal to number of rows of P")
  }
  if(nrow(P)!=ncol(P)){
    stop("P must be a square matrix")
  }
  ts(.Call(C_rfmc,as.integer(n),P,pi0,as.integer(seed)))
}
