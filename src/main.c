#include "../include/Game.h"
#include <stdio.h>

int main(){

    Simulation sim = newSimulation(80, 80, 256, LJPotential, 1.5);
    initialise(&sim);
    
    startGame(&sim, 800, 600);

    return 0;
}