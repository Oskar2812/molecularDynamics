import matplotlib.pyplot as plt
import timeit
import math
import numpy as np
from scipy.optimize import curve_fit

nValues = [128,256,512,1024,1536,2048,2600]
rho = 0.1
times = []

for N in nValues:
    box = math.sqrt(N / rho)
    setupCode = f"""
import ctypes
import MolecularDynamics as MD
potential = ctypes.CFUNCTYPE(ctypes.c_double, ctypes.c_double, ctypes.c_bool)(MD.LJPotential)
sim = MD.newSimulation({box}, {box}, {N}, potential, 1.5)
MD.initialise(sim)
    """
    time_taken = timeit.timeit("MD.run(sim, 1000, True)", setup=setupCode, number=1)
    times.append(time_taken)

log_nValues = np.log(nValues)
log_times = np.log(times)

# Define the model to fit: log(T) = log(c) + a * log(N)
def model(log_N, log_c, a):
    return log_c + a * log_N

# Perform the curve fitting
params, _ = curve_fit(model, log_nValues, log_times)
log_c, a = params

# Compute the fitted values for plotting
fitted_log_times = model(log_nValues, log_c, a)
fitted_times = np.exp(fitted_log_times)

plt.plot(nValues, times, 'o', label="Original Data")
plt.plot(nValues, fitted_times, '-', label=f"Fitted: N^{a:.2f}")
plt.xlabel('Number of Particles (N)')
plt.ylabel('Time (seconds)')
plt.legend()
plt.grid(True)
plt.title('Time Complexity Estimation')
plt.show()
