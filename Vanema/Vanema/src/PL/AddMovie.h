#ifndef ADDMOVIE_H
#define ADDMOVIE_H

#include "raylib.h"
#include <string>
#include "BLL/MovieService.h"
class Films;

class AddMovie {
public:
    int* currentScreen;
    MovieService* movieService;
    Films* filmsScreen;  

    AddMovie();
    ~AddMovie();

    void Update();
    void Draw();
    void SetUserData(bool loggedIn, const std::string& name, bool admin);
    void ResetForm();

    bool ShouldRefreshMovies() const;
    void SetRefreshMovies(bool refresh);

private:
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

    std::string titleInput;
    std::string plotInput;
    std::string yearInput;
    std::string genreInput;
    std::string durationInput;
    std::string languageInput;
    std::string ratingInput;
    std::string posterPathInput;
    int activeField;

    bool isTransitioning;
    int transitionFrames;

    bool shouldRefreshMovies;

    void Unload();
    void ConsumeMouseClicks();
    void DrawNavigationBar();
};

#endif 