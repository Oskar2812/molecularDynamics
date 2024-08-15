#include "/home/oskar/projects/molecularDynamics/include/Cell.h"

#include <stdlib.h>
#include <stdio.h>

Cell newCell(int nParticles){
    Cell result;

    result.nParticles = nParticles;

    result.particles = (Particle**)malloc(nParticles * sizeof(Particle*));
    if (result.particles == NULL) {
        fprintf(stderr, "Failed to allocate memory for particles in cell\n");
        exit(EXIT_FAILURE);
    }
    result.count = 0;
    return result;
}

void freeCell(Cell* cell){
    free(cell->particles);
}

void addParticle(Cell* cell, Particle* particle){
    cell->particles[cell->count] = particle;
    cell->count++;
}


