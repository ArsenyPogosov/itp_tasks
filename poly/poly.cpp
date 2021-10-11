#include "poly.h"

#include <algorithm>

Poly::Poly() {
}

Poly::Poly(std::vector<int64_t> coefficients) {
    for (size_t i = 0; i < coefficients.size(); ++i) {
        if (coefficients[i]) {
            coefficients_[i] = coefficients[i];
        }
    }
}

Poly::Poly(std::vector<std::pair<int64_t, int64_t>> coefficients) {
    for (const auto& [i, ai] : coefficients) {
        if (ai) {
            coefficients_[i] = ai;
        }
    }
}

int64_t Poly::operator()(int64_t x) const {
    int64_t result = 0;

    const auto quick_power = [](int64_t x, int64_t k) {
        int64_t result = 1;
        for (int64_t i = 0, pi = x; (1 << i) <= k; ++i, pi *= pi) {
            if ((k >> i) & 1) {
                result *= pi;
            }
        }

        return result;
    };

    for (const auto& [i, ai] : coefficients_) {
        result += ai * quick_power(x, i);
    }

    return result;
}

bool Poly::operator==(const Poly& second) const {
    return coefficients_ == second.coefficients_;
}

bool Poly::operator!=(const Poly& second) const {
    return coefficients_ != second.coefficients_;
}

Poly Poly::operator+() const {
    return *this;
}

Poly Poly::operator-() const {
    Poly result;
    result.coefficients_ = coefficients_;
    for (auto& [i, ai] : result.coefficients_) {
        ai = -ai;
    }
    return result;
}

Poly Poly::operator+(const Poly& second) const {
    Poly result;
    result.coefficients_ = coefficients_;
    for (const auto& [i, ai] : second.coefficients_) {
        result.coefficients_[i] += ai;
        if (!result.coefficients_[i]) {
            result.coefficients_.erase(i);
        }
    }

    return result;
}

Poly Poly::operator-(const Poly& second) const {
    Poly result;
    result.coefficients_ = coefficients_;
    for (const auto& [i, ai] : second.coefficients_) {
        result.coefficients_[i] -= ai;
        if (!result.coefficients_[i]) {
            result.coefficients_.erase(i);
        }
    }

    return result;
}

Poly& Poly::operator+=(const Poly& second) {
    for (const auto& [i, ai] : second.coefficients_) {
        coefficients_[i] += ai;
        if (!coefficients_[i]) {
            coefficients_.erase(i);
        }
    }

    return *this;
}

Poly& Poly::operator-=(const Poly& second) {
    for (const auto& [i, ai] : second.coefficients_) {
        coefficients_[i] -= ai;
        if (!coefficients_[i]) {
            coefficients_.erase(i);
        }
    }

    return *this;
}

Poly Poly::operator*(const Poly& second) const {
    Poly result;

    for (const auto& [i, ai] : coefficients_) {
        for (const auto& [j, bj] : second.coefficients_) {
            result.coefficients_[i + j] += ai * bj;
            if (!result.coefficients_[i]) {
                result.coefficients_.erase(i);
            }
        }
    }

    return result;
}

Poly& Poly::operator*=(const Poly& second) {
    return *this = (*this) * second;
}

std::ostream& operator<<(std::ostream& out, const Poly& poly) {
    out << "y = ";

    if (poly.coefficients_.empty()) {
        return out << "0";
    }

    std::vector<std::pair<int64_t, int64_t>> sorted_coefficients;
    for (const auto& coefficient : poly.coefficients_) {
        sorted_coefficients.push_back(coefficient);
    }
    std::sort(sorted_coefficients.begin(), sorted_coefficients.end(), std::greater<std::pair<int64_t, int64_t>>());

    for (const auto& [i, ai] : sorted_coefficients) {
        if (i != sorted_coefficients.front().first) {
            if (ai > 0) {
                out << " + ";
            } else {
                out << " - ";
            }
        } else {
            if (ai < 0) {
                out << '-';
            }
        }

        if (std::abs(ai) != 1 || i == 0) {
            out << std::abs(ai);
        }

        if (i != 0) {
            out << 'x';
        }

        if (i >= 2) {
            out << '^' << i;
        }
    }

    return out;
}
