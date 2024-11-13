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

/*
 * BigInteger
 */

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

    // more operators
    double sqrt() const;
#if SUPPORT_MORE_OPS == 1
    BigInteger isqrt() const;
    bool is_prime(size_t k) const;
#endif

private:
    // realization
    std::string value;
    bool sign;

    // friends
    friend std::ostream& operator<<(std::ostream& lhs, const BigInteger& rhs);
    friend std::istream& operator>>(std::istream& lhs, BigInteger& rhs);

    friend bool operator==(const BigInteger& lhs, const BigInteger& rhs);
    friend bool operator!=(const BigInteger& lhs, const BigInteger& rhs);
    friend bool operator<(const BigInteger& lhs, const BigInteger& rhs);
    friend bool operator>(const BigInteger& lhs, const BigInteger& rhs);
    friend bool operator<=(const BigInteger& lhs, const BigInteger& rhs);
    friend bool operator>=(const BigInteger& lhs, const BigInteger& rhs);

    // assistants
    void removeLeadingZeros();
    static std::string subtractStrings(const std::string& a, const std::string& b);
    static std::string addStrings(const std::string& a, const std::string& b);
    void divisionAndModulus(const BigInteger& rhs, BigInteger& quotient, BigInteger& remainder) const;
};

/* Constructors */

inline BigInteger::BigInteger() : value("0"), sign(true) {}

inline BigInteger::BigInteger(int64_t n)
{
    value = std::to_string(std::abs(n));
    sign = (n >= 0);
}

