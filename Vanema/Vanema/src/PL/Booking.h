#ifndef BOOKING_H
#define BOOKING_H

#include "raylib.h"

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
    Font customFont;
    int activeIndex = 0;

public:
    Booking();
    ~Booking();
    void Unload();
    void Update();
    void Draw();
    int* currentScreen = nullptr;  // Pointer to current screen variable
};

#endif