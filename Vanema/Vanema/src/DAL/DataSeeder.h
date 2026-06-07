#ifndef DATASEEDER_H
#define DATASEEDER_H

#include "MovieRepository.h"

class DataSeeder {
public:
    // Checks if the database is empty and seeds it if necessary
    static void SeedIfEmpty(MovieRepository& repo);
};

#endif