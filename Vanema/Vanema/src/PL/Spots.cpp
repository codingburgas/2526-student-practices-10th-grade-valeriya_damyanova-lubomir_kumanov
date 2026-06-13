#include "Spots.h"
#include <stdio.h>
#include <string.h>

Spots::Spots() {
    logo = LoadTexture("assets/logo.png");
    iconHome = LoadTexture("assets/icon_home.png");
    iconMap = LoadTexture("assets/icon_map.png");
    iconFilms = LoadTexture("assets/icon_films.png");
    iconOffers = LoadTexture("assets/icon_offers.png");
    iconProfile = LoadTexture("assets/icon_profile.png");

    iconLocationMarker = LoadTexture("assets/icon_location_marker.png");
    iconSearch = LoadTexture("assets/icon_search.png");
    iconFilter = LoadTexture("assets/icon_filter.png");
    iconHalls = LoadTexture("assets/icon_halls.png");
    icon3D = LoadTexture("assets/icon_3d.png");
    iconAudio = LoadTexture("assets/icon_audio.png");
    iconBeach = LoadTexture("assets/icon_beach.png");
    iconPool = LoadTexture("assets/icon_pool.png");
    iconSofa = LoadTexture("assets/icon_sofa.png");

    customFont = LoadFont("assets/fonts/PlayfairDisplay-Medium.ttf");

    scrollOffset = 0.0f;
    maxScroll = 300.0f; 
    activeIndex = 1;    
    activeCityIndex = 0; 

    memset(searchBuffer, 0, sizeof(searchBuffer));
    searchLetterCount = 0;

    currentScreen = nullptr;
    isLoggedIn = false;
    userName = "";

    InitializeData();
}

Spots::~Spots() {
    Unload();
}

void Spots::InitializeData() {
    cities = { "All Locations", "Burgas", "Nessebar", "Pomorie", "Sozopol", "Primorsko", "Sunny Beach", "Aheloy", "Kableshkovo" };

    locations = {
        { "Burgas Center", "ul. Aleksandrovska 100, Burgas", 7, true, true },
        { "Nessebar", "ul. Han Krum 12, Nessebar", 5, true, true },
        { "Pomorie", "ul. Otec Paisii 8, Pomorie", 4, true, true },
        { "Sozopol", "ul. Republikanska 15, Sozopol", 4, true, true },
        { "Primorsko", "ul. Treti mart 6, Primorsko", 4, true, true },
        { "Sunny Beach", "k.k. Sunny Beach, zona Zapad", 6, true, true },
        { "Aheloy", "ul. Cherno more 9, Aheloy", 3, true, true },
        { "Kableshkovo", "ul. Mladezhka 3, Kableshkovo", 2, true, true }
    };

    experiences = {
        { "Vanema - On the Beach", "Outdoor cinema experience by the sea.\nMovies, sand and stars.", "Outdoor", "Big Screen", "Snacks & Drinks" },
        { "Vanema - By the Pool", "Relax by the pool and enjoy your\nfavorite movies.", "Outdoor", "Big Screen", "Drinks" },
        { "Vanema - Private Hall", "Private cinema hall for you and your friends.\nCatering and comfy sofa zone included.", "Private Hall", "Catering", "Comfy Sofas" }
    };
}

void Spots::SetScreenPointer(int* screen) {
    currentScreen = screen;
}

void Spots::SetUserData(bool loggedIn, const std::string& name) {
    this->isLoggedIn = loggedIn;
    size_t spacePos = name.find(' ');
    this->userName = (spacePos != std::string::npos) ? name.substr(0, spacePos) : name;
}

