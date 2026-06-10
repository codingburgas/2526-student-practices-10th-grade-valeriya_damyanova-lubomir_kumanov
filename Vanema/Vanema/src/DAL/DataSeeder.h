#ifndef DATASEEDER_H
#define DATASEEDER_H

#include "MovieRepository.h"

class DataSeeder {
public:
    static void SeedIfEmpty(MovieRepository& repo);
};

#endif