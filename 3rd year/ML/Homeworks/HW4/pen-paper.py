import numpy as np
from scipy.stats import multivariate_normal

# Initial parameters
x = np.array([[1, 0], [0, 2], [3, -1]])  # Observations x1, x2, x3

mu1 = np.array([2, -1])  # Initial mean for cluster 1
mu2 = np.array([1, 1])   # Initial mean for cluster 2

cov1 = np.array([[4, 1], [1, 4]])  # Covariance for cluster 1
cov2 = np.array([[2, 0], [0, 2]])  # Covariance for cluster 2

pi1, pi2 = 0.5, 0.5  # Initial mixing coefficients

# Function to perform the E-step with detailed intermediary calculations
def e_step(x, mu1, mu2, cov1, cov2, pi1, pi2):
    # Compute Gaussian likelihoods normals(x_i, c_k) = N(x_i | mu_k, cov_k)
    normals1 = np.round(multivariate_normal.pdf(x, mean=mu1, cov=cov1), 3)
    normals2 = np.round(multivariate_normal.pdf(x, mean=mu2, cov=cov2), 3)

    # Calculate p(x_i, c_k) = pi_k * normals(x_i, c_k)
    px_c1 = np.round(pi1 * normals1, 3)
    px_c2 = np.round(pi2 * normals2, 3)

    # Calculate responsibilities gamma_{ki}
    gamma1 = np.round(px_c1 / (px_c1 + px_c2), 3)
    gamma2 = np.round(px_c2 / (px_c1 + px_c2), 3)
    
    # Print intermediary calculations
    for i in range(len(x)):
        print(f"Data point x_{i+1}: {x[i]}")
        print(f"normals(x_{i+1}, c1) = {normals1[i]}, normals(x_{i+1}, c2) = {normals2[i]}")
        print(f"p(x_{i+1}, c1) = {px_c1[i]}, p(x_{i+1}, c2) = {px_c2[i]}")
        print(f"gamma_{i+1}1 = {gamma1[i]}, gamma_{i+2}1 = {gamma2[i]}\n")

    return gamma1, gamma2

# Function to perform the M-step
def m_step(x, gamma1, gamma2):
    # Update mixing coefficients
    n1, n2 = np.sum(gamma1), np.sum(gamma2)
    pi1_new, pi2_new = np.round(n1 / len(x), 3), np.round(n2 / len(x), 3)
    
    # Update means
    mu1_new = np.sum(gamma1[:, np.newaxis] * x, axis=0) / n1
    mu2_new = np.sum(gamma2[:, np.newaxis] * x, axis=0) / n2
    mu1_new = np.round(mu1_new, 3)
    mu2_new = np.round(mu2_new, 3)
    
    # Update covariances
    cov1_new = np.dot((gamma1[:, np.newaxis] * (x - mu1_new)).T, (x - mu1_new)) / n1
    cov2_new = np.dot((gamma2[:, np.newaxis] * (x - mu2_new)).T, (x - mu2_new)) / n2
    cov1_new = np.round(cov1_new, 3)
    cov2_new = np.round(cov2_new, 3)
    
    return mu1_new, mu2_new, cov1_new, cov2_new, pi1_new, pi2_new

# EM algorithm: Two epochs
for epoch in range(2):
    print(f"Epoch {epoch + 1}:\n")
    
    # E-step with detailed intermediary calculations
    gamma1, gamma2 = e_step(x, mu1, mu2, cov1, cov2, pi1, pi2)
    
    # M-step
    mu1, mu2, cov1, cov2, pi1, pi2 = m_step(x, gamma1, gamma2)
    
    # Print updated parameters after M-step
    print(f"Updated parameters after epoch {epoch + 1}:")
    print(f"Mean 1: {mu1}")
    print(f"Mean 2: {mu2}")
    print(f"Covariance 1:\n{cov1}")
    print(f"Covariance 2:\n{cov2}")
    print(f"Mixing Coefficients: pi1 = {pi1}, pi2 = {pi2}\n")
    print("="*40 + "\n")