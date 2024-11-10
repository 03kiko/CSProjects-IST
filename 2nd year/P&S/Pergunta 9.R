set.seed(2822)
lambda_0 <- 2.90
lambda_1 <- 3.15
n <- 100
m <- 5000
k <- 3.234

erro_tipo_I <- 0
erro_tipo_II <- 0

for(i in 1:m) {
  h0 <- rpois(n, lambda_0)
  h1 <- rpois(n, lambda_1)
  
  sum_h0 <- 0
  sum_h1 <- 0
  for(j in 1:n) {
    sum_h0 <- sum_h0 + h0[j]
    sum_h1 <- sum_h1 + h1[j]
    
  }
  media_h0 <- sum_h0/n
  media_h1 <- sum_h1/n
  
  if(media_h0 > k) {
    erro_tipo_I <- erro_tipo_I + 1
  }
  if(media_h1 <= k) {
    erro_tipo_II <- erro_tipo_II + 1
  }
}

quociente <- erro_tipo_II / erro_tipo_I
print(quociente)
