#pragma once
#include "raylib.h"
#include "../BLL/MovieService.h"
#include <vector>

class Booking
{
private:
    // Textures
    Texture2D background;
    Texture2D logo;
    Texture2D iconHome;
    Texture2D iconMap;
    Texture2D iconFilms;
    Texture2D iconOffers;
    Texture2D iconProfile;
    Texture2D suggestedPhotos[4];

    // Font
    Font customFont;

    // Business Logic
    MovieService movieService;
    std::vector<Movie> currentSuggestedMovies;

    // UI State
    float scrollOffset;
    float maxScroll;
    float mouseScrollAccumulator;

    // Private methods
    void loadRandomSuggestions();
    void Unload();
    void DrawNavigationBar();
    void DrawMoviePosters();
    void DrawScrollbar();

public:
    // Public members (for screen management)
    int* currentScreen;
    int activeIndex;

    // Constructor & Destructor
    Booking();
    ~Booking();

    // Public methods
    void Update();
    void Draw();
    void RefreshSuggestions();
    void SetScreenPointer(int* screen);
};