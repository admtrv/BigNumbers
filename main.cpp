#include <iostream>

#include "bignumbers.h"

int main()
{
    BigInteger num1(-12345);
    BigInteger num2("67890");
    std::cout << (num1 == num2) << std::endl;
    std::cout << (num1 != num2) << std::endl;
    std::cout << (num1 > num2) << std::endl;
    std::cout << (num1 < num2) << std::endl;
    std::cout << (num1 >= num2) << std::endl;
    std::cout << (num1 <= num2) << std::endl;

    return 0;
}