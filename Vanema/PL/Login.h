#pragma once
#include "raylib.h"
#include <string>

class Login
{
private:
    Texture2D background;
    Texture2D logo;
    Font headerFont;
    Font bodyFont;

    std::string username;
    std::string password;

    int activeField = -1;

    bool loginError = false;
    bool openSignup = false;
    bool showErrorPopup = false;
    float errorPopupTimer = 0.0f;

public:
    bool completed = false;

    void Init();
    void Unload();
    void Update();
    void Draw();

    bool ShouldOpenSignup() const { return openSignup; }
    void ResetOpenSignup() { openSignup = false; }
};