#include "../include/Game.h"
#include <stdio.h>

int main(){

    Simulation sim = newSimulation(150, 150, 4096, LJPotential, 1.5);
    initialise(&sim);
    
    startGame(&sim, 800, 600);

    return 0;
}