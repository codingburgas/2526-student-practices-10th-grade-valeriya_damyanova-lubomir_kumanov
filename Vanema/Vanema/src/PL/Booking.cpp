#include "Booking.h"
#include <stdio.h>
#include <stdarg.h>
#include <fstream>
#include <iostream>
#include <algorithm> 

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

    iconHome = LoadTexture("assets/icons/icon_home.png");
    iconMap = LoadTexture("assets/icons/icon_map.png");
    iconFilms = LoadTexture("assets/icons/icon_films.png");
    iconOffers = LoadTexture("assets/icons/icon_offers.png");
    iconProfile = LoadTexture("assets/icons/icon_profile.png");

    if (iconHome.id == 0) printf("Failed to load: assets/icons/icon_home.png\n");
    if (iconMap.id == 0) printf("Failed to load: assets/icons/icon_map.png\n");
    if (iconFilms.id == 0) printf("Failed to load: assets/icons/icon_films.png\n");
    if (iconOffers.id == 0) printf("Failed to load: assets/icons/icon_offers.png\n");
    if (iconProfile.id == 0) printf("Failed to load: assets/icons/icon_profile.png\n");

    for (int i = 0; i < 4; i++)
    {
        suggestedPhotos[i].id = 0;
        topRatedPhotos[i].id = 0;
    }

    customFont = LoadFont("assets/fonts/Roboto-Medium.ttf");
    if (customFont.texture.id == 0) printf("Failed to load font\n");

    scrollOffset = 0;
    maxScroll = 450;
    activeIndex = 0;
    currentScreen = nullptr;
    mouseScrollAccumulator = 0;
    isLoggedIn = false;
    userName = "";
    isAdmin = false;

    printf("Booking initialized successfully!\n");
}

Booking::~Booking()
{
    Unload();
}

