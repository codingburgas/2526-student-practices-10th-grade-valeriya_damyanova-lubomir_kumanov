#pragma once

#include "raylib.h"
#include <vector>
#include <string>
#include "../BLL/MovieService.h" 
#include "../BLL/Movie.h"

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
    int* currentScreen;

    Texture2D background;
    Texture2D logo;
    Texture2D iconHome;
    Texture2D iconMap;
    Texture2D iconFilms;
    Texture2D iconOffers;
    Texture2D iconProfile;
    Font customFont;

    float scrollOffset;
    float maxScroll;
    int activeIndex;

    char searchQuery[64] = "\0";
    int letterCount = 0;
    bool searchActive = false;

    MovieService* movieService = nullptr;

    Films();
    ~Films();

    void Init();
    void Update();
    void Draw();
    void Unload();
    void DrawGenreBar(float startY);
    void DrawMovieGrid(float startY);
    void SyncDisplayWithDatabase();
    void SetUserData(bool loggedIn, const std::string& name, bool admin);

private:
    std::vector<GenreItem> genres;
    int selectedGenreIndex;
    int lastSelectedGenreIndex;

    float genreScrollX;
    float targetGenreScrollX;
    float maxGenreScrollWidth;

    bool isLoggedIn = false;
    std::string userName = "";
    bool isAdmin;

    std::vector<DisplayMovie> displayedMovies;

    void DrawNavigationBar();
    void DrawScrollbar();
};