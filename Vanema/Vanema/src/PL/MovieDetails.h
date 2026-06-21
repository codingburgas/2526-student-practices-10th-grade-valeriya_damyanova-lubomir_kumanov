#pragma once
#include "Movie.h"
#include "raylib.h"
#include <string>

enum class DetailScreenResult { NONE, BACK, BOOKING };

class MovieDetails {
private:
    Movie currentMovie;
    Texture2D posterTexture;
    bool textureLoaded;

    // Design layout assets
    Font customFont;
    Texture2D background;
    Texture2D logo;
    Texture2D iconHome, iconMap, iconFilms, iconOffers, iconProfile;

    // User profile state trackers
    bool isLoggedIn;
    std::string userName;

    // Interactive Calendar & Showtimes states
    int selectedShowtimeIndex;
    bool calendarDropdownOpen;
    const char* selectedDateText;

    // Dynamic paragraph text-wrapping utility
    static void DrawTextTextBoxed(Font font, const char* text, Rectangle rec, float fontSize, float spacing, bool wordWrap, Color selectColor);

public:
    int* currentScreen = nullptr;

    MovieDetails();
    ~MovieDetails();

    void SetUserData(bool loggedIn, const std::string& name);
    void LoadMovie(const Movie& movie);
    void UnloadCurrentPoster();

    DetailScreenResult Update();
    void Draw();
};