#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#include "/home/oskar/projects/molecularDynamics/include/Simulation.h"

Simulation newSimulation(double boxX, double boxY, int nParticles){
    Simulation result;

    result.boxX = boxX;
    result.boxY = boxY;

    result.nParticles = nParticles;

    result.timestep = 0;

    result.particles = (Particle*)malloc(sizeof(Particle) * nParticles);

    return result;
}

void initialise(Simulation* sim){
    srand(0);

    int id = 0;

    for(int ii = 0; ii < sim->nParticles; ii++){
        double newX = ((double) rand() / RAND_MAX) * sim->boxX;
        double newY = ((double) rand() / RAND_MAX) * sim->boxY;

        double newVx = ((double) rand() / RAND_MAX) * 0.1 * sim->boxX;
        double newVy = ((double) rand() / RAND_MAX) * 0.1 * sim->boxY;

        sim->particles[ii] = newParticle(id, newVector2(newX, newY), newVector2(newVx, newVy));
    }
}

double hardDiskPotential(double r){
    if(r <= radius) return force;
    else return 0;
}

void calculateForces(Simulation* sim){

    for(int ii = 0; ii < sim->nParticles; ii++){
        sim->particles[ii].force = newVector2(0,0);
    }
    
    for(int ii = 0; ii < sim->nParticles; ii++){
        for(int jj = ii + 1; jj < sim->nParticles; jj++){
            Vector2 sep = sub(sim->particles[ii].pos, sim->particles[jj].pos);
            double r = mag(sep);

            Vector2 newForce = mul(sep, hardDiskPotential(r));
            
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
