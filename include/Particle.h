#ifndef PARTICLE_H
#define PARTICLE_H

#include "Vector.h"

typedef struct{
    int id;
    int xCell, yCell;
    Vector pos;
    Vector vel;
    Vector force;
} Particle;

Particle newParticle(int id, Vector pos, Vector vel);

double energy(Particle* pp);

void printParticle(Particle pp);

#endif
