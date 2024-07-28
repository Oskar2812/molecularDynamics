#ifndef PARTICLE_H
#define PARTICLE_H

#include "Vector2.h"

typedef struct{
    int id;
    int xCell, yCell;
    Vector2 pos;
    Vector2 vel;
    Vector2 force;
} Particle;

Particle newParticle(int id, Vector2 pos, Vector2 vel);

double energy(Particle* pp);

#endif
