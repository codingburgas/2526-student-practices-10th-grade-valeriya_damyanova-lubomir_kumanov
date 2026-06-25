#ifndef SPOTS_H
#define SPOTS_H

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

struct ExperienceData {
    std::string title;
    std::string description;
    std::string tag1;
    std::string tag2;
    std::string tag3;
};

class Spots {
public:
    Spots();
    ~Spots();

    void InitializeData();
    void SetScreenPointer(int* screen);
    void SetUserData(bool loggedIn, const std::string& name, bool isAdmin);
    void Unload();
    void Update();
    void Draw();
    void AddNewExperienceData(const std::string& title, const std::string& desc, const std::string& t1, const std::string& t2, const std::string& t3);

    int* currentScreen;

private:
    void DrawNavigationBar();
    void DrawSearchBarAndFilters();
    void DrawCinemaCards(float& currentY, Vector2 mousePos);
    void DrawExperiences(float& currentY);
    void DrawScrollbar();

    Texture2D logo;
    Texture2D iconHome;
    Texture2D iconMap;
    Texture2D iconFilms;
    Texture2D iconOffers;
    Texture2D iconProfile;
    Texture2D iconLocationMarker;
    Texture2D iconHalls;
    Texture2D icon3D;
    Texture2D iconAudio;
    Texture2D iconBeach;
    Texture2D iconPool;
    Texture2D iconSofa;

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
    std::vector<ExperienceData> experiences;
};

#endif