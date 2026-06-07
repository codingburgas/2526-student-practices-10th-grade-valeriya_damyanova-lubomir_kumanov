#ifndef USERSEEDER_H
#define USERSEEDER_H

#include "UserRepository.h"

class UserSeeder {
public:
    static void SeedIfEmpty(UserRepository& repo);
};

#endif