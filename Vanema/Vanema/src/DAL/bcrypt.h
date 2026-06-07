#ifndef BCRYPT_H
#define BCRYPT_H

#include <string>

namespace Crypto {
    // Hashes a plain-text password using bcrypt. 
    // Higher workFactor = more secure but slower (10-12 is standard).
    std::string hashPassword(const std::string& password, int workFactor = 10);

    // Compares a plain-text password with an existing bcrypt hash string.
    bool verifyPassword(const std::string& password, const std::string& hash);
}

#endif // BCRYPT_H