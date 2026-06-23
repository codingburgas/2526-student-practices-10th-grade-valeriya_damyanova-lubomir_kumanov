#pragma once
#include "raylib.h"
#include "BLL/MovieService.h"
#include "BLL/Movie.h"
#include <vector>
#include <string>

class Booking
{
private:
    // --- Original UI and Asset Design Layout Assets ---
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

    // --- Scrolling and Grid State Layout Properties ---
    std::vector<Movie> currentSuggestedMovies;
    std::vector<Movie> topRatedMovies;
    float scrollOffset;
    float maxScroll;
    int activeIndex;
    int mouseScrollAccumulator;

    // --- User Session States ---
    bool isLoggedIn = false;
    std::string userName = "";
    bool isAdmin = false;

    // --- Integrated Admin Deletion Logic States ---
    bool isDeleteMode = false;
    bool showDeleteConfirmation = false;
    Movie movieToDelete;

    // --- Private Methods ---
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