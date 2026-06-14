#pragma once
#include "raylib.h"
#include <string>
#include <vector>

struct CinemaLocation {
    std::string name;
    std::string address;
    int halls;
    bool has3D;
    bool hasDolby;
};

struct CinemaExperience {
    std::string title;
    std::string description;
    std::string tag1;
    std::string tag2;
    std::string tag3;
};

class Spots {
private:
    Texture2D logo;
    Texture2D iconHome, iconMap, iconFilms, iconOffers, iconProfile;
    Texture2D iconLocationMarker;
    Texture2D iconSearch, iconFilter;
    Texture2D iconHalls, icon3D, iconAudio;
    Texture2D iconBeach, iconPool, iconSofa;
    Font uiFont;

    float scrollOffset;
    float maxScroll;
    int activeIndex; 
    int activeCityIndex;
    char searchBuffer[64];
    int searchLetterCount;

    bool isLoggedIn;
    bool isAdmin;
    std::string userName;

    std::vector<std::string> cities;
    std::vector<CinemaLocation> locations;
    std::vector<CinemaExperience> experiences;

public:
    int* currentScreen;

    Spots();
    ~Spots();

    void InitializeData();
    void SetScreenPointer(int* screen);
    void SetUserData(bool loggedIn, const std::string& name, bool isAdmin);
    void Unload();
    void Update();
    void DrawNavigationBar();
    void DrawSearchBarAndFilters();
    void DrawCinemaCards(float& currentY, Vector2 mousePos);
    void DrawExperiences(float& currentY);
    void DrawScrollbar();
    void Draw();
};