void Spots::Unload() {
    if (logo.id != 0) UnloadTexture(logo);
    if (iconHome.id != 0) UnloadTexture(iconHome);
    if (iconMap.id != 0) UnloadTexture(iconMap);
    if (iconFilms.id != 0) UnloadTexture(iconFilms);
    if (iconOffers.id != 0) UnloadTexture(iconOffers);
    if (iconProfile.id != 0) UnloadTexture(iconProfile);
    if (iconLocationMarker.id != 0) UnloadTexture(iconLocationMarker);
    if (iconSearch.id != 0) UnloadTexture(iconSearch);
    if (iconFilter.id != 0) UnloadTexture(iconFilter);
    if (iconHalls.id != 0) UnloadTexture(iconHalls);
    if (icon3D.id != 0) UnloadTexture(icon3D);
    if (iconAudio.id != 0) UnloadTexture(iconAudio);
    if (iconBeach.id != 0) UnloadTexture(iconBeach);
    if (iconPool.id != 0) UnloadTexture(iconPool);
    if (iconSofa.id != 0) UnloadTexture(iconSofa);
    if (customFont.texture.id != 0) UnloadFont(customFont);
}

void Spots::Update() {
    float wheelMove = GetMouseWheelMove();
    if (wheelMove != 0) {
        scrollOffset -= wheelMove * 45.0f;
        if (scrollOffset < 0) scrollOffset = 0;
        if (scrollOffset > maxScroll) scrollOffset = maxScroll;
    }
    if (CheckCollisionPointRec(GetMousePosition(), { (float)GetScreenWidth() * 0.35f, 150 - scrollOffset, 400, 45 })) {
        int key = GetCharPressed();
        while (key > 0) {
            if ((key >= 32) && (key <= 125) && (searchLetterCount < 63)) {
                searchBuffer[searchLetterCount] = (char)key;
                searchBuffer[searchLetterCount + 1] = '\0';
                searchLetterCount++;
            }
            key = GetCharPressed();
        }
        if (IsKeyPressed(KEY_BACKSPACE)) {
            searchLetterCount--;
            if (searchLetterCount < 0) searchLetterCount = 0;
            searchBuffer[searchLetterCount] = '\0';
        }
    }
}

void Spots::DrawNavigationBar() {
    int screenWidth = GetScreenWidth();
    float navWidth = screenWidth * 0.9f;
    float navHeight = 100.0f;
    Rectangle navBarRect = { (screenWidth - navWidth) / 2, 20, navWidth, navHeight };

    DrawRectangleRounded(navBarRect, 0.5f, 10, WHITE);

    if (logo.id != 0) DrawTextureEx(logo, { navBarRect.x - 2, navBarRect.y - 20 }, 0.0f, 0.3f, WHITE);
    if (customFont.texture.id != 0) DrawTextEx(customFont, "Vanema", { navBarRect.x + 130, navBarRect.y + 40 }, 34, 1, BLACK);

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
            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                activeIndex = i;
                if (currentScreen != nullptr) {
                    if (i == 0) *currentScreen = 0; 
                    if (i == 2) *currentScreen = 4; 
                }
            }
        }
        if (icons[i].id != 0) DrawTextureEx(icons[i], { itemX + 50, navBarRect.y + 5 }, 0.0f, 0.1f, tint);
        if (customFont.texture.id != 0) DrawTextEx(customFont, labels[i], { itemX + 58, navBarRect.y + 70 }, 20, 1, BLACK);
    }

    Rectangle profileRect = { navBarRect.x + navWidth - 70, navBarRect.y + 15, 50, 50 };
    if (CheckCollisionPointRec(mousePos, profileRect) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && currentScreen != nullptr) {
        *currentScreen = 1;
    }
    if (iconProfile.id != 0) DrawTextureEx(iconProfile, { profileRect.x - 15, profileRect.y + (isLoggedIn ? -5.0f : 10.0f) }, 0.0f, 0.1f, DARKBLUE);
    if (isLoggedIn && !userName.empty() && customFont.texture.id != 0) {
        DrawTextEx(customFont, userName.c_str(), { profileRect.x, profileRect.y + profileRect.height + 5.0f }, 22, 1, BLACK);
    }
}

