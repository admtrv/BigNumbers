/*
 * tests.cpp
 */

#include <iostream>
#include <cassert>
#include <sstream>

#include "bignumbers.h"

class Tests {
public:
    void runTests()
    {
        std::cout << "Running all tests..." << std::endl;
        std::cout << std::endl;

        testConstructors();
        std::cout << std::endl;

        testUnaryOperators();
        std::cout << std::endl;

        testLogicalOperators();
        std::cout << std::endl;

        testInputOutput();
        std::cout << std::endl;

        testEdgeCases();
        std::cout << std::endl;
        
        std::cout << "All tests passed successfully!" << std::endl;
    }

private:
    static void testConstructors()
    {
        std::cout << "Running constructor tests..." << std::endl;

        BigInteger num1;
        BigInteger num2(12345);
        BigInteger num3("-67890");
        BigInteger num4(num2);

        assert(num1 == BigInteger("0"));
        assert(num2 == BigInteger("12345"));
        assert(num3 == BigInteger("-67890"));
        assert(num4 == num2);

        std::cout << "Constructor tests passed!" << std::endl;
    }

    static void testUnaryOperators()
    {
        std::cout << "Running unary operator tests..." << std::endl;

        BigInteger num1(-12345);
        BigInteger num2("67890");

        // Unary operator +
        assert(+num1 == num1);
        assert(+num2 == num2);

        // Unary operator -
        assert(-num1 == BigInteger("12345"));
        assert(-num2 == BigInteger("-67890"));

        std::cout << "Unary operator tests passed!" << std::endl;
    }

    static void testLogicalOperators()
    {
        std::cout << "Running logical operator tests..." << std::endl;

        BigInteger num1(12345);
        BigInteger num2("67890");
        BigInteger num3("-12345");
        BigInteger num4(67890);
        BigInteger zero("0");

        assert(num1 != num3);
        assert(num2 == num4);

        assert(num3 < num1);
        assert(num2 > num1);

        assert(num1 <= num2);
        assert(num2 >= num4);
        assert(zero <= num1);
        assert(zero >= num3);

        std::cout << "Logical operator tests passed!" << std::endl;
    }

    void testInputOutput()
    {
        std::cout << "Running input/output tests..." << std::endl;

        std::istringstream input_stream("-123456");
        BigInteger num1;
        input_stream >> num1;
        assert(num1 == BigInteger("-123456"));
        std::cout << "Expected input: -123456, Actual input: " << num1 << std::endl;

        BigInteger num2("78901");
        std::ostringstream output_stream;
        output_stream << num2;
        assert(output_stream.str() == "78901");

        std::cout << "Expected output: 78901, Actual output: " << output_stream.str() << std::endl;

        std::cout << "Input/output tests passed!" << std::endl;
    }

    void testEdgeCases()
    {
        std::cout << "Running edge case tests..." << std::endl;

        BigInteger zero("0");
        BigInteger negZero("-0");

        assert(zero == negZero);
        assert(!(zero < negZero));
        assert(!(zero > negZero));
        assert(zero >= negZero);
        assert(zero <= negZero);

        BigInteger large1("123456789012345678901234567890");
        BigInteger large2("123456789012345678901234567890");
        BigInteger large3("-123456789012345678901234567890");

        assert(large1 == large2);
        assert(large1 != large3);
        assert(large3 < large1);
        assert(large1 > large3);
        assert(large1 >= large2);
        assert(large3 <= large1);

        std::cout << "Edge case tests passed!" << std::endl;
    }
};

