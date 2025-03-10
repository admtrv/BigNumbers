/*
 * bignumbers.h
 */

#pragma once

#include <string>
#include <cstdint>
#include <stdexcept>
#include <valarray>
#include <random>
#include <compare>
#include <limits>

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
    BigInteger(int64_t n);
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
#if SUPPORT_IFSTREAM == 1
    friend bool readBigInteger(std::istream& in, BigInteger& x);
    friend std::istream& operator>>(std::istream& lhs, BigInteger& rhs);
#endif

    friend bool operator==(const BigInteger& lhs, const BigInteger& rhs);
    friend bool operator!=(const BigInteger& lhs, const BigInteger& rhs);
    friend bool operator<(const BigInteger& lhs, const BigInteger& rhs);
    friend bool operator>(const BigInteger& lhs, const BigInteger& rhs);
    friend bool operator<=(const BigInteger& lhs, const BigInteger& rhs);
    friend bool operator>=(const BigInteger& lhs, const BigInteger& rhs);

    friend class BigRational;

    // assistants
    void removeLeadingZeros();
    static std::string subtractStrings(const std::string& a, const std::string& b);
    static std::string addStrings(const std::string& a, const std::string& b);
    void divisionAndModulus(const BigInteger& rhs, BigInteger& quotient, BigInteger& remainder) const;
    static BigInteger modulusPower(const BigInteger& base, const BigInteger& exponent, const BigInteger& modulus);
    static BigInteger randomRange(const BigInteger& low, const BigInteger& high);
    static size_t bitLength(const BigInteger& number);
};

/* Constructors */

inline BigInteger::BigInteger() : value("0"), sign(true) {}

