#include <iostream>
#include "raylib.h"
#include "Login.h"
#include "Menu.h"
#include "Booking.h"
#include "SignUp.h"
#include "Films.h"
#include "Spots.h"
#include "Offers.h"
#include "MovieDetails.h" // <--- 1. INCLUDE MOVIE DETAILS
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
    Spots spots;
    Offers offers;
    MovieDetails movieDetails; // <--- 2. INSTANTIATE THE STATE INSTANCE

    booking.movieService = &movieService;
    films.movieService = &movieService;

    booking.loadRandomSuggestions();
    films.SyncDisplayWithDatabase();

    int currentScreen = 0;

    login.currentScreen = &currentScreen;
    signup.currentScreen = &currentScreen;
    booking.currentScreen = &currentScreen;
    films.currentScreen = &currentScreen;
    spots.currentScreen = &currentScreen;
    offers.currentScreen = &currentScreen;
    movieDetails.currentScreen = &currentScreen; 

    login.Init();
    signup.Init();

    while (!WindowShouldClose())
    {
        // --- 4. CHECK IF A MOVIE WAS CLICKED FROM THE GRID LISTS ---
        if (booking.hasSelectedMovieChanged)
        {
            movieDetails.LoadMovie(booking.GetLastClickedMovie());
        }
        else if (films.hasSelectedMovieChanged)
        {
            movieDetails.LoadMovie(films.GetLastClickedMovie());
        }

        // --- SCREEN STATE UPDATES ---
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
        else if (currentScreen == 5)
        {
            spots.Update();
        }
        else if (currentScreen == 6)
        {
            offers.Update();
        }
        else if (currentScreen == 7) // <--- 5. UPDATE STATE FOR DETAILED VIEW
        {
            movieDetails.Update();
        }

        if (login.IsLoggedIn())
        {
            std::string currentRealName = login.GetUserDisplayName();
            bool isAdminUser = login.IsAdmin();
            films.SetUserData(true, currentRealName, isAdminUser);
            booking.SetUserData(true, currentRealName, isAdminUser);
            spots.SetUserData(true, currentRealName, isAdminUser);
            offers.SetUserData(true, currentRealName, isAdminUser);
        }

        // --- DRAWING PIPELINE ---
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
        else if (currentScreen == 5)
        {
            spots.Draw();
        }
        else if (currentScreen == 6)
        {
            offers.Draw();
        }
        else if (currentScreen == 7) 
        {
            movieDetails.Draw();
        }

        EndDrawing();
    }

    CloseWindow();
    return 0;
}