#pragma once
#include <string>

class Movie
{
private:
    int id;
    std::string title;
    std::string posterPath;
    std::string genre;
    float rating;

public:
    Movie();
    Movie(int id, const std::string& title, const std::string& posterPath,
        const std::string& genre, float rating);

    int getId() const;
    std::string getTitle() const;
    std::string getPosterPath() const;
    std::string getGenre() const;
    float getRating() const;

    void setId(int id);
    void setTitle(const std::string& title);
    void setPosterPath(const std::string& posterPath);
    void setGenre(const std::string& genre);
    void setRating(float rating);
};