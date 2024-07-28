#include <stdio.h>
#include "/home/oskar/projects/molecularDynamics/include/Particle.h"

Particle newParticle(int id, Vector2 pos, Vector2 vel){
    Particle p;
    p.id = id;
    p.pos = pos;
    p.vel = vel;
    p.force = newVector2(0,0);
    p.xCell = -1;
    p.yCell = -1;
    return p;
}

double energy(Particle* pp){
    return 0.5*mag2(pp->vel);
}

