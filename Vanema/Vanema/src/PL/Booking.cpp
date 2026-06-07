#include "Booking.h"
#include <stdio.h>
#include <stdarg.h>
#include <fstream>
#include <iostream>

#ifdef _WIN32
#include <direct.h>
#define getcwd _getcwd
#else
#include <unistd.h>
#endif

Booking::Booking()
{
    char cwd[1024];
    if (getcwd(cwd, sizeof(cwd)) != NULL)
    {
        printf("Current working directory: %s\n", cwd);
    }

    background = LoadTexture("assets/booking.png");
    if (background.id == 0) printf("Failed to load: assets/booking.png\n");

    logo = LoadTexture("assets/logo.png");
    if (logo.id == 0) printf("Failed to load: assets/logo.png\n");

    iconHome = LoadTexture("assets/icon_home.png");
    iconMap = LoadTexture("assets/icon_map.png");
    iconFilms = LoadTexture("assets/icon_films.png");
    iconOffers = LoadTexture("assets/icon_offers.png");
    iconProfile = LoadTexture("assets/icon_profile.png");

    if (iconHome.id == 0) printf("Failed to load: assets/icon_home.png\n");
    if (iconMap.id == 0) printf("Failed to load: assets/icon_map.png\n");
    if (iconFilms.id == 0) printf("Failed to load: assets/icon_films.png\n");
    if (iconOffers.id == 0) printf("Failed to load: assets/icon_offers.png\n");
    if (iconProfile.id == 0) printf("Failed to load: assets/icon_profile.png\n");

    for (int i = 0; i < 4; i++)
    {
        suggestedPhotos[i].id = 0;
    }

    customFont = LoadFont("assets/fonts/PlayfairDisplay-Medium.ttf");
    if (customFont.texture.id == 0) printf("Failed to load font\n");

    loadRandomSuggestions();

    scrollOffset = 0;
    maxScroll = 500;
    activeIndex = 0;
    currentScreen = nullptr;
    mouseScrollAccumulator = 0;

    printf("Booking initialized successfully!\n");
}

Booking::~Booking()
{
    Unload();
}

void Booking::loadRandomSuggestions()
{
    currentSuggestedMovies = movieService.getRandomMovies(4);

    if (currentSuggestedMovies.empty())
    {
        printf("No movies loaded from service!\n");
        return;
    }

    printf("Loading %zu movies...\n", currentSuggestedMovies.size());

    for (size_t i = 0; i < currentSuggestedMovies.size() && i < 4; i++)
    {
        std::string rawPath = currentSuggestedMovies[i].getPosterPath();
        std::string posterPath = rawPath;

        std::ifstream fileCheck(posterPath);
        if (!fileCheck.good())
        {
            posterPath = "Vanema/" + rawPath;
            std::ifstream nestedCheck(posterPath);
            if (!nestedCheck.good())
            {
                posterPath = "../" + rawPath;
            }
        }

        std::ifstream finalCheck(posterPath);
        if (finalCheck.good())
        {
            suggestedPhotos[i] = LoadTexture(posterPath.c_str());
            if (suggestedPhotos[i].id != 0)
            {
                printf("✓ Loaded: %s (Rating: %.1f) - %s [%dx%d]\n",
                    currentSuggestedMovies[i].getTitle().c_str(),
                    currentSuggestedMovies[i].getRating(),
                    currentSuggestedMovies[i].getGenre().c_str(),
                    suggestedPhotos[i].width,
                    suggestedPhotos[i].height);
            }
            else
            {
                printf("✗ Failed to load texture: %s\n", posterPath.c_str());
            }
        }
        else
        {
            printf("✗ File not found anywhere: %s\n", rawPath.c_str());
            suggestedPhotos[i].id = 0;
        }
    }
}

void Booking::RefreshSuggestions()
{
    for (int i = 0; i < 4; i++)
    {
        if (suggestedPhotos[i].id != 0)
        {
            UnloadTexture(suggestedPhotos[i]);
            suggestedPhotos[i].id = 0;
        }
    }

    loadRandomSuggestions();
}

void Booking::SetScreenPointer(int* screen)
{
    currentScreen = screen;
}

