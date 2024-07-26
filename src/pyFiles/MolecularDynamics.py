import matplotlib
matplotlib.use('TkAgg')

import ctypes
import matplotlib.pyplot as plt
from matplotlib.backends.backend_tkagg import FigureCanvasTkAgg
import tkinter as tk
import time

MD = ctypes.CDLL("/home/oskar/projects/molecularDynamics/src/cFiles/molecularDynamics.so")

class Vector2(ctypes.Structure):
    _fields_ = [("x", ctypes.c_double),
                ("y", ctypes.c_double)]
    
class Particle(ctypes.Structure):
    _fields_ = [("id", ctypes.c_int),
                ("pos", Vector2),
                ("vel", Vector2),
                ("force", Vector2)]

class Simulation(ctypes.Structure):
    _fields_ = [("boxX", ctypes.c_double),
                ("boxY", ctypes.c_double),
                ("nParticles", ctypes.c_int),
                ("timestep", ctypes.c_int),
                ("particles", ctypes.POINTER(Particle))]

newVector2 = MD.newVector2
newVector2.argtypes = [ctypes.c_double,ctypes.c_double]
newVector2.restype = Vector2

add = MD.add
add.argtypes = [Vector2, Vector2]
add.restype = Vector2

sub = MD.sub
sub.argtypes = [Vector2, Vector2]
sub.restype = Vector2

mul = MD.mul
mul.argtypes = [Vector2, ctypes.c_double]
mul.restype = Vector2

dot = MD.dot
dot.argtypes = [Vector2, Vector2]
dot.restype = ctypes.c_double

newParticle = MD.newParticle
newParticle.argtypes = [ctypes.c_int, Vector2, Vector2]
newParticle.restype = Particle

newSimulation = MD.newSimulation
newSimulation.argtypes = [ctypes.c_double, ctypes.c_double, ctypes.c_int]
newSimulation.restype = Simulation

initialise = MD.initialise
initialise.argtypes = [ctypes.POINTER(Simulation)]
initialise.restype = None

hardDiskPotential = MD.hardDiskPotential
hardDiskPotential.argtypes = [ctypes.c_double]
hardDiskPotential.restype = ctypes.c_double

calculateForces = MD.calculateForces
calculateForces.argtypes = [ctypes.POINTER(Simulation)]
calculateForces.restype = None

run = MD.run
run.argtypes = [ctypes.POINTER(Simulation), ctypes.c_int]
run.restype = None

def visualiseSim(sim, ax):
    ax.clear()
    for ii in range(sim.nParticles):
        ax.scatter(sim.particles[ii].pos.x, sim.particles[ii].pos.y, color = 'r', s = 3.14 * (2.5)**2)

    ax.set_xlim(0, sim.boxX)
    ax.set_ylim(0, sim.boxY)
    ax.set_title(f"Tmestep: {sim.timestep}")
    plt.draw()

def printSim(sim):
    print("Postions:    | Velocities:    | Forces:     ")
    for ii in range(sim.nParticles):
        print(f"({sim.particles[ii].pos.x:.2f}, {sim.particles[ii].pos.y:.2f}) | ({sim.particles[ii].vel.x:.2f}, {sim.particles[ii].vel.y:.2f}) | ({sim.particles[ii].force.x:.2f}, {sim.particles[ii].force.y:.2f})")

def runSimulation(sim, nSteps, visStep, canvas, ax, isVis = True):
    def update_simulation():
        nonlocal totalSteps
        if totalSteps < nSteps:
            run(sim, visStep)
            totalSteps += visStep
            if(isVis): 
                visualiseSim(sim, ax)
                canvas.draw()
            root.after(10, update_simulation)  # Schedule next update
        else:
            root.quit()
            plt.close()

    totalSteps = 0
    #visualiseSim(sim, ax)
    update_simulation()


# Initialize Tkinter window
root = tk.Tk()
root.title("Molecular Dynamics Simulation")

# Create Matplotlib figure and axis
fig, ax = plt.subplots(figsize=(8, 8))

# Create a canvas widget to display the plot
canvas = FigureCanvasTkAgg(fig, master=root)
canvas.get_tk_widget().pack()

# Create the simulation
sim = newSimulation(100, 100, 512)
initialise(sim)

# Run the simulation
start = time.time()
runSimulation(sim, 1000, 20, canvas, ax, True)

# Start Tkinter main loop
root.mainloop()
print(f"Simulation complete! Time taken: {time.time()-start}")















