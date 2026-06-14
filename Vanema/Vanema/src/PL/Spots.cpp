#include "Spots.h"
#include <stdio.h>
#include <string.h>
#include <sstream>
#include <vector>

Spots::Spots() {
    logo = LoadTexture("assets/logo.png");
    iconHome = LoadTexture("assets/icon_home.png");
    iconMap = LoadTexture("assets/icon_map.png");
    iconFilms = LoadTexture("assets/icon_films.png");
    iconOffers = LoadTexture("assets/icon_offers.png");
    iconProfile = LoadTexture("assets/icon_profile.png");
    iconLocationMarker = LoadTexture("assets/icon_map.png");
    iconHalls = LoadTexture("assets/icon_clapperboard.png");
    icon3D = LoadTexture("assets/icon_3d.png");
    iconAudio = LoadTexture("assets/icon_sound.png");
    iconBeach = LoadTexture("assets/icon_car.png");
    iconPool = LoadTexture("assets/icon_beach.png");
    iconSofa = LoadTexture("assets/icon_sofa.png");

    uiFont = LoadFont("assets/fonts/Roboto-Medium.ttf");

    scrollOffset = 0.0f;
    maxScroll = 1200.0f;
    activeIndex = 1;
    activeCityIndex = 0;

    memset(searchBuffer, 0, sizeof(searchBuffer));
    searchLetterCount = 0;

    currentScreen = nullptr;
    isLoggedIn = false;
    isAdmin = false;
    userName = "";

    InitializeData();
}

Spots::~Spots() {
    Unload();
}

void Spots::InitializeData() {
    cities = { "All Locations", "Burgas", "Nessebar", "Pomorie", "Sozopol", "Primorsko", "Sunny Beach", "Aheloy", "Kableshkovo" };

    locations = {
        { "Burgas Center", "ul. Aleksandrovska 100, Burgas", 11, true, true },
        { "Nessebar", "ul. Han Krum 12, Nessebar", 5, true, true },
        { "Pomorie", "ul. Otec Paisii 8, Pomorie", 4, true, true },
        { "Sozopol", "ul. Republikanska 15, Sozopol", 4, true, true },
        { "Primorsko", "ul. Treti mart 6, Primorsko", 4, true, true },
        { "Sunny Beach", "k.k. Sunny Beach, zona Zapad", 6, true, true },
        { "Aheloy", "ul. Cherno more 9, Aheloy", 3, true, true },
        { "Kableshkovo", "ul. Mladezhka 3, Kableshkovo", 2, true, true }
    };

    experiences = {
        { "Vanema - Drive In", "Retro outdoor cinema experience from your vehicle.\nMovies, car audio tuning, and stars.", "Car Cinema", "Big Screen", "Snacks & Drinks" },
        { "Vanema - By the Pool", "Relax by the pool and enjoy your\nfavorite movies.", "Outdoor", "Big Screen", "Drinks" },
        { "Vanema - Private Hall", "Private cinema hall for you and your friends.\nCatering and comfy sofa zone included.", "Private Hall", "Catering", "Comfy Sofas" }
    };
}

void Spots::SetScreenPointer(int* screen) {
    currentScreen = screen;
}

void Spots::SetUserData(bool loggedIn, const std::string& name, bool isAdmin) {
    this->isLoggedIn = loggedIn;
    this->isAdmin = isAdmin;
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
    if (iconHalls.id != 0) UnloadTexture(iconHalls);
    if (icon3D.id != 0) UnloadTexture(icon3D);
    if (iconAudio.id != 0) UnloadTexture(iconAudio);
    if (iconBeach.id != 0) UnloadTexture(iconBeach);
    if (iconPool.id != 0) UnloadTexture(iconPool);
    if (iconSofa.id != 0) UnloadTexture(iconSofa);
    if (uiFont.texture.id != 0) UnloadFont(uiFont);
}

void Spots::Update() {
    float wheelMove = GetMouseWheelMove();
    if (wheelMove != 0) {
        scrollOffset -= wheelMove * 45.0f;
        if (scrollOffset < 0) scrollOffset = 0;
        if (scrollOffset > maxScroll) scrollOffset = maxScroll;
    }
}

