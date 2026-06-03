#pragma once
#include "raylib.h"

class Films {
public:
    int* currentScreen;

    // UI Assets
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

    Films();
    ~Films();

    void Init();
    void Update();
    void Draw();
    void Unload();

private:
    void DrawNavigationBar();
    void DrawScrollbar();
};