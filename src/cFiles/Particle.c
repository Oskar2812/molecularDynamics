#include <stdio.h>
#include "/home/oskar/projects/molecularDynamics/include/Particle.h"

Particle newParticle(int id,Vector2 pos, Vector2 vel){
    Particle p = {id, pos, vel, newVector2(0,0)};
    return p;
}

