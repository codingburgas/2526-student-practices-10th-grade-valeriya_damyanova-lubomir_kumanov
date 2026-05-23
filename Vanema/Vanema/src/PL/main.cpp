#include <iostream>
#include "raylib.h"
#include "Login.h"
#include "Menu.h"
#include "Booking.h"

int main()
{
    InitWindow(1600, 980, "Vanema");
    SetTargetFPS(60);

    Menu menu;
    Login login;
    Booking booking;

    int currentScreen = 0;

    booking.currentScreen = &currentScreen;

    login.Init();

    while (!WindowShouldClose())
    {
        if (currentScreen == 0)
        {
            menu.Update();

            if (menu.IsStartPressed())
                currentScreen = 2;

            if (menu.IsExitPressed())
                break;
        }
        else if (currentScreen == 1)
        {
            login.Update();
        }
        else if (currentScreen == 2)
        {
            booking.Update();
        }

        BeginDrawing();
        ClearBackground(WHITE);

        if (currentScreen == 0)
        {
            menu.Draw();
        }
        else if (currentScreen == 1)
        {
            login.Draw();
        }
        else if (currentScreen == 2)
        {
            booking.Draw();
        }

        EndDrawing();
    }

    booking.Unload();
    login.Unload();

    CloseWindow();

    return 0;
}