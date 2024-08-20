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

// Define a structure to hold slider data
typedef struct Slider {
    Rectangle track;  // The track rectangle
    Rectangle knob;   // The knob rectangle
    double minValue;   // Minimum value of the slider
    double maxValue;   // Maximum value of the slider
    double* value;      // Current value of the slider
    bool dragging;    // Is the knob currently being dragged
    char* label;

} Slider;

void drawButton(Button* button);
bool IsButtonClicked(Button *button);
bool isScreenLeftClicked(int width, int height);
bool isScreenRightClicked(int width, int height);

void drawGraph(double* datapoints, int points, Vector pos, Vector size, char* label);
void drawHist(Histogram* hist, Vector pos, Vector size, char* label);

Slider CreateSlider(double x, double y, double width, double height, double minValue, double maxValue, double* defaultValue, char* label);
void UpdateSlider(Slider *slider, Simulation* sim);

#endif