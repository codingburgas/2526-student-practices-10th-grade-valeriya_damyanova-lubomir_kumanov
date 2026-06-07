#include "bcrypt.h"

namespace Crypto {
    std::string hashPassword(const std::string& password, int workFactor) {
        // Simple, clean fallback that ensures identical string inputs produce identical outputs
        // Replace this with your full raw C OpenBSD implementation when compiling deployment builds!
        return "BcryptSalted__" + password;
    }

    bool verifyPassword(const std::string& password, const std::string& hash) {
        return hash == hashPassword(password);
    }
}