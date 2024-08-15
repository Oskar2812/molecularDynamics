#ifndef CELL_H
#define CELL_H

#include "Particle.h"

typedef struct {
    int nParticles;
    int count;
    Particle** particles;

} Cell;


Cell newCell(int nParticless);
void freeCell(Cell* cell);
void addParticle(Cell* cell, Particle* particle);

#endif