#ifndef LOGIN_H
#define LOGIN_H

#include "raylib.h"
#include "BLL/UserService.h"
#include <string>

class Login {
public:
    int* currentScreen;
    UserService userService{ "movies.db" };

    void Init();
    void Unload();
    void Update();
    void Draw();

    bool IsLoggedIn() const { return loggedIn; }
    std::string GetUserDisplayName() const { return userDisplayName; }

private:
    Texture2D background;
    Texture2D logo;
    Font headerFont;
    Font bodyFont;

    std::string username;
    std::string password;

    int activeField;
    bool loginError;
    bool openSignup;
    bool showErrorPopup;
    float errorPopupTimer;
    bool completed;

    bool loggedIn = false;
    std::string userDisplayName = "";
};

#endif