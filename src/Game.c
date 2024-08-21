#include "../include/Game.h"
#include "../include/Assets.h"
#include <stdio.h>
#include <stdlib.h>

Vector simCoordsToRayCoords(Vector v, int width, int height, Simulation* sim){
    Vector result;
    result.x = v.x * (width / sim->boxX);
    result.y = height - v.y * (height / sim->boxY);

    return result;
}

Color velColour(Simulation* sim, double vel){
        double normVel = (vel - sim->minVel) / (sim->maxVel - sim->minVel);

        double red = (255 * normVel);
        double blue = (255 * (1 - normVel));

        return (Color) {red, 0, blue, 255};
    }

void startGame(Simulation* sim, int width, int height){
    SetTraceLogLevel(LOG_ERROR);
    InitWindow(width + width, height + 0.25*height, "Molecular Dynamics Simulation");
    //SetWindowSize(width, height); 
    SetTargetFPS(120);

    double radius = 0.2 * width / sim->boxX;

    Button gravButton = {
        .bounds = {width - 90, 20, 70, 30},
        .text = "Gravity",
        .color = (Color){200,200,200,128},
        .flag = &sim->gravFlag,
    };

    Button pbcButton = {
        .bounds = {width - 180, 20, 70, 30},
        .text = "PBC",
        .color = (Color){200,200,200,128},
        .flag = &sim->pbcFlag,
    };

    Button pauseButton = {
        .bounds = {20, height - 40, 30, 30},
        .text = "||",
        .color = (Color) {200,200,200,128},
    };

    bool isPaused = false;

    Slider tempSlider = CreateSlider(30, height + 30, 0.45*(width - 30), 0.025*height, 0, 5, &sim->kT, "kT");
    Slider gravSlider = CreateSlider(30, height + 0.025 * height + 80, 0.45*(width - 30), 0.025*height, -10, 10, &sim->G, "Gravitational Strength");

    while(!WindowShouldClose()){
        if(!isPaused) run(sim, 1, true);
        BeginDrawing();
        ClearBackground(BLACK);

        for(int ii = 0; ii < sim->nParticles; ii++){
            Vector pos = simCoordsToRayCoords(sim->particles[ii].pos, width, height, sim);
            DrawCircle(pos.x,pos.y,radius, velColour(sim, sim->particles[ii].velMag));
        }

        DrawLine(width,0,width,height,RAYWHITE);
        DrawLine(0, height, width, height, RAYWHITE);
        
        char text[100];
        sprintf(text, "Timestep: %d, Framerate: %d", sim->timestep, GetFPS());
        DrawText(text,5,5,20, RAYWHITE);

        drawGraph(sim->tempList, sim->timestep, newVector(width, 0), newVector(0.5*width, 0.5*height), "Temperature");
        drawGraph(sim->potList, sim->timestep, newVector(width, 0.5*height), newVector(0.5 * width, 0.5*height), "Potential Energy");
        drawHist(&sim->velHist, newVector(1.5*width, 0), newVector(0.5*width, 0.5*height), "Velocity distribution");
        drawHist(&sim->posHist, newVector(1.5*width, 0.5*height), newVector(0.5*width, 0.5*height), "Radial distribution");

        drawButton(&gravButton);
        drawButton(&pbcButton);
        drawButton(&pauseButton);

        UpdateSlider(&tempSlider, sim);
        UpdateSlider(&gravSlider, sim);

        if(IsButtonClicked(&gravButton)) {
            *gravButton.flag = !*gravButton.flag;
        } else if(IsButtonClicked(&pbcButton)) {
            *pbcButton.flag = !*pbcButton.flag;
        } else if(IsButtonClicked(&pauseButton)) {
            isPaused = !isPaused;
        } else if(isScreenLeftClicked(width, height)) {
            Vector2 rayPos = GetMousePosition();
            Vector pos = newVector(rayPos.x / width * sim->boxX, (1 - rayPos.y / height) * sim->boxY);
            externForce(sim,pos,-500);
        } else if(isScreenRightClicked(width, height)){
            Vector2 rayPos = GetMousePosition();
            Vector pos = newVector(rayPos.x / width * sim->boxX, (1 - rayPos.y / height) * sim->boxY);
            externForce(sim,pos,1000);
        }
         
        EndDrawing();
    }
    endGame(sim);
}

void endGame(Simulation* sim){
    CloseWindow();
    freeSimulation(sim);
}