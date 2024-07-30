#ifndef SIMULATION_H
#define SIMUATION_H

#include "Particle.h"
#include <stdbool.h>

const double radius = 1;
const double force = 500;
const double cutoff = 3.5;

const double dt = 0.005;

typedef struct {
    double boxX, boxY;
    int nParticles, timestep;
    Particle* particles;
    double (*potential)(double, bool);
    double kT;
    bool pbcFlag;
    int nCellsX, nCellsY;
    double cellX, cellY;
    double temperature, potEnergy;
    Vector2 netForce;

} Simulation;

Simulation newSimulation(double boxX, double boxY, int nParticles, double (*potential)(double, bool), double kT);

void initialise(Simulation* sim);

double hardDiskPotential(double r, bool forceFlag);

double LJPotential(double r, bool forceFlag);

void calculateForces(Simulation* sim);

void run(Simulation* sim, int nSteps, bool equibFlag);

void freeSimulation(Simulation* sim);

void constructCellList(Simulation* sim);

void printSim(Simulation* sim);

void calculatePotential(Simulation* sim);

void calculateTemperature(Simulation* sim);

void calculateNetForce(Simulation* sim);

double getTemp(Simulation* sim);

double getPot(Simulation* sim);

void rescale(Simulation* sim);

#endif