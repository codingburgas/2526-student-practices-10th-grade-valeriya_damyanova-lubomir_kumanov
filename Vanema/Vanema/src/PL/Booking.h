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
    Texture2D topRatedPhotos[4];
    Font customFont;

    std::vector<Movie> currentSuggestedMovies;
    std::vector<Movie> topRatedMovies;
    float scrollOffset;
    float maxScroll;
    int activeIndex;
    int mouseScrollAccumulator;

    bool isLoggedIn = false;
    std::string userName = "";
    bool isAdmin = false;

    bool isDeleteMode = false;
    bool showDeleteConfirmation = false;
    Movie movieToDelete;

    void ProcessMoviePosterClicks(Vector2 mousePos);

public:
    int* currentScreen;
    MovieService* movieService = nullptr;

    Movie lastClickedMovie;
    bool hasSelectedMovieChanged = false;

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

    Movie GetLastClickedMovie() {
        hasSelectedMovieChanged = false;
        return lastClickedMovie;
    }
};