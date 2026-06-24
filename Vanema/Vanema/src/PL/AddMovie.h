#ifndef ADD_MOVIE_H
#define ADD_MOVIE_H

#include "raylib.h"
#include <string>
#include "BLL/MovieService.h"

class AddMovie {
public:
    AddMovie();
    ~AddMovie();

    void Update();
    void Draw();
    void Unload();

    void SetUserData(bool loggedIn, const std::string& name, bool admin);

    int* currentScreen;
    MovieService* movieService;

private:
    void DrawNavigationBar();
    void ResetForm();

    Texture2D background;
    Texture2D logo;
    Texture2D iconHome;
    Texture2D iconMap;
    Texture2D iconFilms;
    Texture2D iconOffers;
    Texture2D iconProfile;
    Font customFont;

    int activeIndex;
    bool isLoggedIn;
    std::string userName;
    bool isAdmin;

    float scrollYOffset;
    float maxScrollY;

    int activeField;
    std::string titleInput;
    std::string plotInput;
    std::string yearInput;
    std::string genreInput;
    std::string durationInput;
    std::string languageInput;
    std::string ratingInput;
    std::string posterPathInput;
};

#endif