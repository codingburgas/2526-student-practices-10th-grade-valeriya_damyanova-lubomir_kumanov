#pragma once
#include "raylib.h"
#include <string>

class Login
{
private:
    Texture2D background;
    Texture2D logo;

    std::string username;
    std::string password;

    int activeField;
    bool loginError;
    bool openSignup;
    bool showErrorPopup;
    float errorPopupTimer;
    bool completed;

public:
    void Init();
    void Unload();
    void Update();
    void Draw();
    bool IsCompleted() const;
    bool ShouldOpenSignup() const;
    void Reset();
};