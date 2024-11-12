/*
 * bignumbers.h
 */

#pragma once

#include <string>
#include <cstdint>
#include <stdexcept>

#define SUPPORT_IFSTREAM 1
#define SUPPORT_MORE_OPS 1
#define SUPPORT_EVAL 1

/* BigInteger */

class BigInteger
{
public:
    // constructors
    BigInteger();
    explicit BigInteger(int64_t n);
    explicit BigInteger(const std::string& str);
    // copy
    BigInteger(const BigInteger& other);
    BigInteger& operator=(const BigInteger& rhs);
    // unary operators
    const BigInteger& operator+() const;
    BigInteger operator-() const;
    // binary arithmetics operators
    BigInteger& operator+=(const BigInteger& rhs);
    BigInteger& operator-=(const BigInteger& rhs);
    BigInteger& operator*=(const BigInteger& rhs);
    BigInteger& operator/=(const BigInteger& rhs);
    BigInteger& operator%=(const BigInteger& rhs);

    double sqrt() const;
#if SUPPORT_MORE_OPS == 1
    BigInteger isqrt() const;
    bool is_prime(size_t k) const;
#endif
private:
    std::string value;
    char sign;

    // friends
    friend std::ostream& operator<<(std::ostream& lhs, const BigInteger& rhs);
    friend std::istream& operator>>(std::istream& lhs, BigInteger& rhs);

    // assistants
    void removeLeadingZeros();
};

/* Constructors */

inline BigInteger::BigInteger() : value("0"), sign('+') {}

inline BigInteger::BigInteger(int64_t n)
{
    value = std::to_string(std::abs(n));
    sign = (n < 0) ? '-' : '+';
}

inline BigInteger::BigInteger(const std::string& str)
{
    if (str.empty())
    {
        throw std::invalid_argument("empty string");
    }

    size_t pos = 0;
    if (str[0] == '-' || str[0] == '+')
    {
        sign = str[0];
        pos = 1;
    }
    else
    {
        sign = '+';
    }

    value = str.substr(pos);

    if (value.empty() || value.find_first_not_of("0123456789") != std::string::npos)
    {
        throw std::invalid_argument("invalid number format");
    }

    removeLeadingZeros();
}

/* Copy */

inline BigInteger::BigInteger(const BigInteger& other) = default;

inline BigInteger& BigInteger::operator=(const BigInteger& rhs) {
    if (this != &rhs)
    {
        value = rhs.value;
        sign = rhs.sign;
    }
    return *this;
}

/* Assistants */

inline void BigInteger::removeLeadingZeros()
{
    size_t pos = value.find_first_not_of('0');

    if (pos == std::string::npos)
    {
        value = "0";
        sign = '+';
    }
    else
    {
        value.erase(0, pos);
    }
}

inline BigInteger operator+(BigInteger lhs, const BigInteger& rhs);
inline BigInteger operator-(BigInteger lhs, const BigInteger& rhs);
inline BigInteger operator*(BigInteger lhs, const BigInteger& rhs);
inline BigInteger operator/(BigInteger lhs, const BigInteger& rhs);
inline BigInteger operator%(BigInteger lhs, const BigInteger& rhs);

inline bool operator==(const BigInteger& lhs, const BigInteger& rhs);
inline bool operator!=(const BigInteger& lhs, const BigInteger& rhs);
inline bool operator<(const BigInteger& lhs, const BigInteger& rhs);
inline bool operator>(const BigInteger& lhs, const BigInteger& rhs);
inline bool operator<=(const BigInteger& lhs, const BigInteger& rhs);
inline bool operator>=(const BigInteger& lhs, const BigInteger& rhs);

/* Friends */

inline std::ostream& operator<<(std::ostream& lhs, const BigInteger& rhs)
{
    if (rhs.sign == '-')
    {
        lhs << '-';
    }

    lhs << rhs.value;

    return lhs;
}

#if SUPPORT_IFSTREAM == 1
inline std::istream& operator>>(std::istream& lhs, BigInteger& rhs)
{
    std::string input;
    lhs >> input;

    if (input.empty())
    {
        lhs.setstate(std::ios::failbit);
        return lhs;
    }

    size_t pos = 0;
    if (input[0] == '-' || input[0] == '+')
    {
        rhs.sign = input[0];
        pos = 1;
    }
    else
    {
        rhs.sign = '+';
    }

    rhs.value = input.substr(pos);
    if (rhs.value.empty() || rhs.value.find_first_not_of("0123456789") != std::string::npos)
    {
        lhs.setstate(std::ios::failbit);
        rhs = BigInteger(0);
        return lhs;
    }

    rhs.removeLeadingZeros();

    return lhs;
}
#endif

/* BigRational */

class BigRational
{
public:
    // constructors
    BigRational();
    BigRational(int64_t a, int64_t b);
    BigRational(const std::string& a, const std::string& b);
    // copy
    BigRational(const BigRational& other);
    BigRational& operator=(const BigRational& rhs);
    // unary operators
    const BigRational& operator+() const;
    BigRational operator-() const;
    // binary arithmetics operators
    BigRational& operator+=(const BigRational& rhs);
    BigRational& operator-=(const BigRational& rhs);
    BigRational& operator*=(const BigRational& rhs);
    BigRational& operator/=(const BigRational& rhs);

    double sqrt() const;
#if SUPPORT_MORE_OPS == 1
    BigInteger isqrt() const;
#endif
private:

};

inline BigRational operator+(BigRational lhs, const BigRational& rhs);
inline BigRational operator-(BigRational lhs, const BigRational& rhs);
inline BigRational operator*(BigRational lhs, const BigRational& rhs);
inline BigRational operator/(BigRational lhs, const BigRational& rhs);

inline bool operator==(const BigRational& lhs, const BigRational& rhs);
inline bool operator!=(const BigRational& lhs, const BigRational& rhs);
inline bool operator<(const BigRational& lhs, const BigRational& rhs);
inline bool operator>(const BigRational& lhs, const BigRational& rhs);
inline bool operator<=(const BigRational& lhs, const BigRational& rhs);
inline bool operator>=(const BigRational& lhs, const BigRational& rhs);

inline std::ostream& operator<<(std::ostream& lhs, const BigRational& rhs);

#if SUPPORT_IFSTREAM == 1
inline std::istream& operator>>(std::istream& lhs, BigRational& rhs);
#endif

#if SUPPORT_EVAL == 1
inline BigInteger eval(const std::string&);
#endif