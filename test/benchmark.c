#include "../include/Simulation.h"

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <omp.h>

void benchmarkVarParticles(int nSteps){
    const double targDensity = 1;

    int nParticles[] = {256, 512, 1024, 1536, 2048, 2560, 3072, 3584, 4096};
    double times[9];

    for(int ii = 0; ii < 9; ii++){

        double box = sqrt(nParticles[ii]);

        double start = omp_get_wtime();
        Simulation sim = newSimulation(box, box, nParticles[ii], LJPotential, 1.5);

        initialise(&sim);

        run(&sim, nSteps, true);

        double time = (omp_get_wtime() - start);
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
}

void timeSimulationFunctions(int nSteps){
    Simulation sim = newSimulation(45.25,45.25, 2048, LJPotential, 1.5);

    initialise(&sim);
    run(&sim, nSteps, true);

    printf("Function timings: \ncalculateForces: %lf\nrun: %lf\nconstructCellList: %lf\nobtainCellTargets: %lf\n",
            sim.calcForcesTime, sim.runTime, sim.constrCellListTime, sim.targetsTime);
}

int main(){
    printf("Begging benchmarking, number of threads: %d\n", omp_get_num_threads());
    benchmarkVarParticles(5000);
    
    return EXIT_SUCCESS;
}