#ifndef SIMULATION_H
#define SIMUATION_H

#include "Particle.h"
#include "Cell.h"
#include "Histogram.h"
#include <stdbool.h>

typedef struct {
    double boxX, boxY;
    int nParticles, timestep;
    Particle* particles;
    double (*potential)(double, bool);
    double kT, maxVel, minVel;
    bool pbcFlag, gravFlag;
    int nCellsX, nCellsY, nCells;
    double cellX, cellY;
    double temperature, potEnergy;
    Vector netForce;
    Cell* cellList;
    double* tempList; 
    double* potList;
    Histogram velHist, posHist;
} Simulation;

Simulation newSimulation(double boxX, double boxY, int nParticles, double (*potential)(double, bool), double kT);

void initialise(Simulation* sim);

double hardDiskPotential(double r, bool forceFlag);

double LJPotential(double r, bool forceFlag);

void calculateForces(Simulation* sim);

void run(Simulation* sim, int nSteps, bool equibFlag);

void freeSimulation(Simulation* sim);

void freeCellList(Simulation* sim);

void constructCellList(Simulation* sim);

void printSim(Simulation* sim);

void calculatePotential(Simulation* sim);

void calculateTemperature(Simulation* sim);

void calculateNetForce(Simulation* sim);

double getTemp(Simulation* sim);

double getPot(Simulation* sim);

void rescale(Simulation* sim);

void externForce(Simulation* sim, Vector pos, double strength);

#endif