void Booking::loadRandomSuggestions()
{
    if (movieService == nullptr)
    {
        printf("✗ Error: movieService pointer is NULL inside loadRandomSuggestions!\n");
        return;
    }
    currentSuggestedMovies = movieService->getRandomMovies(4);

    if (currentSuggestedMovies.empty())
    {
        printf("No movies loaded from service!\n");
        return;
    }

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
        }
        else
        {
            suggestedPhotos[i].id = 0;
        }
    }

    std::vector<Movie> allMovies = movieService->getRandomMovies(100);

    std::sort(allMovies.begin(), allMovies.end(), [](const Movie& a, const Movie& b) {
        return a.getRating() > b.getRating();
        });

    topRatedMovies.clear();
    for (size_t i = 0; i < allMovies.size() && i < 4; i++)
    {
        topRatedMovies.push_back(allMovies[i]);
    }

    for (size_t i = 0; i < topRatedMovies.size() && i < 4; i++)
    {
        std::string rawPath = topRatedMovies[i].getPosterPath();
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
            topRatedPhotos[i] = LoadTexture(posterPath.c_str());
        }
        else
        {
            topRatedPhotos[i].id = 0;
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
        if (topRatedPhotos[i].id != 0)
        {
            UnloadTexture(topRatedPhotos[i]);
            topRatedPhotos[i].id = 0;
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
    if (background.id != 0) UnloadTexture(background);
    if (logo.id != 0) UnloadTexture(logo);
    if (iconHome.id != 0) UnloadTexture(iconHome);
    if (iconMap.id != 0) UnloadTexture(iconMap);
    if (iconFilms.id != 0) UnloadTexture(iconFilms);
    if (iconOffers.id != 0) UnloadTexture(iconOffers);
    if (iconProfile.id != 0) UnloadTexture(iconProfile);

    for (int i = 0; i < 4; i++)
    {
        if (suggestedPhotos[i].id != 0) UnloadTexture(suggestedPhotos[i]);
        if (topRatedPhotos[i].id != 0) UnloadTexture(topRatedPhotos[i]);
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

        if (scrollOffset < 0) scrollOffset = 0;
        if (scrollOffset > maxScroll) scrollOffset = maxScroll;
    }
}

void Booking::DrawNavigationBar()
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
                if (currentScreen != nullptr)
                {
                    if (i == 0)
                    {
                        *currentScreen = 0; 
                    }
                    else if (i == 1)
                    {
                        *currentScreen = 5; 
                    }
                    else if (i == 2)
                    {
                        *currentScreen = 4; 
                    }
                    else if (i == 3)
                    {
                        *currentScreen = 6;
                    }
                }
            }
        }

        if (icons[i].id != 0)
        {
            DrawTextureEx(icons[i], { itemX + 50, navBarRect.y + 5 }, 0.0f, 0.1f, tint);
        }

        if (customFont.texture.id != 0)
        {
            DrawTextEx(customFont, labels[i], { itemX + 58, navBarRect.y + 70 }, 20, 1, BLACK);
        }
    }

    Rectangle profileRect = { navBarRect.x + navWidth - 70, navBarRect.y + 15, 50, 50 };
    bool isProfileHovered = CheckCollisionPointRec(mousePos, profileRect);

    if (isProfileHovered)
    {
        SetMouseCursor(MOUSE_CURSOR_POINTING_HAND);
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && currentScreen != nullptr)
        {
            *currentScreen = 1;
        }
    }
    if (iconProfile.id != 0)
    {
        float iconYOffset = isLoggedIn ? -5.0f : 10.0f;

        DrawTextureEx(
            iconProfile,
            { profileRect.x - 15, profileRect.y + iconYOffset },
            0.0f,
            0.1f,
            isProfileHovered ? BLUE : DARKBLUE
        );
    }
    if (isLoggedIn && !userName.empty())
    {
        float fontSize = 22.0f;
        Color nameColor = BLACK;

        if (customFont.texture.id != 0)
        {
            Vector2 textSize = MeasureTextEx(customFont, userName.c_str(), fontSize, 1);
            float textX = profileRect.x + (profileRect.width / 2.0f) - (textSize.x / 1.5f);
            float textY = profileRect.y + profileRect.height + 5.0f;

            DrawTextEx(customFont, userName.c_str(), { textX, textY }, fontSize, 1, nameColor);
        }
        else
        {
            int textWidth = MeasureText(userName.c_str(), 16);
            int textX = profileRect.x + (profileRect.width / 2) - (textWidth / 2);
            DrawText(userName.c_str(), textX, profileRect.y + profileRect.height + 5, 16, BLACK);
        }
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

    if (isLoggedIn && isAdmin)
    {
        float btnWidth = 90.0f;
        float btnHeight = 40.0f;
        float btnY = contentY;

        Rectangle addBtn = { navBarRect.x + navWidth - 210, btnY, btnWidth, btnHeight };
        Rectangle deleteBtn = { navBarRect.x + navWidth - 110, btnY, btnWidth, btnHeight };

        Vector2 mousePos = GetMousePosition();
        bool hoverAdd = CheckCollisionPointRec(mousePos, addBtn);
        bool hoverDelete = CheckCollisionPointRec(mousePos, deleteBtn);

        if (hoverAdd || hoverDelete)
        {
            SetMouseCursor(MOUSE_CURSOR_POINTING_HAND);
        }

        if (hoverAdd && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
        {
            std::cout << "Admin triggered: ADD MOVIE\n";
        }
        if (hoverDelete && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
        {
            std::cout << "Admin triggered: DELETE MOVIE\n";
        }

        DrawRectangleRounded(addBtn, 0.3f, 6, hoverAdd ? Color{ 46, 204, 113, 255 } : Color{ 39, 174, 96, 255 });
        DrawText("Add", addBtn.x + 28, addBtn.y + 10, 20, WHITE);

        DrawRectangleRounded(deleteBtn, 0.3f, 6, hoverDelete ? Color{ 231, 76, 60, 255 } : Color{ 192, 41, 43, 255 });
        DrawText("Delete", deleteBtn.x + 16, deleteBtn.y + 10, 20, WHITE);
    }

    float posterWidth = 260;
    float posterHeight = 380;
    float spacingX = 45;

    float startPosterX = navBarRect.x + 120;
    float startPosterY = contentY + 80;

    Vector2 mousePos = GetMousePosition();

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
            DrawTextEx(customFont, currentSuggestedMovies[i].getTitle().c_str(),
                { x + 10, startPosterY + posterHeight + 5 }, 27, 1, BLACK);

            char ratingText[32];
            snprintf(ratingText, sizeof(ratingText), "Rating: %.1f", currentSuggestedMovies[i].getRating());

            DrawTextEx(customFont, ratingText,
                { x + 10, startPosterY + posterHeight + 33 }, 24, 1, ORANGE);

            DrawTextEx(customFont, currentSuggestedMovies[i].getGenre().c_str(),
                { x + 10, startPosterY + posterHeight + 55 }, 23, 1, BLACK);
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

            if (!isLoggedIn)
            {
                printf("Redirecting to login.\n");
                if (currentScreen != nullptr)
                {
                    *currentScreen = 1;
                    EndScissorMode(); 
                    return;
                }
            }
        }
    }
    EndScissorMode();

    float topRatedSectionY = startPosterY + posterHeight + 150;

    Rectangle topRatedBg = {
        navBarRect.x + 20,
        topRatedSectionY,
        navBarRect.width - 40,
        450
    };
    DrawRectangleRounded(topRatedBg, 0.05f, 12, Fade(WHITE, 0.95f));
    DrawRectangleRoundedLines(topRatedBg, 0.05f, 12, 1, Fade(LIGHTGRAY, 0.5f));

    DrawRectangle(topRatedBg.x + 20, topRatedBg.y + 30, 6, 35, Color{ 41, 128, 185, 255 });

    if (customFont.texture.id != 0)
    {
        DrawTextEx(customFont, "Top Rated Movies", { topRatedBg.x + 38, topRatedBg.y + 32 }, 32, 1, BLACK);
    }

    float colWidth = (topRatedBg.width - 80) / 2;
    float rowHeight = 150;
    float startGridX = topRatedBg.x + 30;
    float startGridY = topRatedBg.y + 100;

    for (size_t i = 0; i < topRatedMovies.size() && i < 4; i++)
    {
        int col = i % 2;
        int row = i / 2;

        float itemX = startGridX + col * (colWidth + 40);
        float itemY = startGridY + row * (rowHeight + 20);

        float imgW = 110;
        float imgH = 150;
        Rectangle imgRect = { itemX, itemY + (rowHeight - imgH) / 2, imgW, imgH };

        if (topRatedPhotos[i].id != 0)
        {
            DrawTexturePro(
                topRatedPhotos[i],
                { 0, 0, (float)topRatedPhotos[i].width, (float)topRatedPhotos[i].height },
                imgRect,
                { 0, 0 },
                0.0f,
                WHITE
            );
        }
        else
        {
            DrawRectangleRec(imgRect, DARKGRAY);
        }

        DrawRectangleRoundedLines(imgRect, 0.1f, 6, 1, Fade(GRAY, 0.4f));

        float textStartX = imgRect.x + imgRect.width + 20;

        if (customFont.texture.id != 0)
        {
            DrawTextEx(customFont, topRatedMovies[i].getTitle().c_str(), { textStartX, itemY + 12 }, 30, 1, BLACK);
            DrawTextEx(customFont, topRatedMovies[i].getGenre().c_str(), { textStartX, itemY + 44 }, 26, 1, GRAY);

            char ratingStr[16];
            snprintf(ratingStr, sizeof(ratingStr), "%.1f", topRatedMovies[i].getRating());

            Vector2 scoreSize = MeasureTextEx(customFont, ratingStr, 22, 1);
            float scoreX = itemX + colWidth - scoreSize.x - 10;
            float starX = scoreX - 24;

            DrawTextEx(customFont, "*", { starX, itemY + 20 }, 45, 1, GOLD);
            DrawTextEx(customFont, ratingStr, { scoreX, itemY + 23 }, 25, 1, DARKGRAY);
        }
    }
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

void Booking::SetUserData(bool loggedIn, const std::string& name, bool admin) {
    this->isLoggedIn = loggedIn;
    this->isAdmin = admin;

    size_t spacePos = name.find(' ');
    if (spacePos != std::string::npos) {
        this->userName = name.substr(0, spacePos);
    }
    else {
        this->userName = name;
    }
}