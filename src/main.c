#include "../include/Simulation.h"
#include "../raylib-5.0_linux_amd64/include/raylib.h"
#include <stdio.h>

int main(){

    InitWindow(800, 600, "Raylib Example");

    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(RAYWHITE);
        DrawText("Hello, Raylib!", 190, 200, 20, LIGHTGRAY);
        EndDrawing();
    }

    CloseWindow();

    return 0;
}