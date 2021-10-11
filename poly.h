#pragma once

#include <cstdint>
#include <vector>
#include <unordered_map>
#include <iostream>

class Poly {
public:
    Poly();
    Poly(std::vector<int64_t> coefficients);
    Poly(std::vector<std::pair<int64_t, int64_t>> coefficients);
    Poly(const Poly& from) = default;
    Poly(Poly&& from) = default;

    Poly& operator=(const Poly& from) = default;
    Poly& operator=(Poly&& from) = default;

    int64_t operator()(int64_t x) const;

    bool operator==(const Poly& second) const;
    bool operator!=(const Poly& second) const;

    Poly operator+() const;
    Poly operator-() const;
    Poly operator+(const Poly& second) const;
    Poly operator-(const Poly& second) const;

    Poly& operator+=(const Poly& second);
    Poly& operator-=(const Poly& second);

    Poly operator*(const Poly& second) const;

    Poly& operator*=(const Poly& second);

    friend std::ostream& operator<<(std::ostream& out, const Poly& poly);

private:
    std::unordered_map<int64_t, int64_t> coefficients_;
};
