#include <iostream>
#include "raylib.h"
#include "Login.h"
#include "Menu.h"

enum Screen {
    MENU,
    LOGIN
};

int main()
{
    InitWindow(1600, 980, "Vanema");
    SetTargetFPS(60);

    Menu menu;
    Login login;

    login.Init();

    Screen currentScreen = MENU;

    while (!WindowShouldClose())
    {
        if (currentScreen == MENU)
        {
            menu.Update();

            if (menu.IsStartPressed())
            {
                currentScreen = LOGIN;
            }

            if (menu.IsExitPressed())
            {
                break;
            }
        }
        else if (currentScreen == LOGIN)
        {
            login.Update();
        }

        BeginDrawing();

        ClearBackground(WHITE);

        if (currentScreen == MENU)
        {
            menu.Draw();
        }
        else if (currentScreen == LOGIN)
        {
            login.Draw();
        }

        EndDrawing();
    }

    login.Unload();

    CloseWindow();

    return 0;
}