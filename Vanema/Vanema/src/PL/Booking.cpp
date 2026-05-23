#include "Booking.h"

Booking::Booking()
{
    background = LoadTexture("assets/booking.png");
    logo = LoadTexture("assets/logo.png");
    iconHome = LoadTexture("assets/icon_home.png");
    iconMap = LoadTexture("assets/icon_map.png");
    iconFilms = LoadTexture("assets/icon_films.png");
    iconOffers = LoadTexture("assets/icon_offers.png");
    iconProfile = LoadTexture("assets/icon_profile.png");
    customFont = LoadFont("assets/PlayfairDisplay-Medium.ttf");
}

Booking::~Booking()
{
    Unload();
}

void Booking::Unload()
{
    UnloadTexture(background);
    UnloadTexture(logo);
    UnloadTexture(iconHome);
    UnloadTexture(iconMap);
    UnloadTexture(iconFilms);
    UnloadTexture(iconOffers);
    UnloadTexture(iconProfile);
}

void Booking::Update()
{
}

void Booking::Draw()
{
    int screenWidth = GetScreenWidth();
    int screenHeight = GetScreenHeight();

    DrawTexturePro(
        background,
        { 0, 0, (float)background.width, (float)background.height },
        { 0, 0, (float)screenWidth, (float)screenHeight },
        { 0,0 },
        0.0f,
        WHITE
    );

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

    DrawTextureEx(
        logo,
        { navBarRect.x - 2, navBarRect.y - 20 },
        0.1f,
        0.3f,
        WHITE
    );

    DrawTextEx(
        customFont,
        "Vanema",
        { navBarRect.x + 130, navBarRect.y + 40 },
        34,
        1,
        BLACK
    );

    const char* labels[] =
    {
        "Home",
        "Map",
        "Films",
        "Offers"
    };

    Texture2D icons[] =
    {
        iconHome,
        iconMap,
        iconFilms,
        iconOffers
    };

    float spacing = 94.0f;
    float startX = navBarRect.x + 930;

    SetMouseCursor(MOUSE_CURSOR_DEFAULT);

    for (int i = 0; i < 4; i++)
    {
        float itemX = startX + (i * spacing);

        Rectangle btnRect =
        {
            itemX,
            navBarRect.y,
            110,
            navHeight
        };

        Vector2 mousePos = GetMousePosition();

        bool isHovered =
            CheckCollisionPointRec(mousePos, btnRect);

        Color tint =
            (i == activeIndex) ? BLUE : DARKBLUE;

        if (isHovered)
        {
            SetMouseCursor(MOUSE_CURSOR_POINTING_HAND);

            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
            {
                activeIndex = i;
            }
        }

        DrawTextureEx(
            icons[i],
            { itemX + 22, navBarRect.y + 5 },
            0.0f,
            0.1f,
            tint
        );

        DrawTextEx(
            customFont,
            labels[i],
            { itemX + 30, navBarRect.y + 70 },
            20,
            1,
            BLACK
        );
    }

    Rectangle profileRect =
    {
        navBarRect.x + navWidth - 70,
        navBarRect.y + 15,
        50,
        50
    };

    Vector2 mousePos = GetMousePosition();
    bool isProfileHovered = CheckCollisionPointRec(mousePos, profileRect);

    if (isProfileHovered)
    {
        SetMouseCursor(MOUSE_CURSOR_POINTING_HAND);

        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
        {
            if (currentScreen != nullptr)
            {
                *currentScreen = 1;
            }
        }
    }

    DrawTextureEx(
        iconProfile,
        { profileRect.x - 15, profileRect.y + 10 },
        0.0f,
        0.1f,
        isProfileHovered ? BLUE : DARKBLUE
    );
}