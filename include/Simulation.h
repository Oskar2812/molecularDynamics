#ifndef SIMULATION_H
#define SIMUATION_H

#include "Particle.h"
#include <stdbool.h>

const double radius = 1;
const double force = 500;
const double cutoff = 3.5;

const double dt = 0.01;

typedef struct {
    double boxX, boxY;
    int nParticles, timestep;
    Particle* particles;
    double (*potential)(double);
    double kT;
    bool pbcFlag;
    int nCellsX, nCellsY;
    double cellX, cellY;
    double temperature, potEnergy;

} Simulation;

Simulation newSimulation(double boxX, double boxY, int nParticles, double (*potential)(double), double kT);

void initialise(Simulation* sim);

double hardDiskPotential(double r);

double LJPotential(double r);

void calculateForces(Simulation* sim);

void run(Simulation* sim, int nSteps);

void freeSimulation(Simulation* sim);

void constructCellList(Simulation* sim);

#endif