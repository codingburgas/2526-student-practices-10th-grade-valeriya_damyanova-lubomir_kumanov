#include <iostream>
#include "raylib.h"


#include "BLL.h"

int main() {
    InitWindow(800, 450, "Vanema");
    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(RAYWHITE);
        DrawText("IT WORKS!", 190, 200, 20, LIGHTGRAY);
        EndDrawing();
    }
    CloseWindow();
    return 0;
}