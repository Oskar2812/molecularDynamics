#include <stdio.h>
#include "../include/Particle.h"

Particle newParticle(int id, Vector pos, Vector vel){
    Particle p;
    p.id = id;
    p.pos = pos;
    p.vel = vel;
    p.force = newVector(0,0);
    p.xCell = -1;
    p.yCell = -1;
    return p;
}

double energy(Particle* pp){
    return 0.5*mag2(pp->vel);
}

void printParticle(Particle pp){
    printf("Particle: %d at (%lf, %lf) with velocity (%lf,%lf), and force (%lf, %lf)\n",
        pp.id, pp.pos.x, pp.pos.y, pp.vel.x, pp.vel.y, pp.force.x, pp.force.y);
}

