#include <iostream>
#include "raylib.h"
#include "Login.h"
#include "Menu.h"
#include "Booking.h"

enum Screen {
    MENU,
    LOGIN,
    BOOKING
};

int main()
{
    InitWindow(1600, 980, "Vanema");
    SetTargetFPS(60);

    Menu menu;
    Login login;
    Booking booking;

    login.Init();

    Screen currentScreen = MENU;

    while (!WindowShouldClose())
    {
        if (currentScreen == MENU)
        {
            menu.Update();

            if (menu.IsStartPressed())
            {
                currentScreen = BOOKING;
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
        else if (currentScreen == BOOKING)
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