void Spots::DrawNavigationBar() {
    int screenWidth = GetScreenWidth();
    float navWidth = screenWidth * 0.9f;
    float navHeight = 100.0f;
    Rectangle navBarRect = { (screenWidth - navWidth) / 2, 20, navWidth, navHeight };

    DrawRectangleRounded(navBarRect, 0.5f, 10, WHITE);

    if (logo.id != 0) DrawTextureEx(logo, { navBarRect.x - 2, navBarRect.y - 20 }, 0.0f, 0.3f, WHITE);
    if (uiFont.texture.id != 0) DrawTextEx(uiFont, "Vanema", { navBarRect.x + 130, navBarRect.y + 40 }, 34, 1, BLACK);

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
                    if (i == 0) *currentScreen = 2;
                    if (i == 2) *currentScreen = 4;
                }
            }
        }
        if (icons[i].id != 0) DrawTextureEx(icons[i], { itemX + 50, navBarRect.y + 5 }, 0.0f, 0.1f, tint);
        if (uiFont.texture.id != 0) DrawTextEx(uiFont, labels[i], { itemX + 58, navBarRect.y + 70 }, 20, 1, BLACK);
    }

    Rectangle profileRect = { navBarRect.x + navWidth - 70, navBarRect.y + 15, 50, 50 };
    if (CheckCollisionPointRec(mousePos, profileRect) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && currentScreen != nullptr) {
        *currentScreen = 1;
    }
    if (iconProfile.id != 0) DrawTextureEx(iconProfile, { profileRect.x - 15, profileRect.y + (isLoggedIn ? -5.0f : 10.0f) }, 0.0f, 0.1f, DARKBLUE);
    if (isLoggedIn && !userName.empty() && uiFont.texture.id != 0) {
        DrawTextEx(uiFont, userName.c_str(), { profileRect.x, profileRect.y + profileRect.height + 5.0f }, 22, 1, BLACK);
    }
}

void Spots::DrawSearchBarAndFilters() {
    int screenWidth = GetScreenWidth();
    float contentWidth = screenWidth * 0.9f;
    float startX = (screenWidth - contentWidth) / 2;

    float titleY = 150 - scrollOffset;

    if (uiFont.texture.id != 0) {
        DrawTextEx(uiFont, "Locations", { startX + 20, titleY }, 52, 1, Color{ 16, 25, 48, 255 });
        DrawTextEx(uiFont, "Find a Vanema cinema near you in Burgas region.", { startX + 20, titleY + 65 }, 24, 1, GRAY);
    }

    float chipY = titleY + 135;
    float currentChipX = startX + 20;

    float chipFontSize = 22.0f;
    float chipGap = 8.0f;

    for (size_t i = 0; i < cities.size(); i++) {
        float textWidth = uiFont.texture.id != 0 ? MeasureTextEx(uiFont, cities[i].c_str(), chipFontSize, 1).x : 85;

        float chipWidth = textWidth + (i > 0 ? 56 : 30);
        Rectangle chipRect = { currentChipX, chipY, chipWidth, 50 };

        Color bgCol = (i == activeCityIndex) ? Color{ 16, 25, 48, 255 } : WHITE;
        Color textCol = (i == activeCityIndex) ? WHITE : Color{ 16, 25, 48, 255 };

        DrawRectangleRounded(chipRect, 0.25f, 8, bgCol);
        DrawRectangleRoundedLines(chipRect, 0.25f, 8, 1.5f, Fade(LIGHTGRAY, 0.8f));

        float textYOffset = chipRect.y + (chipRect.height - chipFontSize) / 2.0f;

        if (iconLocationMarker.id != 0 && i > 0) {
            float iconHeight = iconLocationMarker.height * 0.050f;
            float iconYOffset = chipRect.y + (chipRect.height - iconHeight) / 2.0f;

            DrawTextureEx(iconLocationMarker, { chipRect.x + 10, iconYOffset }, 0.0f, 0.050f, textCol);
            if (uiFont.texture.id != 0) DrawTextEx(uiFont, cities[i].c_str(), { chipRect.x + 44, textYOffset }, chipFontSize, 1, textCol);
        }
        else {
            if (uiFont.texture.id != 0) DrawTextEx(uiFont, cities[i].c_str(), { chipRect.x + 15, textYOffset }, chipFontSize, 1, textCol);
        }

        currentChipX += chipWidth + chipGap;
    }
}

