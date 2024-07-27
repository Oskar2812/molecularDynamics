import ctypes
import time

import pyqtgraph as pg
from pyqtgraph.Qt import QtGui, QtCore, QtWidgets

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
                ("particles", ctypes.POINTER(Particle)),
                ("potential", ctypes.CFUNCTYPE(ctypes.c_double, ctypes.c_double))]

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
newSimulation.argtypes = [ctypes.c_double, ctypes.c_double, ctypes.c_int, ctypes.CFUNCTYPE(ctypes.c_double, ctypes.c_double)]
newSimulation.restype = Simulation

initialise = MD.initialise
initialise.argtypes = [ctypes.POINTER(Simulation)]
initialise.restype = None

hardDiskPotential = MD.hardDiskPotential
hardDiskPotential.argtypes = [ctypes.c_double]
hardDiskPotential.restype = ctypes.c_double

LJPotential = MD.LJPotential
LJPotential.argtypes = [ctypes.c_double]
LJPotential.restype = ctypes.c_double

calculateForces = MD.calculateForces
calculateForces.argtypes = [ctypes.POINTER(Simulation)]
calculateForces.restype = None

run = MD.run
run.argtypes = [ctypes.POINTER(Simulation), ctypes.c_int]
run.restype = None

def printSim(sim):
    print("Postions:    | Velocities:    | Forces:     ")
    for ii in range(sim.nParticles):
        print(f"({sim.particles[ii].pos.x:.2f}, {sim.particles[ii].pos.y:.2f}) | ({sim.particles[ii].vel.x:.2f}, {sim.particles[ii].vel.y:.2f}) | ({sim.particles[ii].force.x:.2f}, {sim.particles[ii].force.y:.2f})")

def particlePosToArray(sim):
    result = []
    for ii in range(sim.nParticles):
        result.append([sim.particles[ii].pos.x, sim.particles[ii].pos.y])
    
    return result


def runSimulation(sim, visStep):
    fps = 30
    updateInterval = int(1000 / fps)
    app = QtWidgets.QApplication([])
    win = pg.GraphicsLayoutWidget(show=True)
    plot = win.addPlot()

    particle_radius_sim_units = 1

    def calculate_particle_size():
        plot_area_size = win.size()
        plot_width_pixels = plot_area_size.width()
        plot_height_pixels = plot_area_size.height()

        # Assuming the plot area corresponds directly to the simulation box size
        box_width_sim_units = sim.boxX
        box_height_sim_units = sim.boxY

        # Calculate pixels per simulation unit
        pixels_per_unit_x = plot_width_pixels / box_width_sim_units
        pixels_per_unit_y = plot_height_pixels / box_height_sim_units

        # Use the minimum to ensure particles are not too large
        pixels_per_unit = min(pixels_per_unit_x, pixels_per_unit_y)

        # Return the diameter in pixels (size is diameter, not radius)
        return int(particle_radius_sim_units * 2 * pixels_per_unit)

    scatter = pg.ScatterPlotItem(size=calculate_particle_size(), pen=pg.mkPen(None), brush=pg.mkBrush(255, 255, 255, 120))
    plot.addItem(scatter)
    plot.setRange(xRange=[0, sim.boxX], yRange=[0, sim.boxY])

    text_item = pg.TextItem('', anchor=(0, 1), color='w', fill=pg.mkBrush(0, 0, 0, 150))
    plot.addItem(text_item)
    text_item.setPos(0, sim.boxY)
    
    def update():
        run(sim, visStep)
        scatter.setData(pos=particlePosToArray(sim))
        scatter.setSize(calculate_particle_size())
        text_item.setText(f'Timestep: {sim.timestep}')
    
    timer = QtCore.QTimer()
    timer.timeout.connect(update)
    timer.start(updateInterval)
    
    QtWidgets.QApplication.instance().exec_()


# Create the simulation
potential = ctypes.CFUNCTYPE(ctypes.c_double, ctypes.c_double)(LJPotential)
sim = newSimulation(100, 100, 128, potential)
initialise(sim)

# Run the simulation
start = time.time()
runSimulation(sim, 100)

print(f"Simulation complete! Time taken: {time.time()-start}")