void Booking::Unload()
{
    if (background.id != 0)
        UnloadTexture(background);

    if (logo.id != 0)
        UnloadTexture(logo);

    if (iconHome.id != 0)
        UnloadTexture(iconHome);

    if (iconMap.id != 0)
        UnloadTexture(iconMap);

    if (iconFilms.id != 0)
        UnloadTexture(iconFilms);

    if (iconOffers.id != 0)
        UnloadTexture(iconOffers);

    if (iconProfile.id != 0)
        UnloadTexture(iconProfile);

    for (int i = 0; i < 4; i++)
    {
        if (suggestedPhotos[i].id != 0)
        {
            UnloadTexture(suggestedPhotos[i]);
        }
    }

    if (customFont.texture.id != 0)
    {
        UnloadFont(customFont);
    }

    printf("Booking resources unloaded\n");
}

void Booking::Update()
{
    float wheelMove = GetMouseWheelMove();
    if (wheelMove != 0)
    {
        scrollOffset -= wheelMove * 40;

        if (scrollOffset < 0)
            scrollOffset = 0;
        if (scrollOffset > maxScroll)
            scrollOffset = maxScroll;
    }

    // Static variables preserve their data state layout across consecutive frames.
    // When returning from the Login screen, flip this to true to proceed with ticket selection.
    static bool isUserLoggedIn = false;

    // Optional placeholder check: Add code logic here if you want to pull down 
    // real-time active user states from your global UserService session state.
}

void Booking::DrawNavigationBar()
{
    int screenWidth = GetScreenWidth();
    int screenHeight = GetScreenHeight();

    float navWidth = screenWidth * 0.9f;
    float navHeight = 100.0f;

    Rectangle navBarRect =
    {
        (screenWidth - navWidth) / 2,
        20,
        navWidth,
        navHeight
    };

    DrawRectangleRounded(navBarRect, 0.5f, 10, WHITE);

    if (logo.id != 0)
    {
        DrawTextureEx(logo, { navBarRect.x - 2, navBarRect.y - 20 }, 0.0f, 0.3f, WHITE);
    }

    if (customFont.texture.id != 0)
    {
        DrawTextEx(customFont, "Vanema", { navBarRect.x + 130, navBarRect.y + 40 }, 34, 1, BLACK);
    }

    const char* labels[] = { "Home", "Spots", "Films", "Offers" };
    Texture2D icons[] = { iconHome, iconMap, iconFilms, iconOffers };

    float spacing = 94.0f;
    float startX = navBarRect.x + 930;

    SetMouseCursor(MOUSE_CURSOR_DEFAULT);
    Vector2 mousePos = GetMousePosition();

    for (int i = 0; i < 4; i++)
    {
        float itemX = startX + (i * spacing);

        Rectangle btnRect = { itemX, navBarRect.y, 110, navHeight };
        bool isHovered = CheckCollisionPointRec(mousePos, btnRect);
        Color tint = (i == activeIndex) ? BLUE : DARKBLUE;

        if (isHovered)
        {
            SetMouseCursor(MOUSE_CURSOR_POINTING_HAND);
            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
            {
                activeIndex = i;
                printf("Navigation switched to: %s\n", labels[i]);

                if (currentScreen != nullptr)
                {
                    if (i == 2)
                    {
                        *currentScreen = 4;
                        printf("Switching state to Films screen\n");
                    }
                }
            }
        }

        if (icons[i].id != 0)
        {
            DrawTextureEx(icons[i], { itemX + 22, navBarRect.y + 5 }, 0.0f, 0.1f, tint);
        }

        if (customFont.texture.id != 0)
        {
            DrawTextEx(customFont, labels[i], { itemX + 30, navBarRect.y + 70 }, 20, 1, BLACK);
        }
    }

    Rectangle profileRect = { navBarRect.x + navWidth - 70, navBarRect.y + 15, 50, 50 };
    bool isProfileHovered = CheckCollisionPointRec(mousePos, profileRect);

    if (isProfileHovered)
    {
        SetMouseCursor(MOUSE_CURSOR_POINTING_HAND);
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && currentScreen != nullptr)
        {
            // Profile icon opens the account login authentication gate directly
            *currentScreen = 1;
            printf("Switching to Profile Login screen\n");
        }
    }

    if (iconProfile.id != 0)
    {
        DrawTextureEx(iconProfile, { profileRect.x - 15, profileRect.y + 10 }, 0.0f, 0.1f,
            isProfileHovered ? BLUE : DARKBLUE);
    }
}