void Spots::DrawCinemaCards(float& currentY, Vector2 mousePos) {
    int screenWidth = GetScreenWidth();
    float contentWidth = screenWidth * 0.9f;
    float startX = (screenWidth - contentWidth) / 2;

    currentY += 240;
    if (uiFont.texture.id != 0) DrawTextEx(uiFont, "Vanema Cinemas", { startX + 20, currentY }, 32, 1, Color{ 16, 25, 48, 255 });

    currentY += 50;

    const int cardsPerRow = 3;
    float cardGap = 20.0f;

    float cardWidth = (contentWidth - (cardsPerRow - 1) * cardGap) / cardsPerRow;
    float cardHeight = 220;

    for (size_t i = 0; i < locations.size(); i++) {
        int row = i / cardsPerRow;
        int col = i % cardsPerRow;

        float cx = startX + col * (cardWidth + cardGap);
        float cy = currentY + row * (cardHeight + cardGap);

        Rectangle cardRect = { cx, cy, cardWidth, cardHeight };
        DrawRectangleRounded(cardRect, 0.08f, 10, WHITE);
        DrawRectangleRoundedLines(cardRect, 0.08f, 10, 1.5f, Fade(LIGHTGRAY, 0.8f));

        if (iconLocationMarker.id != 0) {
            DrawTextureEx(iconLocationMarker, { cx + 15, cy + 24 }, 0.0f, 0.085f, Color{ 16, 25, 48, 255 });
        }

        if (uiFont.texture.id != 0) {
            DrawTextEx(uiFont, "Vanema", { cx + 76, cy + 22 }, 22, 1, GRAY);
            DrawTextEx(uiFont, locations[i].name.c_str(), { cx + 76, cy + 48 }, 32, 1, Color{ 16, 25, 48, 255 });
            DrawTextEx(uiFont, locations[i].address.c_str(), { cx + 24, cy + 105 }, 23, 1, GRAY);
        }

        float itemY = cy + 160;
        float itemTextSize = 24.0f;

        if (iconHalls.id != 0) {
            float hIconH = iconHalls.height * 0.055f;
            float hIconY = itemY + (itemTextSize - hIconH) / 2.0f;
            DrawTextureEx(iconHalls, { cx + 24, hIconY }, 0.0f, 0.055f, Color{ 16, 25, 48, 255 });
        }
        char hallsStr[16]; sprintf(hallsStr, "%d Halls", locations[i].halls);
        if (uiFont.texture.id != 0) DrawTextEx(uiFont, hallsStr, { cx + 64, itemY }, itemTextSize, 1, Color{ 16, 25, 48, 255 });

        if (icon3D.id != 0) {
            float icon3DH = icon3D.height * 0.055f;
            float icon3DY = itemY + (itemTextSize - icon3DH) / 2.0f;
            DrawTextureEx(icon3D, { cx + 160, icon3DY }, 0.0f, 0.055f, Color{ 16, 25, 48, 255 });
        }
        if (uiFont.texture.id != 0) DrawTextEx(uiFont, "2D,3D,IMAX", { cx + 160, itemY }, itemTextSize, 1, Color{ 16, 25, 48, 255 });

        if (iconAudio.id != 0) {
            float audIconH = iconAudio.height * 0.055f;
            float audIconY = itemY + (itemTextSize - audIconH) / 2.0f;
            DrawTextureEx(iconAudio, { cx + 298, audIconY }, 0.0f, 0.055f, Color{ 16, 25, 48, 255 });
        }
        if (uiFont.texture.id != 0) DrawTextEx(uiFont, "Dolby", { cx + 346, itemY }, itemTextSize, 1, Color{ 16, 25, 48, 255 });
    }

    int rows = (locations.size() + cardsPerRow - 1) / cardsPerRow;
    currentY += rows * (cardHeight + cardGap) + 20;
}

