#ifndef SIMULATION_H
#define SIMUATION_H

#include "Particle.h"

const double radius = 1;
const double force = 500;

const double dt = 0.05;

typedef struct {
    double boxX;
    double boxY;
    int nParticles;
    int timestep;
    Particle* particles;
    double (*potential)(double);
    double kT;

} Simulation;

Simulation newSimulation(double boxX, double boxY, int nParticles, double (*potential)(double), double kT);

void initialise(Simulation* sim);

double hardDiskPotential(double r);

double LJPotential(double r);

void calculateForces(Simulation* sim);

void run(Simulation* sim, int nSteps);

void freeSimulatiom(Simulation* sim);

#endif