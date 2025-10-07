import matplotlib.pyplot as plt
import numpy as np

# Data
N = np.array([100, 1000, 2000, 3000, 5000])

handcrafted_avg = np.array([0.0266, 4.468, 44.134, 160.224, 842.56])
handcrafted_std = np.array([0.00765, 0.0431, 0.3550, 1.103, 3.227])

eigen_avg = np.array([0.0233, 0.1283, 0.77, 2.49, 11.168])
eigen_std = np.array([0.00745, 0.0186, 0.0183, 0.0190, 0.0392])

# ===============================
# 1️⃣ Plot of average execution time
# ===============================
plt.figure(figsize=(8,5))
plt.plot(N, handcrafted_avg, '-o', color='red', label='Handcrafted')
plt.plot(N, eigen_avg, '-o', color='blue', label='Eigen')

plt.yscale('log')
plt.title('Average Execution Time')
plt.xlabel('Matrix size (N)')
plt.ylabel('Time [s] (log scale)')
plt.grid(True, which="both", ls="--", lw=0.5)
plt.legend()
plt.tight_layout()
plt.show()

# ===============================
# 2️⃣ Plot of standard deviations
# ===============================
plt.figure(figsize=(8,5))
plt.plot(N, handcrafted_std, '-o', color='red', label='Handcrafted')
plt.plot(N, eigen_std, '-o', color='blue', label='Eigen')

plt.yscale('log')
plt.title('Standard Deviation of Execution Time')
plt.xlabel('Matrix size (N)')
plt.ylabel('Std [s] (log scale)')
plt.grid(True, which="both", ls="--", lw=0.5)
plt.legend()
plt.tight_layout()
plt.show()
