lambda <- 1/4
duração_total <- 126
seed <- 1973
n_amostras <- 1000
size_amostras <- 40
taxa <- 1/4

set.seed(seed)

count <- 0
for(i in 1:n_amostras) {
  s40 <- rexp(size_amostras, lambda)
  Y <- sum(s40)
  if(Y > duração_total) {
    count <- count + 1
  }
}
proportion <- count / n_amostras

direct_fiability <-  1 - pgamma(duração_total, shape= size_amostras, rate = taxa)

res <- round(abs(proportion - direct_fiability) * 100, 4)
print(res)