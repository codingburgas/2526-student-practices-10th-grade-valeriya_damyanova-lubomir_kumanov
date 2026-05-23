#pragma once
#include "raylib.h"
#include <vector>
#include <string>

class Booking
{
public:
    Booking();
    ~Booking();

    void Unload();
    void Update(); 
    void Draw();

private:
    Texture2D background;
    Texture2D logo;
    Texture2D iconHome, iconMap, iconFilms, iconOffers, iconProfile;
    Font customFont;

    int activeIndex = 0;
};