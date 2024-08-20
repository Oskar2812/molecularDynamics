import numpy as np
import matplotlib.pyplot as plt

nParticles = [256, 512, 1024, 1536, 2048, 2560, 3072, 3586, 4096]

with open("test/timings.txt") as file:
    for line in file:
        words = line.split()
        data = [float(word) for word in words]
        plt.plot(nParticles, data)

plt.grid()
plt.show()