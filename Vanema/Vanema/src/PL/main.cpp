#include <iostream>
#include "raylib.h"
#include "Login.h"
#include "Menu.h"
#include "Booking.h"
#include "SignUp.h"
#include "Films.h"

#include "DAL/MovieRepository.h"
#include "DAL/DataSeeder.h"
#include "BLL/MovieService.h"

int main()
{
    InitWindow(1600, 980, "Vanema");
    SetTargetFPS(60);

    MovieRepository movieRepo("movies.db");
    DataSeeder::SeedIfEmpty(movieRepo);

    MovieService movieService("movies.db");

    Menu menu;
    Login login;
    Booking booking;
    SignUp signup;
    Films films;

    booking.movieService = &movieService;
    films.movieService = &movieService;

    int currentScreen = 0;

    login.currentScreen = &currentScreen;
    signup.currentScreen = &currentScreen;
    booking.currentScreen = &currentScreen;
    films.currentScreen = &currentScreen;

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
        else if (currentScreen == 4)
        {
            films.Update();
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
        else if (currentScreen == 4)
        {
            films.Draw();
        }

        EndDrawing();
    }

    CloseWindow();
    return 0;
}