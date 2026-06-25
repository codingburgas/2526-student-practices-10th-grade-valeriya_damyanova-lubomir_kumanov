#ifndef FILMS_H
#define FILMS_H

#include "Movie.h"
#include "MovieService.h"
#include "raylib.h"
#include <string>
#include <vector>

class AddMovie;

class Films { 
public:
    Films();
    ~Films();

    void Update(); 
    void Draw();   
    void Unload(); 

    void SetUserData(bool loggedIn, const std::string& name, bool admin);
    void SetMovieService(MovieService* service) { movieService = service; }
    void SetCurrentScreenPointer(int* screenPtr) { currentScreen = screenPtr; }
    void SetAddMovieScreen(AddMovie* addMovie) { addMovieScreen = addMovie; }
    Movie GetLastClickedMovie() const { return lastClickedMovie; }
    bool HasSelectedMovieChanged() const { return hasSelectedMovieChanged; }
    void ResetSelectionState();
    void ConsumeMouseClicks();

private:
    struct DisplayMovie {
        std::string title;
        std::string genre;
        std::string rating;
        Texture2D posterTexture;
    };

    struct Genre {
        std::string name;
    };

    Texture2D background;
    Texture2D logo;
    Texture2D iconHome;
    Texture2D iconMap;
    Texture2D iconFilms;
    Texture2D iconOffers;
    Texture2D iconProfile;
    Font customFont;

    std::vector<Genre> genres;
    std::vector<DisplayMovie> displayedMovies;
    std::vector<Movie> underlyingMovies;
    MovieService* movieService;
    AddMovie* addMovieScreen;
    int* currentScreen;

    // UI State
    float scrollOffset;
    float maxScroll;
    int activeIndex;
    bool searchActive;
    char searchQuery[64];
    int letterCount;
    int selectedGenreIndex;
    int lastSelectedGenreIndex;

    float genreScrollX;
    float targetGenreScrollX;
    float maxGenreScrollWidth;

    bool isLoggedIn;
    std::string userName;
    bool isAdmin;

    Movie lastClickedMovie;
    bool hasSelectedMovieChanged;

    bool isDeleteMode;
    bool showDeleteConfirmation;
    Movie movieToDelete;

    bool justActivated;
    int activationFrames;
    bool wasActive;
    bool pendingRefresh;
    int lastScreen;
    bool isTransitioning;
    int clickCooldown;

    void SyncDisplayWithDatabase();
    void DrawMovieGrid(float startY);
    void DrawNavigationBar();
    void DrawGenreBar(float startY);
    void DrawScrollbar();
};

#endif 