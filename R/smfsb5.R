##################################################
#   Examples and exercises from SMfSB Ch5
#   Sean Wu
##################################################

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

#' Simulate DTMC (in C)
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

#' Simulate AR(1) (in C)
#' @useDynLib ModellingWithData C_ar1
#' @export
ar1_C <- function(n,alpha,sigma,seed){
  .Call(C_ar1,as.integer(n),alpha,sigma,as.integer(seed))
}

#' Simulate CTMC (in R)
#' @examples
#' Q = matrix(c(-0.5,0.5,1,-1),ncol=2,byrow=TRUE)
#' pi0 = c(1,0)
#' plot(rcfmc(20,Q,pi0))
#' @export
rcfmc <- function(n,Q,pi0) {
  xvec= vector("numeric",n+1)
  tvec= vector("numeric",n)
  r= length(pi0)
  x = sample(r,1,prob=pi0)
  t=0
  xvec[1] = x
  for (i in 1:n){
    t = t+rexp(1,-Q[x,x])
    weights = Q[x,]
    weights[x] = 0
    x = sample(r,1,prob=weights)
    xvec[i+1] = x
    tvec[i] = t
  }
  stepfun(tvec,xvec)
}

#' Simulate CTMC (in C)
#' @useDynLib ModellingWithData C_ctmc
#' @export
rcfmc_C <- function(n,Q,pi0,seed){
  if(!is.matrix(Q)){
    stop("Q must be a numeric matrix")
  }
  if(!is.numeric(pi0)){
    stop("pi0 must be a numeric vector")
  }
  if(length(pi0) != nrow(Q)){
    stop("pi0 must be length equal to number of rows of Q")
  }
  if(nrow(Q)!=ncol(Q)){
    stop("Q must be a square matrix")
  }
  out <- .Call(C_ctmc,as.integer(n),Q,pi0,as.integer(seed))
  return(stepfun(out$times[-1],out$states))
}
