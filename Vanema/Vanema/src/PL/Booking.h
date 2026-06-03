#pragma once
#include "raylib.h"
#include "../BLL/MovieService.h"
#include <vector>

class Booking
{
private:
    Texture2D background;
    Texture2D logo;
    Texture2D iconHome;
    Texture2D iconMap;
    Texture2D iconFilms;
    Texture2D iconOffers;
    Texture2D iconProfile;
    Texture2D suggestedPhotos[4];

    Font customFont;

    MovieService movieService;
    std::vector<Movie> currentSuggestedMovies;

    float scrollOffset;
    float maxScroll;
    float mouseScrollAccumulator;

    void loadRandomSuggestions();
    void Unload();
    void DrawNavigationBar();
    void DrawMoviePosters();
    void DrawScrollbar();

public:
    int* currentScreen;
    int activeIndex;

    Booking();
    ~Booking();

    void Update();
    void Draw();
    void RefreshSuggestions();
    void SetScreenPointer(int* screen);
};