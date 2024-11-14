/*
 * tests.cpp
 */

#include <iostream>
#include <cassert>
#include <sstream>

#include "bignumbers.h"

class Tests {
public:
    static void run()
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

        testArithmeticOperators();
        std::cout << std::endl;

        testMoreOperators();
        std::cout << std::endl;

        testEvaluation();
        std::cout << std::endl;
        
        std::cout << "All tests passed successfully!" << std::endl;
    }

private:
    static void testConstructors()
    {
        std::cout << "Running constructor tests..." << std::endl;

        BigInteger num1;
        BigInteger num2(12345);
        BigInteger num3("-0000000000067890");
        BigInteger num4(num2);
        BigInteger num5("000012345");
        BigInteger num6(-67890);

        assert(num1 == BigInteger("0"));
        assert(num2 == BigInteger("12345"));
        assert(num3 == BigInteger("-67890"));
        assert(num4 == num2);
        assert(num5 == num2);
        assert(num6 == num3);

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
        BigInteger negZero("-0");

        BigInteger large1("123456789012345678901234567890");
        BigInteger large2("123456789012345678901234567890");
        BigInteger large3("-123456789012345678901234567890");

        assert(num1 != num3);
        assert(num2 == num4);

        assert(num3 < num1);
        assert(num2 > num1);

        assert(num1 <= num2);
        assert(num2 >= num4);
        assert(zero <= num1);
        assert(zero >= num3);

        assert(zero == negZero);
        assert(!(zero < negZero));
        assert(!(zero > negZero));
        assert(zero >= negZero);
        assert(zero <= negZero);

        assert(large1 == large2);
        assert(large1 != large3);
        assert(large3 < large1);
        assert(large1 > large3);
        assert(large1 >= large2);
        assert(large3 <= large1);

        std::cout << "Logical operator tests passed!" << std::endl;
    }

    static void testInputOutput()
    {
        std::cout << "Running input/output tests..." << std::endl;

        {
            std::istringstream input_stream("-123456");
            BigInteger num1;
            input_stream >> num1;
            assert(num1 == BigInteger("-123456"));
        }

        {
            std::istringstream input_stream("0008723491");
            BigInteger num1;
            input_stream >> num1;
            assert(num1 == BigInteger(8723491));
        }

        {
            BigInteger num2("78901");
            std::ostringstream output_stream;
            output_stream << num2;
            assert(output_stream.str() == "78901");
        }

        {
            BigInteger num2("000009000238901");
            std::ostringstream output_stream;
            output_stream << num2;
            assert(output_stream.str() == "9000238901");
        }

        std::cout << "Input/output tests passed!" << std::endl;
    }

    static void testArithmeticOperators()
    {
        std::cout << "Running arithmetic operator tests..." << std::endl;

        // Addition
        {
            BigInteger num1("663824");
            BigInteger num2("920457239");
            BigInteger result = num1 + num2;
            assert(result == BigInteger("921121063"));
        }

        {
            BigInteger num1("123125790123");
            BigInteger num2("675");
            BigInteger result = num1 + num2;
            assert(result == BigInteger("123125790798"));
        }

        {
            BigInteger num1("123456789012345678901234567890");
            BigInteger num2("987654321098765432109876543210");
            BigInteger result = num1 + num2;
            assert(result == BigInteger("1111111110111111111011111111100"));
        }

        {
            BigInteger num1("-123456789012345678901234567890");
            BigInteger num2("123456789012345678901234567890");
            BigInteger result = num1 + num2;
            assert(result == BigInteger("0"));
        }

        {
            BigInteger num("123456789012345678901234567890");
            BigInteger zero("0");
            BigInteger result = num + zero;
            assert(result == num);
        }

        {
            BigInteger zero("0");
            BigInteger num("987654321098765432109876543210");
            BigInteger result = zero + num;
            assert(result == num);
        }

        {
            BigInteger large("999999999999999999999999999999");
            BigInteger one("1");
            BigInteger result = large + one;
            assert(result == BigInteger("1000000000000000000000000000000"));
        }

        // Subtraction
        {
            BigInteger num1("921121063");
            BigInteger num2("663824");
            BigInteger result = num1 - num2;
            assert(result == BigInteger("920457239"));
        }

        {
            BigInteger num1("123125790123");
            BigInteger num2("675");
            BigInteger result = num1 - num2;
            assert(result == BigInteger("123125789448"));
        }

        {
            BigInteger num1("987654321098765432109876543210");
            BigInteger num2("123456789012345678901234567890");
            BigInteger result = num1 - num2;
            assert(result == BigInteger("864197532086419753208641975320"));
        }

        {
            BigInteger num1("123456789012345678901234567890");
            BigInteger num2("-123456789012345678901234567890");
            BigInteger result = num1 - num2;
            assert(result == BigInteger("246913578024691357802469135780"));
        }

        {
            BigInteger num("123456789012345678901234567890");
            BigInteger zero("0");
            BigInteger result = num - zero;
            assert(result == num);
        }

        {
            BigInteger zero("0");
            BigInteger num("123456789012345678901234567890");
            BigInteger result = zero - num;
            assert(result == BigInteger("-123456789012345678901234567890"));
        }

        {
            BigInteger large("1000000000000000000000000000000");
            BigInteger one("1");
            BigInteger result = large - one;
            assert(result == BigInteger("999999999999999999999999999999"));
        }

        // Multiplication
        {
            BigInteger num1("12345");
            BigInteger num2("6789");
            BigInteger result = num1 * num2;
            assert(result == BigInteger("83810205"));
        }

        {
            BigInteger num1("987654321");
            BigInteger num2("123456789");
            BigInteger result = num1 * num2;
            assert(result == BigInteger("121932631112635269"));
        }

        {
            BigInteger num1("-123456789");
            BigInteger num2("987654321");
            BigInteger result = num1 * num2;
            assert(result == BigInteger("-121932631112635269"));
        }

        {
            BigInteger num1("1000000000000000000");
            BigInteger num2("0");
            BigInteger result = num1 * num2;
            assert(result == BigInteger("0"));
        }

        // Division
        {
            BigInteger num1("121932631112635269");
            BigInteger num2("987654321");
            BigInteger result = num1 / num2;
            assert(result == BigInteger("123456789"));
        }

        {
            BigInteger num1("123456789");
            BigInteger num2("987654321");
            BigInteger result = num1 / num2;
            assert(result == BigInteger("0"));
        }

        {
            BigInteger num1("-121932631112635269");
            BigInteger num2("987654321");
            BigInteger result = num1 / num2;
            assert(result == BigInteger("-123456789"));
        }

        {
            BigInteger num1("987654321");
            BigInteger num2("1");
            BigInteger result = num1 / num2;
            assert(result == BigInteger("987654321"));
        }

        // Modulus
        {
            BigInteger num1("121932631112635269");
            BigInteger num2("987654321");
            BigInteger result = num1 % num2;
            assert(result == BigInteger("0"));
        }

        {
            BigInteger num1("123456789");
            BigInteger num2("10000");
            BigInteger result = num1 % num2;
            assert(result == BigInteger("6789"));
        }

        {
            BigInteger num1("987654321");
            BigInteger num2("123456789");
            BigInteger result = num1 % num2;
            assert(result == BigInteger("9"));
        }

        {
            BigInteger num1("123456789");
            BigInteger num2("987654321");
            BigInteger result = num1 % num2;
            assert(result == BigInteger("123456789"));
        }

        std::cout << "Arithmetic operator tests passed!" << std::endl;
    }

    static void testMoreOperators()
    {
        std::cout << "Running more operators tests..." << std::endl;

        // sqrt()
        {
            BigInteger num("123456789");
            double result = num.sqrt();
            assert(std::abs(result - std::sqrt(123456789.0)) < 1e-6);
        }

        {
            BigInteger num("-123456789");
            try {
                double result = num.sqrt();
                assert(false); // Should not reach here
            } catch (const std::exception& e) {
                assert(std::string(e.what()) == "negative number");
            }
        }

        // isqrt()
        {
            BigInteger num("144");
            BigInteger result = num.isqrt();
            assert(result == BigInteger("12"));
        }

        {
            BigInteger num("150");
            BigInteger result = num.isqrt();
            assert(result == BigInteger("12"));
        }

        {
            BigInteger num("0");
            BigInteger result = num.isqrt();
            assert(result == BigInteger("0"));
        }

        {
            BigInteger num("1");
            BigInteger result = num.isqrt();
            assert(result == BigInteger("1"));
        }

        {
            BigInteger num("-16");
            try {
                BigInteger result = num.isqrt();
                assert(false); // Should not reach here
            } catch (const std::exception& e) {
                assert(std::string(e.what()) == "negative number");
            }
        }

        // is_prime()
        {
            BigInteger num2("2");
            assert(num2.is_prime(5) == true);

            BigInteger num3("3");
            assert(num3.is_prime(5) == true);

            BigInteger num5("5");
            assert(num5.is_prime(5) == true);

            BigInteger num7("7");
            assert(num7.is_prime(5) == true);

            BigInteger num4("4");
            assert(num4.is_prime(5) == false);

            BigInteger num6("6");
            assert(num6.is_prime(5) == false);

            BigInteger num11("104729");
            assert(num11.is_prime(5) == true);

            BigInteger num12("104730");
            assert(num12.is_prime(5) == false);

            BigInteger numNeg("-7");
            assert(numNeg.is_prime(5) == false);

            BigInteger numZero("0");
            assert(numZero.is_prime(5) == false);

            BigInteger numOne("1");
            assert(numOne.is_prime(5) == false);

            BigInteger carmichael("561");
            assert(carmichael.is_prime(5) == false);
        }

        std::cout << "More operators tests passed!" << std::endl;
    }

    static void testEvaluation()
    {
        std::cout << "Running evaluation tests..." << std::endl;

        {
            std::string json = R"(
            {
                "op":"+",
                "left": 123,
                "right": 456
            }
            )";
            BigInteger result = eval(json);
            assert(result == BigInteger(123 + 456));
        }

        {
            std::string json = R"(
            {
                "op":"+",
                "left": 123,
                "right": {
                    "op":"*",
                    "left": "12345678901234567890",
                    "right": {
                        "op":"%",
                        "left":"34",
                        "right":1
                    }
                }
            }
            )";
            BigInteger result = eval(json);
            assert(result == BigInteger(123 + (12345678901234567890 * (34 % 1))));
        }

        {
            std::string json = R"(
            {
                "op":"+",
                "left": {
                    "op":"-",
                    "left": "1000",
                    "right": "200"
                },
                "right": {
                    "op":"/",
                    "left": "123456",
                    "right": "123"
                }
            }
            )";
            BigInteger result = eval(json);
            assert(result == BigInteger(1000 - 200 + (123456 / 123)));
        }

        std::cout << "Evaluation tests passed!" << std::endl;
    }
};

