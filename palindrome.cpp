#include "palindrome.h"
#include <algorithm>

bool IsPalindrome(const std::string& str) {

    std::string s = str;
    s.erase(std::remove(s.begin(), s.end(), ' '), s.end());
    std::string rs = s;
    std::reverse(rs.begin(), rs.end());

    return s == rs;
}