void Spots::DrawSearchBarAndFilters() {
    int screenWidth = GetScreenWidth();
    float contentWidth = screenWidth * 0.9f;
    float startX = (screenWidth - contentWidth) / 2;
    Vector2 mousePos = GetMousePosition();

    float titleY = 150 - scrollOffset;

    if (customFont.texture.id != 0) {
        DrawTextEx(customFont, "Locations", { startX + 20, titleY }, 42, 1, Color{ 16, 25, 48, 255 });
        DrawTextEx(customFont, "Find a Vanema cinema near you in Burgas region.", { startX + 20, titleY + 50 }, 20, 1, GRAY);
    }

    Rectangle searchRect = { screenWidth * 0.35f, titleY + 15, 400, 48 };
    DrawRectangleRounded(searchRect, 0.3f, 8, WHITE);
    DrawRectangleRoundedLines(searchRect, 0.3f, 8, 1, LIGHTGRAY);
    if (iconSearch.id != 0) DrawTextureEx(iconSearch, { searchRect.x + 15, searchRect.y + 14 }, 0.0f, 0.04f, GRAY);

    if (searchLetterCount == 0 && customFont.texture.id != 0) {
        DrawTextEx(customFont, "Search by city or area...", { searchRect.x + 50, searchRect.y + 13 }, 21, 1, LIGHTGRAY);
    }
    else if (customFont.texture.id != 0) {
        DrawTextEx(customFont, searchBuffer, { searchRect.x + 50, searchRect.y + 13 }, 21, 1, BLACK);
    }

    Rectangle filterBtn = { searchRect.x + searchRect.width + 20, searchRect.y, 110, 48 };
    bool hoverFilter = CheckCollisionPointRec(mousePos, filterBtn);
    DrawRectangleRounded(filterBtn, 0.3f, 8, WHITE);
    DrawRectangleRoundedLines(filterBtn, 0.3f, 8, 1, hoverFilter ? BLUE : LIGHTGRAY);
    if (iconFilter.id != 0) DrawTextureEx(iconFilter, { filterBtn.x + 15, filterBtn.y + 14 }, 0.0f, 0.04f, BLACK);
    if (customFont.texture.id != 0) DrawTextEx(customFont, "Filter", { filterBtn.x + 45, filterBtn.y + 13 }, 21, 1, BLACK);

    float chipY = titleY + 110;
    float currentChipX = startX + 20;

    for (size_t i = 0; i < cities.size(); i++) {
        float textWidth = customFont.texture.id != 0 ? MeasureTextEx(customFont, cities[i].c_str(), 19, 1).x : 80;
        float chipWidth = textWidth + 40;
        Rectangle chipRect = { currentChipX, chipY, chipWidth, 42 };

        bool hoverChip = CheckCollisionPointRec(mousePos, chipRect);
        if (hoverChip) {
            SetMouseCursor(MOUSE_CURSOR_POINTING_HAND);
            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) activeCityIndex = i;
        }

        Color bgCol = (i == activeCityIndex) ? Color{ 16, 25, 48, 255 } : WHITE;
        Color textCol = (i == activeCityIndex) ? WHITE : Color{ 16, 25, 48, 255 };

        DrawRectangleRounded(chipRect, 0.3f, 8, bgCol);
        DrawRectangleRoundedLines(chipRect, 0.3f, 8, 1, LIGHTGRAY);

        if (iconLocationMarker.id != 0 && i > 0) {
            DrawTextureEx(iconLocationMarker, { chipRect.x + 12, chipRect.y + 11 }, 0.0f, 0.04f, textCol);
            if (customFont.texture.id != 0) DrawTextEx(customFont, cities[i].c_str(), { chipRect.x + 35, chipRect.y + 11 }, 19, 1, textCol);
        }
        else {
            if (customFont.texture.id != 0) DrawTextEx(customFont, cities[i].c_str(), { chipRect.x + 20, chipRect.y + 11 }, 19, 1, textCol);
        }

        currentChipX += chipWidth + 15;
    }
}

