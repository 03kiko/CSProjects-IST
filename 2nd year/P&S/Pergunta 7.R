library(stats4)
a <- 4

neg_log_likelihood <- function(theta) {
  x <- c(4.37, 4.3, 5.15, 5.11, 5.15, 4.66, 6.15, 5.72, 5.87, 5.64, 4.05)
  -sum(log(theta*x^(-(theta + 1)) * a^theta))
}

mle_theta <- mle(neg_log_likelihood, start = list(theta = 3.6))

theta_mle <- coef(mle_theta)

p <- 0.75

estimate_quantile <- a *(1-p)^(-1/theta_mle)
  
true_theta = 3.6
true_quantile <- a*(1-p)^(-1/true_theta)

absolute_value <- abs(true_quantile - estimate_quantile)
print(round(absolute_value, 4))