void Booking::DrawMoviePosters()
{
    int screenWidth = GetScreenWidth();

    float navWidth = screenWidth * 0.9f;
    float navHeight = 100.0f;

    Rectangle navBarRect =
    {
        (screenWidth - navWidth) / 2,
        20,
        navWidth,
        navHeight
    };

    BeginScissorMode(
        (int)navBarRect.x,
        (int)(navBarRect.y + navBarRect.height),
        (int)navBarRect.width,
        GetScreenHeight() - (int)(navBarRect.y + navBarRect.height)
    );

    float contentY = navBarRect.y + navBarRect.height + 40 - scrollOffset;

    if (customFont.texture.id != 0)
    {
        DrawTextEx(customFont, "Suggested for You:", { navBarRect.x + 20, contentY }, 40, 1, BLACK);
    }

    float posterWidth = 260;
    float posterHeight = 380;
    float spacingX = 45;

    float startPosterX = navBarRect.x + 120;
    float startPosterY = contentY + 80;

    Vector2 mousePos = GetMousePosition();

    // Session state check (Replace with a unified session helper object if desired)
    bool isUserLoggedIn = false;

    for (size_t i = 0; i < currentSuggestedMovies.size() && i < 4; i++)
    {
        float x = startPosterX + i * (posterWidth + spacingX);

        Rectangle posterRect = { x, startPosterY, posterWidth, posterHeight };
        bool hovered = CheckCollisionPointRec(mousePos, posterRect);

        if (suggestedPhotos[i].id != 0)
        {
            DrawTexturePro(
                suggestedPhotos[i],
                { 0, 0, (float)suggestedPhotos[i].width, (float)suggestedPhotos[i].height },
                posterRect,
                { 0, 0 },
                0.0f,
                hovered ? LIGHTGRAY : WHITE
            );
        }
        else
        {
            DrawRectangleRec(posterRect, DARKGRAY);

            if (customFont.texture.id != 0)
            {
                DrawTextEx(customFont, "No Image", { x + 80, startPosterY + posterHeight / 2 }, 20, 1, WHITE);
            }
        }

        if (customFont.texture.id != 0)
        {
            DrawTextEx(
                customFont,
                currentSuggestedMovies[i].getTitle().c_str(),
                { x + 10, startPosterY + posterHeight + 5 },
                27,
                1,
                BLACK
            );

            char ratingText[32];
            snprintf(ratingText, sizeof(ratingText), "Rating: %.1f", currentSuggestedMovies[i].getRating());

            DrawTextEx(
                customFont,
                ratingText,
                { x + 10, startPosterY + posterHeight + 30 },
                24,
                1,
                ORANGE
            );

            DrawTextEx(
                customFont,
                currentSuggestedMovies[i].getGenre().c_str(),
                { x + 10, startPosterY + posterHeight + 50 },
                23,
                1,
                BLACK
            );
        }

        DrawRectangleRoundedLines(
            posterRect,
            0.08f,
            8,
            4,
            hovered ? BLUE : WHITE
        );

        if (hovered && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
        {
            printf("Selected movie: %s\n", currentSuggestedMovies[i].getTitle().c_str());

            if (!isUserLoggedIn)
            {
                printf("User unauthenticated! Redirecting to login sequence.\n");
                if (currentScreen != nullptr)
                {
                    *currentScreen = 1; // Redirects smoothly to Login (Screen 1)
                    EndScissorMode();
                    return;
                }
            }
            else
            {
                // Authenticated path: Proceed directly to seat selection layout here
                printf("User verified. Opening seat chart details layout...\n");
            }
        }
    }

    EndScissorMode();
}

void Booking::DrawScrollbar()
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

void Booking::Draw()
{
    if (background.id != 0)
    {
        DrawTexturePro(
            background,
            { 0, 0, (float)background.width, (float)background.height },
            { 0, 0, (float)GetScreenWidth(), (float)GetScreenHeight() },
            { 0, 0 },
            0.0f,
            WHITE
        );
    }
    else
    {
        ClearBackground(RAYWHITE);
    }

    DrawMoviePosters();
    DrawScrollbar();
    DrawNavigationBar();
}
