#include "../include/Game.h"
#include <stdio.h>

int main(){

    Simulation sim = newSimulation(15, 15, 4, LJPotential, 3, false);
    initialise(&sim);
    
    startGame(&sim, 800, 600);

    return 0;
}