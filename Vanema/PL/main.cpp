#include <iostream>
#include "raylib.h"
#include "Login.h"
#include "Menu.h"

int main() {
    InitWindow(1600, 980, "Vanema");
    SetTargetFPS(60);

    Menu menu;
    Login login;
    bool showLogin = false;

    login.Init();

    while (!WindowShouldClose()) {

        if (!showLogin) {
            
            menu.Update();

            if (menu.IsStartPressed()) {
                showLogin = true;  
                login.Reset();     
            }

            if (menu.IsExitPressed()) {
                break;
            }
        }
        else {
            
            login.Update();

            if (login.IsCompleted()) {
                
                std::cout << "Login successful!" << std::endl;
                showLogin = false;  
                login.Reset();
            }

            if (IsKeyPressed(KEY_ESCAPE)) {
                showLogin = false;
                login.Reset();
            }
        }

        BeginDrawing();

        if (!showLogin) {
            menu.Draw();
        }
        else {
            login.Draw();
        }

        EndDrawing();
    }

    login.Unload();
    CloseWindow();
    return 0;
}