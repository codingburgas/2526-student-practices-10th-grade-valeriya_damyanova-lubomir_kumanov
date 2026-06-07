#ifndef LOGIN_H
#define LOGIN_H

#include "../BLL/UserService.h"
#include "raylib.h"
#include <string>

class Login {
public:
    void Init();
    void Unload();
    void Update();
    void Draw();

    int* currentScreen;

private:
    UserService userService;

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
};

#endif