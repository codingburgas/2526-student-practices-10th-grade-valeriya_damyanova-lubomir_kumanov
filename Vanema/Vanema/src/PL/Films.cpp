#include "Films.h"
#include <iostream>

Films::Films() {
    background = LoadTexture("assets/booking.png");
    logo = LoadTexture("assets/logo.png");

    iconHome = LoadTexture("assets/icon_home.png");
    iconMap = LoadTexture("assets/icon_map.png");
    iconFilms = LoadTexture("assets/icon_films.png");
    iconOffers = LoadTexture("assets/icon_offers.png");
    iconProfile = LoadTexture("assets/icon_profile.png");

    customFont = LoadFont("assets/fonts/PlayfairDisplay-Medium.ttf");

    currentScreen = nullptr;
    scrollOffset = 0.0f;
    maxScroll = 500.0f;
    activeIndex = 2; 
}

Films::~Films() {
    Unload();
}

void Films::Unload() {
    UnloadTexture(background);
    UnloadTexture(logo);
    UnloadTexture(iconHome);
    UnloadTexture(iconMap);
    UnloadTexture(iconFilms);
    UnloadTexture(iconOffers);
    UnloadTexture(iconProfile);
    UnloadFont(customFont);
}

void Films::Update() {
    float wheelMove = GetMouseWheelMove();
    if (wheelMove != 0) {
        scrollOffset -= wheelMove * 40;
        if (scrollOffset < 0) scrollOffset = 0;
        if (scrollOffset > maxScroll) scrollOffset = maxScroll;
    }
}

void Films::DrawNavigationBar() {
    int screenWidth = GetScreenWidth();
    float navWidth = screenWidth * 0.9f;
    float navHeight = 100.0f;

    Rectangle navBarRect = { (screenWidth - navWidth) / 2, 20, navWidth, navHeight };
    DrawRectangleRounded(navBarRect, 0.5f, 10, WHITE);

    if (logo.id != 0) {
        DrawTextureEx(logo, { navBarRect.x - 2, navBarRect.y - 20 }, 0.0f, 0.3f, WHITE);
    }

    if (customFont.texture.id != 0) {
        DrawTextEx(customFont, "Vanema", { navBarRect.x + 130, navBarRect.y + 40 }, 34, 1, BLACK);
    }

    const char* labels[] = { "Home", "Spots", "Films", "Offers" };
    Texture2D icons[] = { iconHome, iconMap, iconFilms, iconOffers };
    float spacing = 94.0f;
    float startX = navBarRect.x + 930;

    SetMouseCursor(MOUSE_CURSOR_DEFAULT);
    Vector2 mousePos = GetMousePosition();

    for (int i = 0; i < 4; i++) {
        float itemX = startX + (i * spacing);
        Rectangle btnRect = { itemX, navBarRect.y, 110, navHeight };
        bool isHovered = CheckCollisionPointRec(mousePos, btnRect);
        Color tint = (i == activeIndex) ? BLUE : DARKBLUE;

        if (isHovered) {
            SetMouseCursor(MOUSE_CURSOR_POINTING_HAND);
            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && currentScreen != nullptr) {
                if (i == 0) {
                    *currentScreen = 2; 
                }
            }
        }

        if (icons[i].id != 0) {
            DrawTextureEx(icons[i], { itemX + 22, navBarRect.y + 5 }, 0.0f, 0.1f, tint);
        }

        if (customFont.texture.id != 0) {
            DrawTextEx(customFont, labels[i], { itemX + 30, navBarRect.y + 70 }, 20, 1, BLACK);
        }
    }

    Rectangle profileRect = { navBarRect.x + navWidth - 70, navBarRect.y + 15, 50, 50 };
    bool isProfileHovered = CheckCollisionPointRec(mousePos, profileRect);

    if (isProfileHovered) {
        SetMouseCursor(MOUSE_CURSOR_POINTING_HAND);
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && currentScreen != nullptr) {
            *currentScreen = 1; 
        }
    }

    if (iconProfile.id != 0) {
        DrawTextureEx(iconProfile, { profileRect.x - 15, profileRect.y + 10 }, 0.0f, 0.1f, isProfileHovered ? BLUE : DARKBLUE);
    }
}

void Films::DrawScrollbar() {
    int screenWidth = GetScreenWidth();
    float scrollbarHeight = 350;
    float scrollbarWidth = 10;
    float scrollbarX = screenWidth - 25;
    float scrollbarY = 150;

    DrawRectangleRounded({ scrollbarX, scrollbarY, scrollbarWidth, scrollbarHeight }, 0.5f, 8, Fade(LIGHTGRAY, 0.4f));

    float thumbHeight = 80;
    float thumbY = scrollbarY;

    if (maxScroll > 0) {
        thumbY = scrollbarY + (scrollOffset / maxScroll) * (scrollbarHeight - thumbHeight);
    }

    DrawRectangleRounded({ scrollbarX, thumbY, scrollbarWidth, thumbHeight }, 0.5f, 8, DARKBLUE);
}

void Films::Draw() {
    if (background.id != 0) {
        DrawTexturePro(
            background,
            { 0, 0, (float)background.width, (float)background.height },
            { 0, 0, (float)GetScreenWidth(), (float)GetScreenHeight() },
            { 0, 0 }, 0.0f, WHITE
        );
    }
    else {
        ClearBackground(RAYWHITE);
    }

    if (customFont.texture.id != 0) {
        DrawTextEx(customFont, "Films", { 120, 150 - scrollOffset }, 45, 1, BLACK);
        DrawTextEx(customFont, "Discover by genre, trending and more", { 120, 200 - scrollOffset }, 30, 1, BLACK);
    }

    DrawScrollbar();
    DrawNavigationBar();
}