inline BigInteger::BigInteger(const std::string& str)
{
    if (str.empty())
    {
        throw std::invalid_argument("empty string");
    }

    size_t pos = 0;

    if (str[0] == '-')
    {
        sign = false;
        pos = 1;
    }
    else
    {
        if (str[0] == '+')
        {
            pos = 1;
        }
        sign = true;
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

/* Unary operators */

inline const BigInteger& BigInteger::operator+() const
{
    return *this;
}

inline BigInteger BigInteger::operator-() const
{
    BigInteger result = *this;

    if (result.value != "0")
    {
        result.sign = !sign;
    }

    return result;
}

/* Binary arithmetics operators */

inline BigInteger& BigInteger::operator+=(const BigInteger& rhs) {
    if (sign == rhs.sign)
    {
        value = addStrings(value, rhs.value);
    }
    else
    {
        if (value == rhs.value)
        {
            value = "0";
            sign = true;
        }
        else if (value.size() > rhs.value.size() || (value.size() == rhs.value.size() && value > rhs.value))
        {
            value = subtractStrings(value, rhs.value);
        }
        else
        {
            value = subtractStrings(rhs.value, value);
            sign = rhs.sign;
        }
    }

    removeLeadingZeros();
    return *this;
}

inline BigInteger operator+(BigInteger lhs, const BigInteger& rhs)
{
    lhs += rhs;
    return lhs;
}

inline BigInteger& BigInteger::operator-=(const BigInteger& rhs) {
    if (sign != rhs.sign)
    {
        value = addStrings(value, rhs.value);
    }
    else
    {
        if (value == rhs.value)
        {
            value = "0";
            sign = true;
        }
        else if (value.size() > rhs.value.size() || (value.size() == rhs.value.size() && value > rhs.value))
        {
            value = subtractStrings(value, rhs.value);
        }
        else
        {
            value = subtractStrings(rhs.value, value);
            sign = !sign;
        }
    }

    removeLeadingZeros();
    return *this;
}

inline BigInteger operator-(BigInteger lhs, const BigInteger& rhs)
{
    lhs -= rhs;
    return lhs;
}

inline BigInteger& BigInteger::operator*=(const BigInteger& rhs) {
    if (value == "0" || rhs.value == "0")
    {
        value = "0";
        sign = true;
        return *this;
    }

    sign = (sign == rhs.sign);

    std::string result(value.size() + rhs.value.size(), '0');

    for (int i = value.size() - 1; i >= 0; i--)
    {
        int carry = 0;
        int num1 = value[i] - '0';
        for (int j = rhs.value.size() - 1; j >= 0; j--)
        {
            int num2 = rhs.value[j] - '0';
            int sum = (result[i + j + 1] - '0') + num1 * num2 + carry;
            carry = sum / 10;
            result[i + j + 1] = (sum % 10) + '0';
        }
        result[i] += carry;
    }

    value = result;
    removeLeadingZeros();
    return *this;
}

inline BigInteger operator*(BigInteger lhs, const BigInteger& rhs)
{
    lhs *= rhs;
    return lhs;
}

inline BigInteger& BigInteger::operator/=(const BigInteger& rhs) {
    BigInteger quotient;
    BigInteger remainder;
    divisionAndModulus(rhs, quotient, remainder);

    *this = quotient;
    return *this;
}

inline BigInteger operator/(BigInteger lhs, const BigInteger& rhs)
{
    lhs /= rhs;
    return lhs;
}

inline BigInteger& BigInteger::operator%=(const BigInteger& rhs) {
    BigInteger quotient;
    BigInteger remainder;
    divisionAndModulus(rhs, quotient, remainder);

    *this = remainder;
    return *this;
}

inline BigInteger operator%(BigInteger lhs, const BigInteger& rhs)
{
    lhs %= rhs;
    return lhs;
}

/* Logical operators */

inline bool operator==(const BigInteger& lhs, const BigInteger& rhs)
{
    return lhs.sign == rhs.sign && lhs.value == rhs.value;
}

inline bool operator!=(const BigInteger& lhs, const BigInteger& rhs)
{
    return !(lhs == rhs);
}

inline bool operator<(const BigInteger& lhs, const BigInteger& rhs)
{
    if (lhs.sign != rhs.sign)
    {
        return !lhs.sign;
    }

    if (lhs.sign)
    {
        if (lhs.value.size() != rhs.value.size())
        {
            return lhs.value.size() < rhs.value.size();
        }
        return lhs.value < rhs.value;
    }

    if (lhs.value.size() != rhs.value.size())
    {
        return lhs.value.size() > rhs.value.size();
    }
    return lhs.value > rhs.value;
}

inline bool operator>(const BigInteger& lhs, const BigInteger& rhs)
{
    return rhs < lhs;
}

inline bool operator<=(const BigInteger& lhs, const BigInteger& rhs)
{
    return !(rhs < lhs);
}

inline bool operator>=(const BigInteger& lhs, const BigInteger& rhs)
{
    return !(lhs < rhs);
}

/* Friends */

inline std::ostream& operator<<(std::ostream& lhs, const BigInteger& rhs)
{
    if (!rhs.sign)
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
    if (input[0] == '-')
    {
        rhs.sign = false;
        pos = 1;
    }
    else
    {
        if (input[0] == '+')
        {
            pos = 1;
        }
        rhs.sign = true;
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

/* More operators */

#if SUPPORT_EVAL == 1
inline BigInteger eval(const std::string&);
#endif

/* Assistants */

inline void BigInteger::removeLeadingZeros()
{
    size_t pos = value.find_first_not_of('0');

    if (pos == std::string::npos)
    {
        value = "0";
        sign = true;
    }
    else
    {
        value.erase(0, pos);
    }
}

inline std::string BigInteger::addStrings(const std::string& a, const std::string& b)
{
    std::string result;

    int carry = 0;
    int sum = 0;

    int a_len = a.size();
    int b_len = b.size();

    for (int i = 0; i < std::max(a_len, b_len) || carry; i++)
    {
        int digitA = i < a_len ? a[a_len - i - 1] - '0' : 0;
        int digitB = i < b_len ? b[b_len - i - 1] - '0' : 0;
        sum = digitA + digitB + carry;
        result += (sum % 10) + '0';
        carry = sum / 10;
    }

    std::reverse(result.begin(), result.end());
    return result;
}

inline std::string BigInteger::subtractStrings(const std::string& a, const std::string& b)
{
    std::string result;

    int borrow = 0;
    int difference = 0;

    int a_len = a.size();
    int b_len = b.size();

    for (int i = 0; i < a_len; i++)
    {
        int digitA = a[a_len - i - 1] - '0';
        int digitB = i < b_len ? b[b_len - i - 1] - '0' : 0;
        difference = digitA - digitB - borrow;

        if (difference < 0)
        {
            difference += 10;
            borrow = 1;
        }
        else
        {
            borrow = 0;
        }

        result += difference + '0';
    }

    std::reverse(result.begin(), result.end());
    return result;
}

inline void BigInteger::divisionAndModulus(const BigInteger& rhs, BigInteger& quotient, BigInteger& remainder) const
{
    if (rhs.value == "0")
    {
        throw std::logic_error("zero division");
    }

    if (value == "0")
    {
        quotient = BigInteger(0);
        remainder = BigInteger(0);
        return;
    }

    BigInteger dividend = *this;
    dividend.sign = true;

    BigInteger divisor = rhs;
    divisor.sign = true;

    if (dividend < divisor)
    {
        quotient = BigInteger(0);
        remainder = dividend;
        return;
    }

    std::string result;
    BigInteger current;

    for (char i : dividend.value)
    {
        current.value += i;
        current.removeLeadingZeros();

        int count = 0;
        while (current >= divisor)
        {
            current -= divisor;
            ++count;
        }
        result += (count + '0');
    }

    quotient.value = result;
    quotient.sign = (sign == rhs.sign);
    quotient.removeLeadingZeros();

    remainder = current;
    remainder.sign = sign;
    remainder.removeLeadingZeros();
}

/*
 * BigRational
 */

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
