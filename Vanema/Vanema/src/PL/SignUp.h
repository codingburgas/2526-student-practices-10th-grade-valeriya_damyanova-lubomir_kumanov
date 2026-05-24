#ifndef SIGNUP_H
#define SIGNUP_H

#include "raylib.h"
#include <string>

class SignUp {
public:
    void Init();
    void Unload();
    void Update();
    void Draw();

    int* currentScreen;  

private:
    Texture2D background;
    Texture2D logo;
    Font headerFont;
    Font bodyFont;

    std::string name;
    std::string email;
    std::string username;
    std::string password;

    int activeField;
    bool showErrorPopup;
    bool showSuccessPopup;
    float errorPopupTimer;
    float successPopupTimer;
    std::string errorMessage;
};

#endif