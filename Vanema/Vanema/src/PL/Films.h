#ifndef FILMS_H
#define FILMS_H

#include "raylib.h"
#include <vector>
#include <string>
#include "BLL/MovieService.h"
#include "BLL/Movie.h"

// Forward declaration
class AddMovie;

struct Genre {
    std::string name;
};

struct DisplayMovie {
    std::string title;
    std::string genre;
    std::string rating;
    Texture2D posterTexture;
};

class Films {
public:
    // --- Public Members Accessed by Main Loop ---
    int* currentScreen;
    MovieService* movieService;
    AddMovie* addMovieScreen;
    bool hasSelectedMovieChanged;  // MOVED TO PUBLIC: Fixes main.cpp compiler access errors

    // --- Constructor & Destructor ---
    Films();
    ~Films();

    // --- Public Framework Methods ---
    void Update();
    void Draw();
    void SetUserData(bool loggedIn, const std::string& name, bool admin);
    void ConsumeMouseClicks();

    // --- Database Synchronization Interface ---
    void SyncDisplayWithDatabase(); // MOVED TO PUBLIC: Allows main.cpp to refresh movie lists directly

    // --- Public Getter & State Control Methods ---
    bool HasSelectedMovieChanged() const { return hasSelectedMovieChanged; }
    Movie GetLastClickedMovie() const { return lastClickedMovie; }
    void ResetSelectedMovieFlag() { hasSelectedMovieChanged = false; }
    void RefreshDisplay() { SyncDisplayWithDatabase(); }

private:
    // --- Graphic Assets & Textures ---
    Texture2D background;
    Texture2D logo;
    Texture2D iconHome;
    Texture2D iconMap;
    Texture2D iconFilms;
    Texture2D iconOffers;
    Texture2D iconProfile;
    Font customFont;

    // --- UI Layout & Component States ---
    std::vector<Genre> genres;
    std::vector<DisplayMovie> displayedMovies;
    std::vector<Movie> underlyingMovies;
    int activeIndex;
    float scrollOffset;
    float maxScroll;
    bool isLoggedIn;
    std::string userName;
    bool isAdmin;

    // --- Live Text Search Sub-System ---
    bool searchActive;
    int letterCount;
    char searchQuery[64];

    // --- Dynamic Genre Filter Bar State ---
    int selectedGenreIndex;
    int lastSelectedGenreIndex;
    float genreScrollX;
    float targetGenreScrollX;
    float maxGenreScrollWidth;

    // --- Record Erasure / Delete Modal Sub-System ---
    bool isDeleteMode;
    bool showDeleteConfirmation;
    Movie movieToDelete;

    // --- Layout Click Buffering & Debounce Control ---
    bool justActivated;
    int activationFrames;
    bool wasActive;

    // --- Async Refresh Tracking ---
    bool pendingRefresh;

    // --- Selected Item Storage ---
    Movie lastClickedMovie;

    // --- Internal Fragment Rendering Routines ---
    void Unload();
    void DrawMovieGrid(float startY);
    void DrawNavigationBar();
    void DrawScrollbar();
    void DrawGenreBar(float startY);
};

#endif // FILMS_H