void Spots::DrawCinemaCards(float& currentY, Vector2 mousePos) {
    int screenWidth = GetScreenWidth();
    float contentWidth = screenWidth * 0.9f;
    float startX = (screenWidth - contentWidth) / 2;

    currentY += 190;
    if (customFont.texture.id != 0) DrawTextEx(customFont, "Vanema Cinemas", { startX + 20, currentY }, 26, 1, Color{ 16, 25, 48, 255 });

    currentY += 40;

    float cardWidth = (contentWidth - 60) / 4;
    float cardHeight = 220;

    for (size_t i = 0; i < locations.size(); i++) {
        int row = i / 4;
        int col = i % 4;

        float cx = startX + col * (cardWidth + 20);
        float cy = currentY + row * (cardHeight + 20);

        Rectangle cardRect = { cx, cy, cardWidth, cardHeight };
        DrawRectangleRounded(cardRect, 0.08f, 10, WHITE);
        DrawRectangleRoundedLines(cardRect, 0.08f, 10, 1, Fade(LIGHTGRAY, 0.6f));

        if (iconLocationMarker.id != 0) DrawTextureEx(iconLocationMarker, { cx + 20, cy + 20 }, 0.0f, 0.07f, Color{ 16, 25, 48, 255 });

        if (customFont.texture.id != 0) {
            DrawTextEx(customFont, "Vanema", { cx + 65, cy + 18 }, 16, 1, GRAY);
            DrawTextEx(customFont, locations[i].name.c_str(), { cx + 65, cy + 36 }, 23, 1, Color{ 16, 25, 48, 255 });
            DrawTextEx(customFont, locations[i].address.c_str(), { cx + 22, cy + 78 }, 17, 1, GRAY);
        }

        float itemY = cy + 115;
        if (iconHalls.id != 0) DrawTextureEx(iconHalls, { cx + 22, itemY }, 0.0f, 0.04f, Color{ 16, 25, 48, 255 });
        char hallsStr[16]; sprintf(hallsStr, "%d Halls", locations[i].halls);
        if (customFont.texture.id != 0) DrawTextEx(customFont, hallsStr, { cx + 48, itemY + 3 }, 16, 1, Color{ 16, 25, 48, 255 });

        if (icon3D.id != 0) DrawTextureEx(icon3D, { cx + 125, itemY }, 0.0f, 0.04f, Color{ 16, 25, 48, 255 });
        if (customFont.texture.id != 0) DrawTextEx(customFont, "3D", { cx + 152, itemY + 3 }, 16, 1, Color{ 16, 25, 48, 255 });

        if (iconAudio.id != 0) DrawTextureEx(iconAudio, { cx + 200, itemY }, 0.0f, 0.04f, Color{ 16, 25, 48, 255 });
        if (customFont.texture.id != 0) DrawTextEx(customFont, "Dolby Atmos", { cx + 225, itemY + 3 }, 15, 1, Color{ 16, 25, 48, 255 });

        Rectangle btnRect = { cx + 20, cy + 155, cardWidth - 40, 42 };
        bool hoverBtn = CheckCollisionPointRec(mousePos, btnRect);

        DrawRectangleRounded(btnRect, 0.2f, 6, WHITE);
        DrawRectangleRoundedLines(btnRect, 0.2f, 6, 1.5f, Color{ 16, 25, 48, 255 });
        if (customFont.texture.id != 0) DrawTextEx(customFont, "View Details", { btnRect.x + (btnRect.width / 2) - 50, btnRect.y + 11 }, 19, 1, Color{ 16, 25, 48, 255 });

        if (hoverBtn) {
            SetMouseCursor(MOUSE_CURSOR_POINTING_HAND);
            DrawRectangleRounded(btnRect, 0.2f, 6, Fade(LIGHTGRAY, 0.2f));
        }
    }

    currentY += 2 * (cardHeight + 20) + 10;
}

