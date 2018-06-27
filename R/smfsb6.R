##################################################
#   Examples and exercises from SMfSB Ch6
#   Sean Wu
##################################################

#' Euler Scheme for Numerical Integration of ODEs (in R)
#' @examples
#' lv <- function(x,t,k=c(k1=1,k2=0.1,k3=0.1)) {
#'   with(as.list(c(x,k)),{
#'    c( k1*x1 - k2*x1*x2 ,
#'      k2*x1*x2 - k3*x2 )
#'    })
#'  }
#' plot(simpleEuler(t=100,fun=lv,ic=c(x1=4,x2=10)))
#' @export
simpleEuler <- function(t=50,dt=0.001,fun,ic,...) {
  p= length(ic)
  n = t/dt
  xmat = matrix(0,ncol=p,nrow=n)
  x = ic
  t=0
  xmat[1,] = x
  for(i in 2:n){
    t = t + dt
    x = x + fun(x,t,...) * dt
    xmat[i,] = x
  }
  ts(xmat, start=0, deltat=dt)
}

#' Euler Scheme for Numerical Integration of ODEs (in C)
#' @examples
#' lv <- function(x,t,k=c(k1=1,k2=0.1,k3=0.1)) {
#'   with(as.list(c(x,k)),{
#'     c( k1*x[1] - k2*x[1]*x[2] ,
#'        k2*x[1]*x[2] - k3*x[2] )
#'    })
#'  }
#' plot(simpleEuler_C(t=100,fun=lv,ic=c(4,10)))
#' @useDynLib ModellingWithData C_simpleEuler
#' @export
simpleEuler_C <- function(t=50,dt=0.001,fun,ic,...){
  call <- match.call(expand.dots = FALSE)
  ts(.Call(C_simpleEuler,call,t,dt,ic,environment()),start = 0,deltat = dt)
}

#' Gillespie algorithm for Stochastic Petri Net (SPN) (in C)
#' @useDynLib ModellingWithData C_gillespie
#' @examples
#' N = list()
#' N$M=c(x1=50,x2=100)
#' N$Pre=matrix(c(1,0,1,1,0,1),ncol=2,byrow=TRUE)
#' N$Post=matrix(c(2,0,0,2,0,0),ncol=2,byrow=TRUE)
#' N$h=function(x,t,th=c(th1=1,th2=0.005,th3=0.6)){
#'   with(as.list(th),{
#'     return(c(th1*x[1], th2*x[1]*x[2], th3*x[2] ))
#'   })
#' }
#'
#' out <- gillespie_C(N = N,n = 1e4,seed = 1654)
#'
#' plot(out$t,out$x[,1][-1])
#' plot(out$x,type="l")
#' @export
gillespie_C <- function(N,n,seed,...){
  call <- match.call(expand.dots = FALSE)
  S <- t(N$Post - N$Pre)
  storage.mode(S) = "integer"
  storage.mode(N$M) = "integer"
  .Call(C_gillespie,call,N,S,as.integer(n),as.integer(seed),new.env())
}
