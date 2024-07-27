#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <math.h>

#include "/home/oskar/projects/molecularDynamics/include/Simulation.h"

Simulation newSimulation(double boxX, double boxY, int nParticles, double (*potential)(double), double kT){
    Simulation result;

    result.boxX = boxX;
    result.boxY = boxY;

    result.nParticles = nParticles;

    result.timestep = 0;

    result.particles = (Particle*)malloc(sizeof(Particle) * nParticles);

    result.potential = potential;

    result.kT = kT;

    return result;
}

double generateStandardNormal() {
    static const double PI = 3.14159265358979323846;
    double u1 = ((double) rand() / RAND_MAX);
    double u2 = ((double) rand() / RAND_MAX);
    double z0 = sqrt(-2.0 * log(u1)) * cos(2.0 * PI * u2);
    // Optionally, you can generate another independent standard normal random variable:
    // double z1 = sqrt(-2.0 * log(u1)) * sin(2.0 * PI * u2);
    return z0;
}

// Function to generate a random number following a normal distribution with mean and standard deviation
double generateNormal(double mean, double stddev) {
    return mean + stddev * generateStandardNormal();
}


void initialise(Simulation* sim){
    srand(0);

    int id = 0;

    for(int ii = 0; ii < sim->nParticles; ii++){
        double newX = ((double) rand() / RAND_MAX) * sim->boxX;
        double newY = ((double) rand() / RAND_MAX) * sim->boxY;

        double newVx = (((double) rand() / RAND_MAX) * 2) - 1;
        double newVy = (((double) rand() / RAND_MAX) * 2) - 1;

        Vector2 newV = newVector2(newVx, newVy);
        newV = mul(newV, mag(newV));
        newV = mul(newV,generateNormal(sim->kT, sqrt(sim->kT)));


        sim->particles[ii] = newParticle(id, newVector2(newX, newY), newVector2(newVx, newVy));
    }

    double meanKTx = 0;
    double meanKTy = 0;

    for(int ii = 0; ii < sim->nParticles; ii++){
        meanKTx += 0.5 * sim->particles[ii].vel.x * sim->particles[ii].vel.x;
        meanKTy += 0.5 * sim->particles[ii].vel.y * sim->particles[ii].vel.y;
    }

    meanKTx /= sim->nParticles;
    meanKTy /= sim->nParticles;

    for(int ii = 0; ii < sim->nParticles; ii++){
        sim->particles[ii].vel.x = sim->particles[ii].vel.x - meanKTx;
        sim->particles[ii].vel.y = sim->particles[ii].vel.y - meanKTy;
    }
}

double hardDiskPotential(double r){
    if(r <= radius) return force;
    else return 0;
}

double LJPotential(double r){
    double r2 = r * r;
    double r4 = r2 * r2;
    double r6 = r4 * r2;
    double r12 = r6 * r6;

    return 1/r12 - 1/r6;

}

void calculateForces(Simulation* sim){

    for(int ii = 0; ii < sim->nParticles; ii++){
        sim->particles[ii].force = newVector2(0,0);
    }
    
    for(int ii = 0; ii < sim->nParticles; ii++){
        for(int jj = ii + 1; jj < sim->nParticles; jj++){
            Vector2 sep = sub(sim->particles[ii].pos, sim->particles[jj].pos);
            double r = mag(sep);

            Vector2 newForce = mul(sep, sim->potential(r));
            
            sim->particles[jj].force.x -= newForce.x;
            sim->particles[jj].force.y -= newForce.y;
    
            sim->particles[ii].force.x += newForce.x;
            sim->particles[ii].force.y += newForce.y;
        }
    }
}

void run(Simulation* sim, int nSteps){

    for(int tt = 0; tt < nSteps; tt++){
        //First Verlet step
        if(sim->timestep == 0) calculateForces(sim);
        for(int ii = 0; ii < sim->nParticles; ii++){
            Vector2 vHalf = add(sim->particles[ii].vel, mul(sim->particles[ii].force, 0.5*dt));
            sim->particles[ii].vel = vHalf;

            Vector2 newPos = add(sim->particles[ii].pos, mul(vHalf, dt));

            if(newPos.x < 0) { 
                newPos.x = 0;
                sim->particles[ii].vel.x *= -1;
            }
            if(newPos.x > sim->boxX) {
                newPos.x = sim->boxX;
                sim->particles[ii].vel.x *= -1;
            }
            if(newPos.y < 0) {
                newPos.y = 0;
                sim->particles[ii].vel.y *= -1;
            }
            if(newPos.y > sim->boxY) {
                newPos.y = sim->boxY;
                sim->particles[ii].vel.y *= -1;
            }
            sim->particles[ii].pos = newPos;
        }

        //Second Verlet step
        calculateForces(sim);
        for(int ii = 0; ii < sim->nParticles; ii++){
            Vector2 newV = add(sim->particles[ii].vel, mul(sim->particles[ii].force, 0.5*dt));
            sim->particles[ii].vel = newV;
        }

        sim->timestep++;
    }
}

void freeSimulation(Simulation* sim){
    free(sim->particles);
}
