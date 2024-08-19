#ifndef PARTICLE_H
#define PARTICLE_H

#include "Vector.h"

typedef struct{
    int id;
    int xCell, yCell;
    double velMag;
    Vector pos;
    Vector vel;
    Vector force;
} Particle;

Particle newParticle(int id, Vector pos, Vector vel);

void printParticle(Particle pp);

#endif
