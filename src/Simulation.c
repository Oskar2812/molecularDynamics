#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <math.h>

#include "/home/oskar/projects/molecularDynamics/include/Simulation.h"

const double radius = 1;
const double force = 500;
const double cutoff = 3.5;

const double dt = 0.01;

Simulation newSimulation(double boxX, double boxY, int nParticles, double (*potential)(double, bool), double kT){
    Simulation result;

    result.boxX = boxX;
    result.boxY = boxY;

    result.nParticles = nParticles;

    result.timestep = 0;

    result.particles = (Particle*)malloc(sizeof(Particle) * nParticles);
    if (result.particles == NULL) {
        fprintf(stderr, "Failed to allocate memory for particles\n");
        exit(EXIT_FAILURE);
    }

    result.potential = potential;

    result.kT = kT;

    result.pbcFlag = true;

    result.nCellsX = (int) (boxX / cutoff);
    result.nCellsY = (int) (boxY / cutoff);

    result.nCells = result.nCellsX * result.nCellsY;

    result.cellList = (Cell*)calloc(result.nCells , sizeof(Cell));
    if(result.cellList == NULL){
        fprintf(stderr, "Failed to allocate memory to cell list\n");
        exit(EXIT_FAILURE);
    }

    result.cellX = boxX / result.nCellsX;
    result.cellY = boxY / result.nCellsY;

    result.temperature = 0;
    result.potEnergy = 0;
    result.netForce = newVector(0,0);

    result.debugFlag = false;

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

int mod(int a, int b){
    int result = a % b;

    if(result < 0){
        result += b;
    }

    return result;
}

void initialise(Simulation* sim){
    srand(0);

    for(int ii = 0; ii < sim->nParticles; ii++){
        double newX = ((double) rand() / RAND_MAX) * sim->boxX;
        double newY = ((double) rand() / RAND_MAX) * sim->boxY;

        double newVx = (((double) rand() / RAND_MAX) * 2) - 1;
        double newVy = (((double) rand() / RAND_MAX) * 2) - 1;

        Vector newV = newVector(newVx, newVy);
        newV = mul(newV, 1/mag(newV));
        newV = mul(newV,generateNormal(sim->kT, sqrt(sim->kT)));

            
        sim->particles[ii] = newParticle(ii, newVector(newX, newY), newV);
    }

    double meanKTx = 0;
    double meanKTy = 0;

    for(int ii = 0; ii < sim->nParticles; ii++){
        meanKTx += sim->particles[ii].vel.x;
        meanKTy += sim->particles[ii].vel.y;
    }

    meanKTx /= sim->nParticles;
    meanKTy /= sim->nParticles;

    for(int ii = 0; ii < sim->nParticles; ii++){
        sim->particles[ii].vel.x = sim->particles[ii].vel.x - meanKTx;
        sim->particles[ii].vel.y = sim->particles[ii].vel.y - meanKTy;
    }
}

double hardDiskPotential(double r, bool forceFlag){
    if(r <= radius) return force;
    else return 0;
}

double LJPotential(double r, bool forceFlag){

    double eps = 0.25;
    double sig = 0.5;

    double rc = 0.4;

    double r2 = r * r;
    double r4 = r2 * r2;
    double r6 = r4 * r2;
    double r12 = r6 * r6;

    double sig2 = sig * sig;
    double sig4 = sig2 * sig2;
    double sig6 = sig4 * sig2;
    double sig12 = sig6 * sig6;

    if(forceFlag){
        double r13 = r12 * r;
        double r7 = r6 * r;

        if(r < rc){
            double rc6 = rc * rc * rc * rc *rc * rc;
            double rc12 = rc6 * rc6;
            return 4*eps*(12*sig12/(rc12*rc) - 6*sig6/(rc6*rc));

        }
        return 4*eps*(12*sig12/r13 - 6*sig6/r7);
    }

    if(r < rc){
        double rc6 = rc * rc * rc * rc *rc * rc;
        double rc12 = rc6 * rc6;

        return 4*eps*(sig12/rc12 - sig6/rc6);
    }
    return 4*eps*(sig12/r12 - sig6/r6);

}

int arrayCoords(Simulation* sim, int x, int y){

    int coord = y*sim->nCellsX + x;
    if(coord >= sim->nCells) exit(1);

    return coord;
}

void cellCoords(Simulation* sim, int arrayCoord, int* x, int* y){
    *y = arrayCoord / sim->nCellsX;
    *x = arrayCoord % sim->nCellsX;
}

void constructCellList(Simulation* sim){
    int cellCount[sim->nCells];
    for(int ii = 0; ii < sim->nCells; ii++){
        cellCount[ii] = 0;
        sim->cellList[ii].count = 0;
    }

    for(int ii = 0; ii < sim->nParticles; ii++){
        int xCell = (int) (sim->particles[ii].pos.x / sim->cellX);
        int yCell = (int) (sim->particles[ii].pos.y / sim->cellY);

        sim->particles[ii].xCell = xCell;
        sim->particles[ii].yCell = yCell;

        cellCount[arrayCoords(sim, xCell, yCell)] += 1;

    }


    for(int ii = 0; ii < sim->nCells; ii++){
        sim->cellList[ii].nParticles = cellCount[ii];
        fflush(stdout);
        sim->cellList[ii].particles = (Particle**)malloc(sim->cellList[ii].nParticles * sizeof(Particle*));
        if(sim->cellList[ii].particles == NULL){
            fprintf(stderr, "Failed to allocate memory to cell list\n");
            exit(EXIT_FAILURE);
        }
    }
    
    for(int ii = 0; ii < sim->nParticles; ii++){
        addParticle(&sim->cellList[arrayCoords(sim, sim->particles[ii].xCell, sim->particles[ii].yCell)], &sim->particles[ii]);
    }
}

Cell** obtainCellTargets(Simulation* sim, int arrayCoord){
    int xCell, yCell;
    cellCoords(sim, arrayCoord, &xCell, &yCell);

    Cell** result = (Cell**)malloc(9 * sizeof(Cell*));

    result[0] = &sim->cellList[arrayCoord];

    result[1] = &sim->cellList[arrayCoords(sim, mod(xCell - 1, sim->nCellsX), mod(yCell - 1, sim->nCellsY))]; 
    result[2] = &sim->cellList[arrayCoords(sim, mod(xCell, sim->nCellsX), mod(yCell - 1, sim->nCellsY))];
    result[3] = &sim->cellList[arrayCoords(sim, mod(xCell + 1, sim->nCellsX), mod(yCell - 1, sim->nCellsY))];
    result[4] = &sim->cellList[arrayCoords(sim, mod(xCell - 1, sim->nCellsX), mod(yCell, sim->nCellsY))];
    result[5] = &sim->cellList[arrayCoords(sim, mod(xCell + 1, sim->nCellsX), mod(yCell, sim->nCellsY))];
    result[6] = &sim->cellList[arrayCoords(sim, mod(xCell - 1, sim->nCellsX), mod(yCell + 1, sim->nCellsY))];
    result[7] = &sim->cellList[arrayCoords(sim, mod(xCell, sim->nCellsX), mod(yCell + 1, sim->nCellsY))];
    result[8] = &sim->cellList[arrayCoords(sim, mod(xCell + 1, sim->nCellsX), mod(yCell + 1, sim->nCellsY))];

    return result;
}

void calculateForces(Simulation* sim){
    //sim->potEnergy = 0;
    for(int ii = 0; ii < sim->nParticles; ii++){
        sim->particles[ii].force = newVector(0,0);
    }
    constructCellList(sim);
    for(int ii = 0; ii < sim->nCells; ii++){
        Cell** targets = obtainCellTargets(sim, ii);
        for(int jj = 0; jj < sim->cellList[ii].nParticles; jj++){
            for(int cell = 0; cell < 9; cell++){
                for(int kk = 0; kk < targets[cell]->nParticles; kk++){  
                    if(sim->cellList[ii].particles[jj]->id == targets[cell]->particles[kk]->id){continue;}
                    Vector sep = sub(sim->cellList[ii].particles[jj]->pos, targets[cell]->particles[kk]->pos);
                    double r = mag(sep);

                    sep = mul(sep, 1 / (r * r));

                    double force = sim->potential(r, true);
                    
                    Vector newForce = mul(sep, force);

                    sim->cellList[ii].particles[jj]->force.x += newForce.x;
                    sim->cellList[ii].particles[jj]->force.y += newForce.y;
                }
            }
        }
        free(targets);
    }
    freeCellList(sim);
    sim->debugFlag = true;
    //printSim(sim);
}

void printSim(Simulation* sim){
    for(int ii = 0; ii < sim->nParticles; ii++){
        printf("Particle: %d at (%lf, %lf) with velocity (%lf,%lf), and force (%lf, %lf)\n",
        ii, sim->particles[ii].pos.x, sim->particles[ii].pos.y, sim->particles[ii].vel.x, sim->particles[ii].vel.y,
        sim->particles[ii].force.x, sim->particles[ii].force.y);
    }
    printf("Potential Energy: %lf\nTemperature: %lf\nNet Force: (%lf,%lf)\n", sim->potEnergy, sim->temperature, sim->netForce.x, sim->netForce.y);
}

void run(Simulation* sim, int nSteps, bool equibFlag){
    for(int tt = 0; tt < nSteps; tt++){
        //First Verlet step
        if(sim->timestep == 0) calculateForces(sim);
        for(int ii = 0; ii < sim->nParticles; ii++){
            Vector vHalf = add(sim->particles[ii].vel, mul(sim->particles[ii].force, 0.5*dt));
            sim->particles[ii].vel = vHalf;

            Vector newPos = add(sim->particles[ii].pos, mul(vHalf, dt));
            
            if(sim->pbcFlag){
                if(newPos.x < 0) { 
                newPos.x += sim->boxX;
                }
                if(newPos.x > sim->boxX) {
                    newPos.x -= sim->boxX;
                }
                if(newPos.y < 0) {
                    newPos.y += sim->boxY;
                }
                if(newPos.y > sim->boxY) {
                    newPos.y -= sim->boxY;
                }
            } else {
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
            }

            sim->particles[ii].pos = newPos;
        }
        //Second Verlet step
        calculateForces(sim);
        for(int ii = 0; ii < sim->nParticles; ii++){
            Vector newV = add(sim->particles[ii].vel, mul(sim->particles[ii].force, 0.5*dt));
            sim->particles[ii].vel = newV;
        }
        sim->timestep++;

        calculateNetForce(sim);
        calculatePotential(sim);
        calculateTemperature(sim);

        if(mag(sim->netForce) > 0.1){
            printf("Net Force non-zero at timestep: %d! Force: (%lf, %lf)\n", sim->timestep, sim->netForce.x, sim->netForce.y);
            printSim(sim);
            exit(EXIT_FAILURE);
        }

        if(equibFlag && (sim->timestep % 10 == 0)) rescale(sim);
    }
}

void freeSimulation(Simulation* sim){
    free(sim->particles);
    free(sim->cellList);
}

void freeCellList(Simulation* sim){
    for(int ii = 0; ii < sim->nCells; ii++){
        free(sim->cellList[ii].particles);
    }
}

void calculatePotential(Simulation* sim){
    sim->potEnergy = 0;
    constructCellList(sim);
    for(int ii = 0; ii < sim->nCells; ii++){
        Cell** targets = obtainCellTargets(sim, ii);
        for(int jj = 0; jj < sim->cellList[ii].nParticles; jj++){
            for(int cell = 0; cell < 9; cell++){
                for(int kk = 0; kk < targets[cell]->nParticles; kk++){
                    if(sim->cellList[ii].particles[jj]->id == targets[cell]->particles[kk]->id){continue;}
                    Vector sep = sub(sim->cellList[ii].particles[jj]->pos, targets[cell]->particles[kk]->pos);

                    double r = mag(sep);
                    sim->potEnergy += sim->potential(r, false);      
                    
                }  
            }
            
        }
        free(targets);
    }
    freeCellList(sim);
}

void calculateTemperature(Simulation* sim){
    sim->temperature = 0;

    for(int ii = 0; ii < sim->nParticles; ii++){
        sim->temperature += energy(&sim->particles[ii]);
    }

    sim->temperature *= ((double)1/(double)(sim->nParticles * 1.5));
}

void calculateNetForce(Simulation* sim){
    sim->netForce = newVector(0,0);

    for(int ii = 0; ii < sim->nParticles; ii++){
        sim->netForce = add(sim->netForce, sim->particles[ii].force);
    }
}

double getTemp(Simulation* sim){
    return sim->temperature;
}

double getPot(Simulation* sim){
    return sim->potEnergy;
}

void rescale(Simulation* sim){
    calculateTemperature(sim);
    double ktEff = sim->temperature;
    for(int ii = 0; ii < sim->nParticles; ii++){
        sim->particles[ii].vel = mul(sim->particles[ii].vel, sim->kT/ktEff);
    }
}

// int main(){

//     Simulation sim = newSimulation(100,100, 128, LJPotential, 5);
//     initialise(&sim);

//     run(&sim,1000);

//     printf("%lf\n", sim.particles[0].pos.x);

//     freeSimulation(&sim);

//     return EXIT_SUCCESS;
// }
