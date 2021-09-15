#include "password.h"

bool ValidatePassword(const std::string& password) {

    for (auto&i: password)
        if (i < 33 || 126 < i)
            return false;
    if (password.size() < 8 || 14 < password.size())
        return false;

    bool b = false, s = false, n = false, a = false;
    for (auto&i: password)
    {
        if ('A' <= i && i <= 'Z')
            b = true;
        else if ('a' <= i && i <= 'z')
            s = true;
        else if ('0' <= i && i <= '9')
            n = true;
        else
            a = true;
    }

    if (b+s+n+a < 3)
        return false;

    return true;
}