void Spots::DrawExperiences(float& currentY) {
    int screenWidth = GetScreenWidth();
    float contentWidth = screenWidth * 0.9f;
    float startX = (screenWidth - contentWidth) / 2;

    if (customFont.texture.id != 0) DrawTextEx(customFont, "Vanema Experiences", { startX + 20, currentY }, 26, 1, Color{ 16, 25, 48, 255 });
    currentY += 40;

    float expWidth = (contentWidth - 40) / 3;
    float expHeight = 160;
    Texture2D expIcons[] = { iconBeach, iconPool, iconSofa };

    for (size_t i = 0; i < experiences.size(); i++) {
        float ex = startX + i * (expWidth + 20);
        Rectangle expRect = { ex, currentY, expWidth, expHeight };

        DrawRectangleRounded(expRect, 0.08f, 10, WHITE);
        DrawRectangleRoundedLines(expRect, 0.08f, 10, 1, Fade(LIGHTGRAY, 0.6f));

        if (expIcons[i].id != 0) DrawTextureEx(expIcons[i], { ex + 25, currentY + 35 }, 0.0f, 0.08f, Color{ 16, 25, 48, 255 });

        float textX = ex + 95;
        if (customFont.texture.id != 0) {
            DrawTextEx(customFont, experiences[i].title.c_str(), { textX, currentY + 25 }, 22, 1, Color{ 16, 25, 48, 255 });
            DrawTextEx(customFont, experiences[i].description.c_str(), { textX, currentY + 55 }, 17, 1, GRAY);
        }

        float tagY = currentY + 115;
        float currentTagX = textX;
        const char* tags[] = { experiences[i].tag1.c_str(), experiences[i].tag2.c_str(), experiences[i].tag3.c_str() };

        for (int j = 0; j < 3; j++) {
            if (strlen(tags[j]) == 0) continue;
            float tw = customFont.texture.id != 0 ? MeasureTextEx(customFont, tags[j], 15, 1).x : 50;

            if (customFont.texture.id != 0) DrawTextEx(customFont, tags[j], { currentTagX + 15, tagY + 2 }, 15, 1, Color{ 16, 25, 48, 255 });
            currentTagX += tw + 30;
        }
    }
    currentY += expHeight + 40;
}

void Spots::DrawScrollbar() {
    int screenWidth = GetScreenWidth();
    float sbHeight = 400;
    float sbWidth = 8;
    float sbX = screenWidth - 20;
    float sbY = 150;

    DrawRectangleRounded({ sbX, sbY, sbWidth, sbHeight }, 0.5f, 8, Fade(LIGHTGRAY, 0.3f));
    float thumbHeight = 90;
    float thumbY = sbY;
    if (maxScroll > 0) thumbY = sbY + (scrollOffset / maxScroll) * (sbHeight - thumbHeight);

    DrawRectangleRounded({ sbX, thumbY, sbWidth, thumbHeight }, 0.5f, 8, Color{ 16, 25, 48, 255 });
}

void Spots::Draw() {
    ClearBackground(Color{ 238, 244, 253, 255 });

    Vector2 mousePos = GetMousePosition();
    int screenWidth = GetScreenWidth();
    float contentWidth = screenWidth * 0.9f;
    float startX = (screenWidth - contentWidth) / 2;

    BeginScissorMode((int)startX, 130, (int)contentWidth, GetScreenHeight() - 130);

    float trackingY = 150 - scrollOffset;
    DrawSearchBarAndFilters();
    DrawCinemaCards(trackingY, mousePos);
    DrawExperiences(trackingY);

    EndScissorMode();

    DrawScrollbar();
    DrawNavigationBar(); 

}