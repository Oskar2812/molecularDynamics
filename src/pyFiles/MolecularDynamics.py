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
                ("xCell", ctypes.c_int),
                ("yCell", ctypes.c_int),
                ("pos", Vector2),
                ("vel", Vector2),
                ("force", Vector2)]

class Simulation(ctypes.Structure):
    _fields_ = [("boxX", ctypes.c_double),
                ("boxY", ctypes.c_double),
                ("nParticles", ctypes.c_int),
                ("timestep", ctypes.c_int),
                ("particles", ctypes.POINTER(Particle)),
                ("potential", ctypes.CFUNCTYPE(ctypes.c_double, ctypes.c_double, ctypes.c_bool)),
                ("kT", ctypes.c_double),
                ("nCellsX", ctypes.c_int),
                ("nCellsY", ctypes.c_int),
                ("cellX", ctypes.c_double),
                ("cellY", ctypes.c_double),
                ("temperature", ctypes.c_double),
                ("potEnergy", ctypes.c_double),
                ("netForce", Vector2)]

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
newSimulation.argtypes = [ctypes.c_double, ctypes.c_double, ctypes.c_int, 
                          ctypes.CFUNCTYPE(ctypes.c_double, ctypes.c_double, ctypes.c_bool), ctypes.c_double]
newSimulation.restype = Simulation

initialise = MD.initialise
initialise.argtypes = [ctypes.POINTER(Simulation)]
initialise.restype = None

hardDiskPotential = MD.hardDiskPotential
hardDiskPotential.argtypes = [ctypes.c_double, ctypes.c_bool]
hardDiskPotential.restype = ctypes.c_double

LJPotential = MD.LJPotential
LJPotential.argtypes = [ctypes.c_double, ctypes.c_bool]
LJPotential.restype = ctypes.c_double

calculateForces = MD.calculateForces
calculateForces.argtypes = [ctypes.POINTER(Simulation)]
calculateForces.restype = None

run = MD.run
run.argtypes = [ctypes.POINTER(Simulation), ctypes.c_int, ctypes.c_bool]
run.restype = None

freeSimulation = MD.freeSimulation
freeSimulation.argtypes = [ctypes.POINTER(Simulation)]
freeSimulation.restype = None

printSim = MD.printSim
printSim.argtypes = [ctypes.POINTER(Simulation)]
printSim.restypes = None

getTemp = MD.getTemp
getTemp.argtypes = [ctypes.POINTER(Simulation)]
getTemp.restype = ctypes.c_double

getPot = MD.getPot
getPot.argtypes = [ctypes.POINTER(Simulation)]
getPot.restype = ctypes.c_double

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

    # Window for temperature over time
    win2 = pg.GraphicsLayoutWidget(show=True, title="Temperature Over Time")
    plot2 = win2.addPlot()
    temp_curve = plot2.plot()
    
    # Window for potential energy over time
    win3 = pg.GraphicsLayoutWidget(show=True, title="Potential Energy Over Time")
    plot3 = win3.addPlot()
    energy_curve = plot3.plot()

    temps = []
    energies = []
    timesteps = []
    
    def update():
        run(sim, visStep, True)
        temps.append(getTemp(sim))
        energies.append(getPot(sim))
        timesteps.append(sim.timestep)
        scatter.setData(pos=particlePosToArray(sim))
        scatter.setSize(calculate_particle_size())
        text_item.setText(f'Timestep: {sim.timestep}')
        temp_curve.setData(timesteps, temps)
        energy_curve.setData(timesteps, energies)
    
    timer = QtCore.QTimer()
    timer.timeout.connect(update)
    timer.start(updateInterval)
    
    QtWidgets.QApplication.instance().exec_()

if __name__ == '__main__':
    # Create the simulation
    potential = ctypes.CFUNCTYPE(ctypes.c_double, ctypes.c_double, ctypes.c_bool)(LJPotential)

    try:
        sim = newSimulation(100, 100, 128, potential, 1.5)
    except Exception as e:
        print(f"Error creating simulation: {e}")

    initialise(sim)

    # Run the simulation
    start = time.time()

    runSimulation(sim, 1)
    printSim(sim)
    freeSimulation(sim)

    print(f"Simulation complete! Time taken: {time.time()-start}")

















