#ifndef FILMS_H
#define FILMS_H

#include "raylib.h"
#include <string>
#include <vector>
#include <memory>
#include "BLL/Movie.h"
#include "BLL/MovieService.h"

struct GenreItem {
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
    Films();
    ~Films();

    void Update();
    void Draw();
    void Unload();

    void SetUserData(bool loggedIn, const std::string& name, bool admin);
    void SyncDisplayWithDatabase();

    Movie GetLastClickedMovie() const { return lastClickedMovie; }

    int* currentScreen;
    MovieService* movieService;

public:
    bool hasSelectedMovieChanged;

private:
    void DrawNavigationBar();
    void DrawMovieGrid(float startY);
    void DrawGenreBar(float startY);
    void DrawScrollbar();
    void ConsumeMouseClicks();

    Texture2D background;
    Texture2D logo;
    Texture2D iconHome;
    Texture2D iconMap;
    Texture2D iconFilms;
    Texture2D iconOffers;
    Texture2D iconProfile;
    Font customFont;

    std::vector<GenreItem> genres;
    std::vector<DisplayMovie> displayedMovies;
    std::vector<Movie> underlyingMovies;

    float scrollOffset;
    float maxScroll;
    int activeIndex;
    bool searchActive;
    int letterCount;
    char searchQuery[64];

    int selectedGenreIndex;
    int lastSelectedGenreIndex;

    float genreScrollX;
    float targetGenreScrollX;
    float maxGenreScrollWidth;

    bool isLoggedIn;
    std::string userName;
    bool isAdmin;

    bool isDeleteMode;
    bool showDeleteConfirmation;
    Movie movieToDelete;
    Movie lastClickedMovie;

    // Click prevention members
    bool justActivated;
    int activationFrames;
    bool wasActive;  // <-- Add this member
};

#endif