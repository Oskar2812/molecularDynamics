#ifndef SIMULATION_H
#define SIMUATION_H

#include "Particle.h"

const double radius = 1;
const double force = 500;

const double dt = 0.01;

typedef struct {
    double boxX;
    double boxY;
    int nParticles;
    int timestep;
    Particle* particles;
} Simulation;

Simulation newSimulation(double boxX, double boxY, int nParticles);

void initialise(Simulation* sim);

double hardDiskPotential(double r);

void calculateForces(Simulation* sim);

void run(Simulation* sim, int nSteps);
#endif