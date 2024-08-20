#include "../include/Simulation.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

int main(){

    const double targDensity = 1;

    int nParticles[] = {256, 512, 1024, 1536, 2048, 2560, 3072, 3584, 4096};
    double times[9];

    for(int ii = 0; ii < 9; ii++){

        double box = sqrt(nParticles[ii]);

        double start = clock();
        Simulation sim = newSimulation(box, box, nParticles[ii], LJPotential, 1.5);

        initialise(&sim);

        run(&sim, 5, true);

        double time = (clock() - start) / CLOCKS_PER_SEC;
        printf("Run for %d particles complete. Time taken: %lfs\n", nParticles[ii], time);
        fflush(stdout);
        times[ii] = time;
    }

    FILE* file;
    const char* filename = "../test/timings.txt";

    file = fopen(filename, "a");
    if(file == NULL){
        printf("Error: couldnt open file\n");
        exit(EXIT_FAILURE);
    }

    for(int ii = 0; ii < 9; ii++){
        fprintf(file, "%lf ", times[ii]);
    }
    fprintf(file,"\n");

    fclose(file);

    return EXIT_SUCCESS;
}