inline BigInteger::BigInteger(int64_t n)
{
    if (n == std::numeric_limits<int64_t>::min())
    {
        value = "9223372036854775808";
        sign  = false;
    }
    else if (n < 0)
    {
        int64_t tmp = -n;
        value = std::to_string(tmp);
        sign  = false;
    }
    else
    {
        value = std::to_string(n);
        sign  = true;
    }
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

inline bool readBigInteger(std::istream& in, BigInteger& x)
{
    x = BigInteger(0);

    std::istream::sentry s(in);
    if (!s)
    {
        in.setstate(std::ios::failbit);
        return false;
    }

    bool negative = false;
    bool found_digit = false;

    std::string digits;
    digits.reserve(128);

    int c = in.peek();      // sign
    if (c == '+' || c == '-')
    {
        negative = (c == '-');
        in.ignore(1);
    }

    while (true)    // digits
    {
        c = in.peek();

        if (!std::isdigit(c))
        {
            break;
        }

        digits.push_back(static_cast<char>(c));
        found_digit = true;
        in.ignore(1);
    }

    if (!found_digit)
    {
        in.setstate(std::ios::failbit);
        return false;
    }

    x.value = digits;
    x.sign = !negative;
    x.removeLeadingZeros();

    if (x.value == "0")
    {
        x.sign = true;
    }

    return true;
}

inline std::istream& operator>>(std::istream& lhs, BigInteger& rhs)
{
    bool success = readBigInteger(lhs, rhs);
    if (!success)
    {
        if (!lhs.fail())
        {
            lhs.setstate(std::ios::failbit);
        }
    }
    return lhs;
}

#endif

/* More operators */

inline double BigInteger::sqrt() const
{
    if (!sign)
    {
        throw std::runtime_error("negative number");
    }

    double result = 0;

    try {
        result = std::stod(value);
        if (std::isinf(result))
        {
            throw std::runtime_error("large number");
        }
    } catch (const std::out_of_range&) {
        throw std::runtime_error("large number");
    }

    return std::sqrt(result);
}

#if SUPPORT_MORE_OPS == 1

inline BigInteger BigInteger::isqrt() const
{
    if (!sign)
    {
        throw std::runtime_error("negative number");
    }

    if (*this == BigInteger(0) || *this == BigInteger(1))
    {
        return *this;
    }

    BigInteger low(1);
    BigInteger high = *this;
    BigInteger result(0);

    while (low <= high)
    {
        BigInteger mid = (low + high) / BigInteger(2);
        BigInteger square = mid * mid;

        if (square == *this)
        {
            return mid;
        }
        else if (square < *this)
        {
            low = mid + BigInteger(1);
            result = mid;
        }
        else
        {
            high = mid - BigInteger(1);
        }
    }

    return result;
}

inline bool BigInteger::is_prime(size_t k) const {
    if (*this <= BigInteger(1))
    {
        return false;
    }

    if (*this == BigInteger(2) || *this == BigInteger(3))
    {
        return true;
    }

    if (*this % BigInteger(2) == BigInteger(0))
    {
        return false;
    }

    BigInteger n_minus_one = *this - BigInteger(1);
    BigInteger d = n_minus_one;
    size_t s = 0;


    while (d % BigInteger(2) == BigInteger(0))
    {
        d /= BigInteger(2);
        s++;
    }

    for (size_t i = 0; i < k; i++)
    {
        BigInteger a = randomRange(BigInteger(2), *this - BigInteger(2));
        BigInteger x = modulusPower(a, d, *this);

        if (x == BigInteger(1) || x == n_minus_one)
        {
            continue;
        }

        bool is_composite = true;

        for (size_t r = 1; r < s; r++)
        {
            x = (x * x) % *this;
            if (x == n_minus_one)
            {
                is_composite = false;
                break;
            }
        }

        if (is_composite) return false;
    }

    return true;
}

#endif

#if SUPPORT_EVAL == 1

inline std::string removeSpaces(const std::string& string)  // string = "{ \"op\" : \"+\", \"left\" : 123, \"right\" : 456 }"
{
    std::string result;

    for (char ch : string)
    {
        if (!std::isspace(ch))
        {
            result += ch;
        }
    }
    return result;  // result = "{\"op\":\"+\",\"left\":123,\"right\":456}"
}


inline std::string findValue(const std::string& json, const std::string& key, size_t& pos)
{
    size_t key_pos = json.find("\"" + key + "\":", pos);

    if (key_pos == std::string::npos)
    {
        throw std::invalid_argument(key);
    }

    size_t value_pos = key_pos + key.length() + 3;

    if (json[value_pos] == '{')
    {
        // object
        size_t brace_count = 1;
        size_t end_pos = value_pos + 1;
        while (brace_count > 0 && end_pos < json.length())
        {
            if (json[end_pos] == '{')
            {
                brace_count += 1;
            }
            else if (json[end_pos] == '}')
            {
                brace_count-=1;
            }
            end_pos+=1;
        }
        if (brace_count != 0)
        {
            throw std::invalid_argument("end brace");
        }

        pos = end_pos;
        return json.substr(value_pos, end_pos - value_pos);
    }
    else if (json[value_pos] == '"')
    {
        // string
        size_t end_pos = json.find('"', value_pos + 1);

        if (end_pos == std::string::npos)
        {
            throw std::invalid_argument("end quotation mark");
        }

        pos = end_pos + 1;
        return json.substr(value_pos + 1, end_pos - value_pos - 1);
    }
    else
    {
        // number
        size_t end_pos = value_pos;
        while (end_pos < json.length() && (std::isdigit(json[end_pos]) || json[end_pos] == '-' || json[end_pos] == '.'))
        {
            end_pos +=1 ;
        }

        pos = end_pos;
        return json.substr(value_pos, end_pos - value_pos);
    }
}

inline BigInteger evaluate(const std::string& json)
{
    std::string json_format = removeSpaces(json);

    if (json_format.empty())
    {
        throw std::invalid_argument("empty json");
    }

    if (json_format[0] != '{')
    {
        // string or number
        if (json_format[0] == '"')
        {
            // string
            if (json_format.back() != '"')
            {
                throw std::invalid_argument("end string");
            }

            std::string value = json_format.substr(1, json_format.length() - 2);
            return BigInteger(value);
        }
        else
        {
            // number
            size_t dot_pos = json_format.find('.'); // remove decimal part

            std::string integer_part = (dot_pos == std::string::npos) ? json_format : json_format.substr(0, dot_pos);
            return BigInteger(integer_part);
        }
    }
    // object with "op", "left", "rigth" in {}

    size_t pos = 1;

    // op
    std::string op = findValue(json_format, "op", pos);

    // left
    std::string left_str = findValue(json_format, "left", pos);
    BigInteger left_value = evaluate(left_str);

    // right
    std::string right_str = findValue(json_format, "right", pos);
    BigInteger right_value = evaluate(right_str);

    if (op == "+")
    {
        return left_value + right_value;
    }
    else if (op == "-")
    {
        return left_value - right_value;
    }
    else if (op == "*")
    {
        return left_value * right_value;
    }
    else if (op == "/")
    {
        return left_value / right_value;
    }
    else if (op == "%")
    {
        return left_value % right_value;
    }
    else
    {
        throw std::invalid_argument("unknown operator");
    }
}


inline BigInteger eval(const std::string& input)
{
    return evaluate(input);
}

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
        int digit_a = i < a_len ? a[a_len - i - 1] - '0' : 0;
        int digit_b = i < b_len ? b[b_len - i - 1] - '0' : 0;
        sum = digit_a + digit_b + carry;
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
        int digit_a = a[a_len - i - 1] - '0';
        int digit_b = i < b_len ? b[b_len - i - 1] - '0' : 0;
        difference = digit_a - digit_b - borrow;

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

inline BigInteger BigInteger::modulusPower(const BigInteger& base, const BigInteger& exponent, const BigInteger& modulus)
{
    if (modulus == BigInteger(0))
    {
        throw std::invalid_argument("zero modulus");
    }

    BigInteger result(1);
    BigInteger b = base % modulus;
    BigInteger e = exponent;

    while (e > BigInteger(0))
    {
        if (e % BigInteger(2) == BigInteger(1))
        {
            result = (result * b) % modulus;
        }
        e /= BigInteger(2);
        b = (b * b) % modulus;
    }

    return result;
}

inline BigInteger BigInteger::randomRange(const BigInteger& low, const BigInteger& high)
{
    if (low > high)
    {
        throw std::invalid_argument("low bound");
    }

    BigInteger range = high - low + BigInteger(1);
    BigInteger randomNumber;

    std::mt19937_64 rng(std::random_device{}());
    std::uniform_int_distribution<int> bit_dist(0, 1);

    do
    {
        randomNumber = BigInteger(0);
        for (size_t i = 0; i < bitLength(range); i++)
        {
            randomNumber = randomNumber * BigInteger(2) + BigInteger(bit_dist(rng));
        }
    }
    while (randomNumber >= range);

    return low + randomNumber;
}

inline size_t BigInteger::bitLength(const BigInteger& number)
{
    BigInteger temp = number;
    size_t bits = 0;

    while (temp > BigInteger(0))
    {
        temp /= BigInteger(2);
        bits++;
    }
    return bits;
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
    // realization
    BigInteger numerator;
    BigInteger denominator;

    // friends
    friend std::ostream& operator<<(std::ostream& lhs, const BigRational& rhs);
#if SUPPORT_IFSTREAM == 1
    friend std::istream& operator>>(std::istream& lhs, BigRational& rhs);
#endif

    friend std::strong_ordering operator<=>(const BigRational& lhs, const BigRational& rhs);
    friend bool operator==(const BigRational& lhs, const BigRational& rhs);
    friend bool operator!=(const BigRational& lhs, const BigRational& rhs);
    friend bool operator<(const BigRational& lhs, const BigRational& rhs);
    friend bool operator>(const BigRational& lhs, const BigRational& rhs);
    friend bool operator<=(const BigRational& lhs, const BigRational& rhs);
    friend bool operator>=(const BigRational& lhs, const BigRational& rhs);

    // assistants
    void reduce();
    static BigInteger gcd(const BigInteger& x, const BigInteger& y);

};

/* Constructors */

inline BigRational::BigRational() : numerator(0), denominator(1) {}

inline BigRational::BigRational(int64_t a, int64_t b): numerator(a), denominator(b)
{
    if (b == 0)
    {
        throw std::invalid_argument("zero division");
    }
    reduce();
}

inline BigRational::BigRational(const std::string& a, const std::string& b): numerator(a), denominator(b)
{
    if (denominator == BigInteger(0))
    {
        throw std::invalid_argument("zero division");
    }
    reduce();
}

/* Copy */

inline BigRational::BigRational(const BigRational& other): numerator(other.numerator), denominator(other.denominator){}

inline BigRational& BigRational::operator=(const BigRational& rhs) {
    if (this != &rhs)
    {
        numerator = rhs.numerator;
        denominator = rhs.denominator;
    }
    return *this;
}

/* Unary operators */

inline const BigRational& BigRational::operator+() const
{
    return *this;
}

inline BigRational BigRational::operator-() const
{
    BigRational temp(*this);
    temp.numerator = -temp.numerator;
    return temp;
}

/* Binary arithmetics operators */

inline BigRational& BigRational::operator+=(const BigRational& rhs)
{
    // (a/b) + (c/d) = (a*d + b*c) / (b*d)
    numerator = numerator * rhs.denominator + rhs.numerator * denominator;
    denominator = denominator * rhs.denominator;
    reduce();
    return *this;
}

inline BigRational operator+(BigRational lhs, const BigRational& rhs)
{
    lhs += rhs;
    return lhs;
}

inline BigRational& BigRational::operator-=(const BigRational& rhs)
{
    // (a/b) - (c/d) = (a*d - b*c) / (b*d)
    numerator = numerator * rhs.denominator - rhs.numerator * denominator;
    denominator = denominator * rhs.denominator;
    reduce();
    return *this;
}

inline BigRational operator-(BigRational lhs, const BigRational& rhs)
{
    lhs -= rhs;
    return lhs;
}

inline BigRational& BigRational::operator*=(const BigRational& rhs)
{
    // (a/b) * (c/d) = (a*c) / (b*d)
    numerator = numerator * rhs.numerator;
    denominator = denominator * rhs.denominator;
    reduce();
    return *this;
}

inline BigRational operator*(BigRational lhs, const BigRational& rhs)
{
    lhs *= rhs;
    return lhs;
}

inline BigRational& BigRational::operator/=(const BigRational& rhs)
{
    // (a/b) / (c/d) = (a*d) / (b*c)
    if (rhs.numerator == BigInteger(0))
    {
        throw std::logic_error("zero division");
    }
    numerator = numerator * rhs.denominator;
    denominator = denominator * rhs.numerator;
    reduce();
    return *this;
}

inline BigRational operator/(BigRational lhs, const BigRational& rhs)
{
    lhs /= rhs;
    return lhs;
}

/* Logical operators */

inline std::strong_ordering operator<=>(const BigRational& lhs, const BigRational& rhs)
{
    // lhs < rhs  <->  lhs.numerator * rhs.denominator < rhs.numerator * lhs.denominator

    BigInteger left_cross = lhs.numerator * rhs.denominator;
    BigInteger right_cross = rhs.numerator * lhs.denominator;

    if (left_cross == right_cross)
    {
        return std::strong_ordering::equal;
    }
    else if (left_cross < right_cross)
    {
        return std::strong_ordering::less;
    }
    else
    {
        return std::strong_ordering::greater;
    }
}

inline bool operator==(const BigRational& lhs, const BigRational& rhs)
{
    return (lhs <=> rhs) == std::strong_ordering::equal;
}

inline bool operator!=(const BigRational& lhs, const BigRational& rhs)
{
    return (lhs <=> rhs) != std::strong_ordering::equal;
}

inline bool operator<(const BigRational& lhs, const BigRational& rhs)
{
    return (lhs <=> rhs) == std::strong_ordering::less;
}

inline bool operator>(const BigRational& lhs, const BigRational& rhs)
{
    return (lhs <=> rhs) == std::strong_ordering::greater;
}

inline bool operator<=(const BigRational& lhs, const BigRational& rhs)
{
    auto cmp = (lhs <=> rhs);
    return cmp == std::strong_ordering::equal || cmp == std::strong_ordering::less;
}

inline bool operator>=(const BigRational& lhs, const BigRational& rhs)
{
    auto cmp = (lhs <=> rhs);
    return cmp == std::strong_ordering::equal || cmp == std::strong_ordering::greater;
}

/* Friends */

inline std::ostream& operator<<(std::ostream& lhs, const BigRational& rhs)
{

    BigInteger abs_number = rhs.numerator;
    bool negative = false;

    if (abs_number < BigInteger(0))
    {
        negative = true;
        abs_number = -abs_number;
    }

    if (negative)
    {
        lhs << "-";
    }

    lhs << abs_number;

    if (rhs.denominator != BigInteger(1))
    {
        lhs << "/" << rhs.denominator;
    }

    return lhs;
}

#if SUPPORT_IFSTREAM == 1

inline std::istream& operator>>(std::istream& lhs, BigRational& rhs)
{
    BigInteger num;     // numerator

    if (!readBigInteger(lhs, num))
    {
        lhs.setstate(std::ios::failbit);
        return lhs;
    }

    std::istream::sentry s(lhs);    // try read '/'
    if (!s)
    {
        rhs.numerator = num;
        rhs.denominator = BigInteger(1);
        return lhs;
    }

    int c = lhs.peek();
    if (c == '/')
    {
        lhs.ignore(1);

        BigInteger den;     // denominator
        if (!readBigInteger(lhs, den))
        {
            lhs.setstate(std::ios::failbit);
            return lhs;
        }

        if (den == BigInteger(0))
        {
            lhs.setstate(std::ios::failbit);
            return lhs;
        }

        rhs.numerator = num;
        rhs.denominator = den;
    }
    else
    {
        rhs.numerator = num;
        rhs.denominator = BigInteger(1);
    }

    rhs.reduce();

    return lhs;
}

#endif

/* More operators */

inline double BigRational::sqrt() const
{
    if (numerator < BigInteger(0))
    {
        throw std::runtime_error("negative number");
    }

    double double_numerator = 0.0;
    double double_denominator = 0.0;

    try {
        double_numerator = std::stod(this->numerator.value);
        double_denominator = std::stod(this->denominator.value);
    } catch(const std::out_of_range&) {
        throw std::runtime_error("large number");
    }

    if (std::isinf(double_numerator) || std::isinf(double_denominator) || double_denominator == 0.0)
    {
        throw std::runtime_error("large number");
    }

    double number = double_numerator / double_denominator;
    if (number < 0.0)
    {
        throw std::runtime_error("negative number");
    }

    double result = std::sqrt(number);
    if (std::isinf(result))
    {
        throw std::runtime_error("large number");
    }

    return result;
}

#if SUPPORT_MORE_OPS == 1

inline BigInteger BigRational::isqrt() const
{
    if (numerator < BigInteger(0))
    {
        throw std::runtime_error("negative number");
    }

    if (numerator == BigInteger(0))
    {
        return BigInteger(0);
    }

    // searching for x^2 <= numerator/denominator <= (x + 1)^2
    // -> x^2 * denominator <= numerator <= (x + 1)^2 * denominator
    // -> binary search

    BigInteger low(0);
    BigInteger high = numerator;
    BigInteger one(1);

    BigInteger result(0);

    while (low <= high)
    {
        BigInteger mid = (low + high) / BigInteger(2);

        BigInteger left = mid * mid * denominator;

        if (left == numerator)
        {
            return mid;
        }
        else if (left < numerator)
        {
            result = mid;
            low = mid + one;
        }
        else
        {
            high = mid - one;
        }
    }

    return result;
}

#endif

/* Assistants */

inline BigInteger BigRational::gcd(const BigInteger& x, const BigInteger& y) // euclidean
{
    BigInteger a = x;
    BigInteger b = y;

    if (a < BigInteger(0))
    {
        a = -a;
    }

    if (b < BigInteger(0))
    {
        b = -b;
    }

    while (b != BigInteger(0))
    {
        BigInteger remained = a % b;
        a = b;
        b = remained;
    }
    return a;
}

inline void BigRational::reduce()
{
    if (numerator == BigInteger(0))
    {
        denominator = BigInteger(1);
        return;
    }

    BigInteger divisor = gcd(numerator, denominator);

    numerator /= divisor;
    denominator /= divisor;

    if (denominator < BigInteger(0))
    {
        numerator = -numerator;
        denominator = -denominator;
    }
}