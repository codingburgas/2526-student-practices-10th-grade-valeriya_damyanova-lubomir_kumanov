#include <iostream>
#include "raylib.h"
#include "Login.h"
#include "Menu.h"
#include "Booking.h"
#include "SignUp.h"  

int main()
{
    InitWindow(1600, 980, "Vanema");
    SetTargetFPS(60);

    Menu menu;
    Login login;
    Booking booking;
    SignUp signup;  
    int currentScreen = 0;

    std::cout << "currentScreen = " << currentScreen << std::endl;

    login.currentScreen = &currentScreen;
    signup.currentScreen = &currentScreen;
    booking.currentScreen = &currentScreen;

    login.Init();
    signup.Init();

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
        else if (currentScreen == 3)  
        {
            signup.Update();
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
        else if (currentScreen == 3)
        {
            signup.Draw();
        }

        EndDrawing();
    }

    booking.Unload();
    login.Unload();
    signup.Unload();

    CloseWindow();

    return 0;
}