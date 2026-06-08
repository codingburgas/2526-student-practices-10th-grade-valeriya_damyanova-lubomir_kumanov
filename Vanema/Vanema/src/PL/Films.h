#pragma once
#include "raylib.h"
#include <vector>
#include <string>

struct GenreItem {
    std::string name;
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

    Films();
    ~Films();

    void Init();
    void Update();
    void Draw();
    void Unload();
    void DrawGenreBar(float startY);

private:
    std::vector<GenreItem> genres;
    int selectedGenreIndex;

    float genreScrollX;
    float targetGenreScrollX;
    float maxGenreScrollWidth;

    void DrawNavigationBar();
    void DrawScrollbar();
};