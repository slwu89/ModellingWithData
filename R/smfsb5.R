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
  return(list("times"=tvec,"states"=xvec))
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
  .Call(C_ctmc,as.integer(n),Q,pi0,as.integer(seed))
  # return(stepfun(out$times[-1],out$states))
}


#' Simulate Immigration-Death Process (in R)
#' @examples
#' plot(imdeath(n=30))
#' @export
imdeath <- function(n=20,x0=0,lambda=1,mu=0.1){

  xvec= vector("numeric",n+1)
  tvec= vector("numeric",n)
  t=0
  x = x0
  xvec[1] <- x
  for(i in 1:n){
    t = t+rexp(1,lambda+x*mu)
    if(runif(1,0,1) < lambda/(lambda+x*mu)){
      x <- x+1
    } else {
      x <- x-1
    }
    xvec[i+1] <- x
    tvec[i] <- t
  }
  stepfun(tvec, xvec)
}

#' Simulate Immigration-Death Process (in C)
#' @examples
#' plot(imdeath_C(n=30,seed=42))
#' @useDynLib ModellingWithData C_imdeath
#' @export
imdeath_C <- function(n=20,x0=0,lambda=1,mu=0.1,seed){
  out <- .Call(C_imdeath,as.integer(n),as.integer(x0),lambda,mu,as.integer(seed))
  stepfun(out$tvec,out$xvec)
}

#' Simulate Diffusion via Euler-Maruyama Approximation (in R)
#' @examples
#' afun <- function(x,lambda,mu) {
#'  lambda-mu*x
#' }
#' bfun <- function(x,lambda,mu) {
#'  sqrt(lambda+mu*x)
#'  }
#'  plot(rdiff(afun,bfun,lambda=1,mu=0.1,t=30))
#' @export
rdiff <- function(afun, bfun, x0 = 0, t = 50, dt = 0.01, ...){
  n <- t/dt
  xvec<- vector("numeric",n)
  x <- x0
  sdt <- sqrt(dt)
  for(i in 1:n){
    t <- i*dt
    x <- x + afun(x,...)*dt + bfun(x,...)*rnorm(1,0,sdt)
    xvec[i] <- x
  }
  ts(xvec, deltat = dt)
}


#' Simulate Diffusion via Euler-Maruyama Approximation (in C)
#' @examples
#' afun <- function(x,lambda,mu) {
#'  lambda-mu*x
#' }
#' bfun <- function(x,lambda,mu) {
#'  sqrt(lambda+mu*x)
#'  }
#'  plot(rdiff_C(afun,bfun,seed = 42,lambda=1,mu=0.1))
#' @useDynLib ModellingWithData C_rdiff
#' @export
rdiff_C <- function(afun,bfun,x0=0,t=50,dt=0.01,seed,...){
  call <- match.call(expand.dots = FALSE)
  ts(.Call(C_rdiff,call,x0,t,dt,as.integer(seed),environment()),deltat = dt)
}
