#ifndef ADDSPOTS_H
#define ADDSPOTS_H

#include "raylib.h"
#include <string>

class AddSpots {
public:
    AddSpots();
    ~AddSpots();

    void SetScreenPointer(int* screen);
    void SetUserData(bool loggedIn, const std::string& name, bool admin);
    void Unload();
    void Update();
    void Draw();
    void ResetForm();

    bool shouldRefreshExperiences;
    std::string titleInput;
    std::string descInput;
    std::string tag1Input;
    std::string tag2Input;
    std::string tag3Input;

private:
    Texture2D background;
    Texture2D logo;
    Texture2D iconHome;
    Texture2D iconMap;
    Texture2D iconFilms;
    Texture2D iconOffers;
    Texture2D iconProfile;
    Font customFont;

    int* currentScreen;
    int activeIndex;
    bool isLoggedIn;
    std::string userName;
    bool isAdmin;

    float scrollYOffset;
    float maxScrollY;

    bool isTransitioning;
    int transitionFrames;

    int activeField;
};

#endif