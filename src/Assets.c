#include "../include/Assets.h"
#include <stdlib.h>
#include <stdio.h>

void drawGraph(double* datapoints, int points, Vector pos, Vector size, char* label){
    int start = 0;
    if(points > 1000){
        start = points - 1000;
    }
    const int offset = 30;
    DrawLine(pos.x + offset, pos.y + offset, pos.x + offset, pos.y + size.y - offset, RAYWHITE);
    DrawLine(pos.x + offset, pos.y + size.y - offset, pos.x + size.x - offset, pos.y + size.y - offset, RAYWHITE);

    DrawText(label, pos.x + offset + 10, pos.y + size.y - offset + 10, 10, RAYWHITE);

    double max = datapoints[start];
    double min = datapoints[start];
    for(int ii = start; ii < points; ii++){
        if(datapoints[ii] > max){
            max = datapoints[ii];
        }
        if(datapoints[ii] < min){
            min = datapoints[ii];
        }
    }

    double range = abs(max - min);

    for(int ii = 0; ii < 10; ii++){
        char yAxisMarking[100];
        sprintf(yAxisMarking, "%.2lf", min + ii * range / 9);
        DrawLine(pos.x + offset/1.5, pos.y + size.y - offset - ii * (size.y - 2 * offset) / 9,
        pos.x + offset, pos.y + size.y - offset - ii * (size.y - 2 * offset) / 9, RAYWHITE);
        DrawText(yAxisMarking, pos.x + offset/5, pos.y + size.y - offset - ii * (size.y - 2 * offset) / 9 - 5, 1, RAYWHITE);

        char xAxisMarking[100];
        sprintf(xAxisMarking, "%d",  start + ii * (points - start) / 10);
        DrawLine(pos.x + offset + ii * (size.x - 2* offset)/ 10 , pos.y + size.y - offset,
        pos.x + offset + ii* (size.x - 2* offset) / 10, pos.y + size.y - 0.9*offset, RAYWHITE);
        DrawText(xAxisMarking, pos.x + offset + ii * (size.x - 2* offset) / 10, pos.y + size.y - offset, 1, RAYWHITE);
    }

    if(min < 0){
        max = max - min;
    }

    for(int ii = start + 1; ii < points; ii++){
        double y1, y2;
        if(min < 0){
            y1 = (pos.y + size.y - offset) - ((datapoints[ii - 1] - min) / max) * (size.y - 2*offset);
            y2 = (pos.y + size.y - offset) - ((datapoints[ii] - min) / max) * (size.y - 2*offset);
        } else {
            y1 = (pos.y + size.y - offset) - (datapoints[ii - 1] / max) * (size.y - 2*offset);
            y2 = (pos.y + size.y - offset) - (datapoints[ii] / max) * (size.y - 2*offset);
        }
        double x1 = pos.x + offset + (double)(ii - start - 1) / (double)(points - start) * (size.x - 2*offset);
        double x2 = pos.x + offset + (double)(ii - start) / (double)(points - start) * (size.x - 2*offset);
        DrawLine(x1, y1, x2, y2 , RAYWHITE);
    }  
}

void drawHist(Histogram* hist, Vector pos, Vector size, char* label){
    const int offset = 30;
    DrawLine(pos.x + offset, pos.y + offset, pos.x + offset, pos.y + size.y - offset, RAYWHITE);
    DrawLine(pos.x + offset, pos.y + size.y - offset, pos.x + size.x - offset, pos.y + size.y - offset, RAYWHITE);

    DrawText(label, pos.x + offset + 1, pos.y + size.y - offset + 10, 10, RAYWHITE);

    double max = hist->counts[0];
    double min = hist->counts[0];
    for(int ii = 0; ii < hist->nBins; ii++){
        if(hist->counts[ii] > max){
            max = hist->counts[ii];
        }
        if(hist->counts[ii] < min){
            min = hist->counts[ii];
        }
    }

    double range = abs(max - min);
    double inc = abs(hist->end - hist->start);

    for(int ii = 0; ii < 10; ii++){
        char yAxisMarking[100];
        sprintf(yAxisMarking, "%.2lf", min + ii * range / 9);
        DrawLine(pos.x + offset/1.5, pos.y + size.y - offset - ii * (size.y - 2 * offset) / 9,
        pos.x + offset, pos.y + size.y - offset - ii * (size.y - 2 * offset) / 9, RAYWHITE);
        DrawText(yAxisMarking, pos.x + offset/5, pos.y + size.y - offset - ii * (size.y - 2 * offset) / 9 - 5, 1, RAYWHITE);

        char xAxisMarking[100];
        sprintf(xAxisMarking, "%.2lf", hist->start + ii*inc);
        DrawLine(pos.x + offset + ii * (size.x - 2* offset)/ 10 , pos.y + size.y - offset,
        pos.x + offset + ii* (size.x - 2* offset) / 10, pos.y + size.y - 0.9*offset, RAYWHITE);
        DrawText(xAxisMarking, pos.x + offset + ii * (size.x - 2* offset) / 10, pos.y + size.y - offset, 1, RAYWHITE);
    }

    if(min < 0){
        max = max - min;
    }

    for(int ii = 1; ii < hist->nBins; ii++){
        double y1, y2;
        if(min < 0){
            y1 = (pos.y + size.y - offset) - ((hist->counts[ii - 1] - min) / max) * (size.y - 2*offset);
            y2 = (pos.y + size.y - offset) - ((hist->counts[ii] - min) / max) * (size.y - 2*offset);
        } else {
            y1 = (pos.y + size.y - offset) - (hist->counts[ii - 1] / max) * (size.y - 2*offset);
            y2 = (pos.y + size.y - offset) - (hist->counts[ii] / max) * (size.y - 2*offset);
        }
        double x1 = pos.x + offset + (double)(ii - 1) / (double)hist->nBins * (size.x - 2*offset);
        double x2 = pos.x + offset + (double)ii / (double)hist->nBins * (size.x - 2*offset);
        DrawLine(x1, y1, x2, y2 , RAYWHITE);
    }  
}

void drawButton(Button* button){
    DrawRectangleRec(button->bounds, button->color);

    int textX = button->bounds.x + 10;
    int textY = button->bounds.y + (button->bounds.height / 2) - 5;

    DrawText(button->text, textX, textY, 15, RAYWHITE);
}

bool IsButtonClicked(Button *button) {
    // Check if the button is clicked
    if (CheckCollisionPointRec(GetMousePosition(), button->bounds) && IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) {
        return true;
    }
    return false;
}

bool isScreenLeftClicked(int width, int height){
    Rectangle screen = {0,0,width,height};
    if(CheckCollisionPointRec(GetMousePosition(), screen) && IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) {
        return true;
    }
    return false;
}

bool isScreenRightClicked(int width, int height){
    Rectangle screen = {0,0,width,height};
    if(CheckCollisionPointRec(GetMousePosition(), screen) && IsMouseButtonReleased(MOUSE_RIGHT_BUTTON)) {
        return true;
    }
    return false;
}