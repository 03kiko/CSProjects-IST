n <- 4
seed <- 2126
r <- 150
m <- 130
value <- -0.9

set.seed(seed)

proportion <- numeric(r)
for (i in 1:r) {
  T_sample <- numeric(m)
  samples_lt_value <- 0
  for (j in 1:m) {
    Z <- rnorm(n+1)
    T_sample[j] <- sqrt(n) * Z[1]/sqrt(sum(Z[-1]^2))
    if(T_sample[j] <= value) {
      samples_lt_value <- samples_lt_value + 1
    }
  }
  proportion[i] <- samples_lt_value / m
}

aprox <-mean(proportion)
direct_prob <- pt(-0.9, n)

res <- round(abs(aprox - direct_prob)* 100, 5)

print(res)