void Spots::DrawExperiences(float& currentY) {
    Vector2 mousePos = GetMousePosition();
    int screenWidth = GetScreenWidth();
    float contentWidth = screenWidth * 0.9f;
    float startX = (screenWidth - contentWidth) / 2;

    if (uiFont.texture.id != 0) DrawTextEx(uiFont, "Vanema Experiences", { startX + 20, currentY }, 32, 1, Color{ 16, 25, 48, 255 });
    currentY += 50;

    const int cardsPerRow = 2;
    float gap = 24.0f;
    float expWidth = (contentWidth - (cardsPerRow - 1) * gap) / cardsPerRow;

    float expHeight = 240;
    Texture2D expIcons[] = { iconBeach, iconPool, iconSofa };

    for (size_t i = 0; i < experiences.size(); i++) {
        int row = i / cardsPerRow;
        int col = i % cardsPerRow;

        float ex = startX + col * (expWidth + gap);
        float ey = currentY + row * (expHeight + gap);
        Rectangle expRect = { ex, ey, expWidth, expHeight };

        DrawRectangleRounded(expRect, 0.06f, 10, WHITE);
        DrawRectangleRoundedLines(expRect, 0.06f, 10, 1.5f, Fade(LIGHTGRAY, 0.8f));

        if (expIcons[i].id != 0) DrawTextureEx(expIcons[i], { ex + 30, ey + 62 }, 0.0f, 0.11f, Color{ 16, 25, 48, 255 });

        float textX = ex + 145;
        if (uiFont.texture.id != 0) {
            DrawTextEx(uiFont, experiences[i].title.c_str(), { textX, ey + 26 }, 32, 1, Color{ 16, 25, 48, 255 });

            std::stringstream ss(experiences[i].description);
            std::string line;
            float lineYOffset = ey + 72;
            float customLineHeight = 32.0f;

            while (std::getline(ss, line, '\n')) {
                DrawTextEx(uiFont, line.c_str(), { textX, lineYOffset }, 25, 1, GRAY);
                lineYOffset += customLineHeight;
            }
        }

        float tagY = ey + 182;
        float currentTagX = textX;
        const char* tags[] = { experiences[i].tag1.c_str(), experiences[i].tag2.c_str(), experiences[i].tag3.c_str() };

        for (int j = 0; j < 3; j++) {
            if (strlen(tags[j]) == 0) continue;
            float tw = uiFont.texture.id != 0 ? MeasureTextEx(uiFont, tags[j], 28, 1).x : 60;

            if (uiFont.texture.id != 0) DrawTextEx(uiFont, tags[j], { currentTagX + 15, tagY }, 24, 1, Color{ 16, 25, 48, 255 });
            currentTagX += tw + 30;
        }
    }

    int dynamicRow = 3 / cardsPerRow;
    int dynamicCol = 3 % cardsPerRow;
    float slotX = startX + dynamicCol * (expWidth + gap);
    float slotY = currentY + dynamicRow * (expHeight + gap);
    Rectangle slotRect = { slotX, slotY, expWidth, expHeight };

    if (isLoggedIn && isAdmin) {
        bool isSlotHovered = CheckCollisionPointRec(mousePos, slotRect);
        Color slotBg = isSlotHovered ? Color{ 240, 244, 252, 255 } : WHITE;

        if (isSlotHovered) {
            SetMouseCursor(MOUSE_CURSOR_POINTING_HAND);
        }

        DrawRectangleRounded(slotRect, 0.06f, 10, slotBg);
        DrawRectangleRoundedLines(slotRect, 0.06f, 10, 1.5f, Fade(LIGHTGRAY, 0.8f));

        float midX = slotX + (expWidth / 2.0f);
        float midY = slotY + (expHeight / 2.0f);

        DrawCircleV({ midX, midY - 25 }, 26, Color{ 16, 25, 48, 255 });
        DrawLineEx({ midX - 11, midY - 25 }, { midX + 11, midY - 25 }, 4.0f, WHITE);
        DrawLineEx({ midX, midY - 36 }, { midX, midY - 14 }, 4.0f, WHITE);

        if (uiFont.texture.id != 0) {
            Vector2 sizeTxt = MeasureTextEx(uiFont, "Add Experience", 26, 1);
            DrawTextEx(uiFont, "Add Experience", { midX - (sizeTxt.x / 2.0f), midY + 18 }, 26, 1, Color{ 16, 25, 48, 255 });
        }
    }
    else {
        DrawRectangleRounded(slotRect, 0.06f, 10, Color{ 245, 247, 250, 255 });
        DrawRectangleRoundedLines(slotRect, 0.06f, 10, 1.5f, Fade(LIGHTGRAY, 0.6f));

        float midX = slotX + (expWidth / 2.0f);
        float midY = slotY + (expHeight / 2.0f);

        if (uiFont.texture.id != 0) {
            Vector2 sizeTitle = MeasureTextEx(uiFont, "Coming Soon", 34, 1);
            Vector2 sizeDesc = MeasureTextEx(uiFont, "We are designing new custom ways to watch cinema.", 22, 1);

            DrawTextEx(uiFont, "Coming Soon", { midX - (sizeTitle.x / 2.0f), midY - 25 }, 34, 1, Color{ 16, 25, 48, 255 });
            DrawTextEx(uiFont, "We are designing new custom ways to watch cinema.", { midX - (sizeDesc.x / 2.0f), midY + 20 }, 22, 1, BLACK);
        }
    }
    currentY += (2 * expHeight) + gap + 40;
}

void Spots::DrawScrollbar() {
    int screenWidth = GetScreenWidth();
    float sbHeight = 500;
    float sbWidth = 8;
    float sbX = screenWidth - 20;
    float sbY = 150;

    DrawRectangleRounded({ sbX, sbY, sbWidth, sbHeight }, 0.5f, 8, Fade(LIGHTGRAY, 0.3f));
    float thumbHeight = 120;
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

    SetMouseCursor(MOUSE_CURSOR_DEFAULT);
    BeginScissorMode((int)startX - 4, 130, (int)contentWidth + 8, GetScreenHeight() - 130);

    float trackingY = 150 - scrollOffset;
    DrawSearchBarAndFilters();
    DrawCinemaCards(trackingY, mousePos);
    DrawExperiences(trackingY);

    EndScissorMode();

    DrawScrollbar();
    DrawNavigationBar();
}