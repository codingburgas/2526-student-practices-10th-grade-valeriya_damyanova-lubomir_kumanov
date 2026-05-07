#include <iostream>
#include "raylib.h"

#include "Menu.h"   
#include "BLL.h"

int main() {
    InitWindow(1520, 980, "Vanema");
    SetTargetFPS(60); 

    Menu menu;  

    while (!WindowShouldClose()) {

        menu.Update();

        BeginDrawing();
        menu.Draw();
        EndDrawing();
 
        if (menu.IsStartPressed()) {
            std::cout << "Start pressed\n";
        }

        if (menu.IsExitPressed()) {
            break;
        }
    }

    CloseWindow();
    return 0;
}