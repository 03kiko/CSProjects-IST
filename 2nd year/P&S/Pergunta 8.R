library(pracma)

seed <- 1592
n <- 12
x <- c(31.8,31.7,35.2,37.1,31.7,36.1,36.3,33.2,34.3,37.5,30.4,34.6,32.4,31.7,30.2,34.3,35.6,34.9,38.9)

set.seed(seed)
n_observações <- sample(x, n)
s2 <- var(n_observações)

gamma <- 0.96
a <- qchisq((1-gamma)/2, df = n - 1 )
b <- qchisq((1+gamma)/2, df = n-1 )

old_ic <- c( ((n-1)*s2)/b, ((n-1)*s2)/a)

find_cd <- function(cd) {
  c <- cd[1]
  d <- cd[2]
  eq1 <- pchisq(d ,  df = n - 1) - pchisq(c, df = n-1) - gamma
  eq2 <- dchisq(d, df = n + 3) - dchisq(c, df= n + 3)
  return(c(eq1, eq2))
}

cd_sol <- fsolve(find_cd, c(a, b))
c <- cd_sol$x[1]
d <- cd_sol$x[2]

new_ic <- c(((n-1)*s2 )/d, ((n-1)*s2 )/c)

old_amplitude <- old_ic[2] - old_ic[1]
new_amplitude <- new_ic[2] - new_ic[1]

difference <- old_amplitude - new_amplitude
print(round(difference, 3) )



