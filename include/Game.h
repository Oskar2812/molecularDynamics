#include "Simulation.h"
#include "../raylib-5.0_linux_amd64/include/raylib.h"

typedef struct Button {
    Rectangle bounds;
    const char *text;
    Color color;
    void(*effect)(Simulation* sim);
} Button;

void startGame(Simulation* sim, int width, int height);
void endGame(Simulation* sim);
void drawGraph(Simulation* sim, double* datapoints, Vector pos, Vector size, char* label);