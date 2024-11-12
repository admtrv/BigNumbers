#include <iostream>

#include "bignumbers.h"

int main()
{
    BigInteger num1(-12345);
    std::cout << +num1 << std::endl;
    std::cout << -num1 << std::endl;

    BigInteger num2("67890");
    std::cout << +num2 << std::endl;
    std::cout << -num2 << std::endl;

    return 0;
}