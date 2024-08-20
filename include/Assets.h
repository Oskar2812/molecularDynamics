#ifndef ASSETS_H
#define ASSETS_H

#include "Histogram.h"
#include "Simulation.h"
#include "Vector.h"
#include "../raylib-5.0_linux_amd64/include/raylib.h"

typedef struct Button {
    Rectangle bounds;
    const char *text;
    Color color;
    void(*effect)(Simulation* sim);
} Button;

void drawButton(Button* button);
bool IsButtonClicked(Button *button);
bool isScreenLeftClicked(int width, int height);
bool isScreenRightClicked(int width, int height);

void drawGraph(double* datapoints, int points, Vector pos, Vector size, char* label);
void drawHist(Histogram* hist, Vector pos, Vector size, char* label);

#endif