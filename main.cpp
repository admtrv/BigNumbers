#include <iostream>

#include "bignumbers.h"

int main()
{
    BigInteger num1(-12345);
    std::cout << num1 << std::endl;

    BigInteger num2("67890");
    std::cout << num2 << std::endl;

    BigInteger num3("123456789012345678901234567890");
    std::cout << num3 << std::endl;

    BigInteger num4("+0000078901234567890");
    std::cout << num4 << std::endl;

    BigInteger num5("-987654321098765432109876543210");
    std::cout << num5 << std::endl;

    BigInteger num6("0000123400");
    std::cout << num6 << std::endl;

    BigInteger num7("0");
    std::cout << num7 << std::endl;

    BigInteger num8("-0000000000");
    std::cout << num8 << std::endl;

    return 0;
}