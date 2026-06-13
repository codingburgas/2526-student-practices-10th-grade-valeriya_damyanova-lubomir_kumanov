#pragma once
#include "raylib.h"
#include "BLL/MovieService.h"
#include "BLL/Movie.h"
#include <vector>
#include <string>

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

    std::vector<Movie> currentSuggestedMovies;
    float scrollOffset;
    float maxScroll;
    int activeIndex;
    int mouseScrollAccumulator;
    bool isLoggedIn = false;
    std::string userName = "";
    bool isAdmin = false;

public:
    int* currentScreen;

    MovieService* movieService = nullptr;

    Booking();
    ~Booking();

    void loadRandomSuggestions();
    void RefreshSuggestions();
    void SetScreenPointer(int* screen);
    void SetUserData(bool loggedIn, const std::string& name, bool admin);
    void Unload();
    void Update();
    void DrawNavigationBar();
    void DrawMoviePosters();
    void DrawScrollbar();
    void Draw();
};