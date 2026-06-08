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
    searchActive = false;
    letterCount = 0;
    searchQuery[0] = '\0';
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

    Vector2 mousePos = GetMousePosition();

    Rectangle searchBox = { 1150, 205 - scrollOffset, 260, 50 };

    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
    {
        if (CheckCollisionPointRec(mousePos, searchBox))
        {
            searchActive = true;
        }
        else
        {
            if (mousePos.y > 140) {
                searchActive = false;
            }
        }
    }

    if (searchActive) {
        SetMouseCursor(MOUSE_CURSOR_IBEAM);

        int key = GetCharPressed();
        while (key > 0) {
            if ((key >= 32) && (key <= 125) && (letterCount < 63)) {
                searchQuery[letterCount] = (char)key;
                searchQuery[letterCount + 1] = '\0';
                letterCount++;
            }
            key = GetCharPressed();
        }

        if (IsKeyPressed(KEY_BACKSPACE)) {
            letterCount--;
            if (letterCount < 0) letterCount = 0;
            searchQuery[letterCount] = '\0';
        }
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
                    searchActive = false;
                    *currentScreen = 2; 
                    printf("Films -> Switching to Booking screen\n");
                    return;
                }
                else if (i == 2) {
                    *currentScreen = 4; 
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
            bool isUserLoggedIn = false; 

            searchActive = false;
            if (!isUserLoggedIn) {
                *currentScreen = 1; 
                printf("Films -> Switching to Login screen\n");
            }
            else {
                *currentScreen = 2; 
                printf("Films -> Active session found, heading to Booking dashboard\n");
            }
            return;
        }
    }

    if (iconProfile.id != 0) {
        DrawTextureEx(iconProfile, { profileRect.x - 15, profileRect.y + 10 }, 0.0f, 0.1f, isProfileHovered ? BLUE : DARKBLUE);
    }
}

void Films::DrawScrollbar()
{
    int screenWidth = GetScreenWidth();

    float scrollbarHeight = 350;
    float scrollbarWidth = 10;

    float scrollbarX = screenWidth - 25;
    float scrollbarY = 150;

    DrawRectangleRounded(
        { scrollbarX, scrollbarY, scrollbarWidth, scrollbarHeight },
        0.5f, 8, Fade(LIGHTGRAY, 0.4f)
    );

    float thumbHeight = 80;
    float thumbY = scrollbarY;

    if (maxScroll > 0)
    {
        thumbY = scrollbarY + (scrollOffset / maxScroll) * (scrollbarHeight - thumbHeight);
    }

    DrawRectangleRounded(
        { scrollbarX, thumbY, scrollbarWidth, thumbHeight },
        0.5f, 8, DARKBLUE
    );
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

    int contentTopY = 140;
    int contentHeight = GetScreenHeight() - contentTopY;

    BeginScissorMode(0, contentTopY, GetScreenWidth(), contentHeight);

    if (customFont.texture.id != 0)
    {
        DrawTextEx(customFont, "Films", { 120, 160 - scrollOffset }, 45, 1, BLACK);
        DrawTextEx(customFont, "Discover by genre, trending and more", { 120, 210 - scrollOffset }, 30, 1, BLACK);
    }

    Rectangle searchBox = { 1150, 205 - scrollOffset, 260, 50 };

    DrawRectangleRounded(searchBox, 0.3f, 6, searchActive ? WHITE : Fade(WHITE, 0.7f));
    DrawRectangleRoundedLines(searchBox, 0.3f, 6, 2, searchActive ? BLUE : WHITE);

    if (letterCount == 0 && !searchActive)
    {
        DrawText("Search films...", searchBox.x + 15, searchBox.y + 15, 20, GRAY);
    }
    else
    {
        DrawText(searchQuery, searchBox.x + 15, searchBox.y + 15, 20, BLACK);
    }

    if (searchActive) {
        if (((int)(GetTime() * 2) % 2) == 0) {
            int textWidth = MeasureText(searchQuery, 20);
            DrawRectangle(searchBox.x + 15 + textWidth + 2, searchBox.y + 15, 2, 20, BLACK);
        }
    }

    EndScissorMode();
    DrawScrollbar();
    DrawNavigationBar();
}