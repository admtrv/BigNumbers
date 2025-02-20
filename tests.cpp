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

        testBigIntegerConstructors();
        std::cout << std::endl;

        testBigIntegerUnaryOperators();
        std::cout << std::endl;

        testBigIntegerLogicalOperators();
        std::cout << std::endl;

#if SUPPORT_IFSTREAM == 1
        testBigIntegerInputOutput();
        std::cout << std::endl;
#endif

        testBigIntegerArithmeticOperators();
        std::cout << std::endl;

#if SUPPORT_MORE_OPS == 1
        testBigIntegerMoreOperators();
        std::cout << std::endl;
#endif

#if SUPPORT_EVAL == 1
        testBigIntegerEvaluation();
        std::cout << std::endl;
#endif

        testBigRationalConstructors();
        std::cout << std::endl;

        testBigRationalUnaryOperators();
        std::cout << std::endl;

        testBigRationalLogicalOperators();
        std::cout << std::endl;

#if SUPPORT_IFSTREAM == 1
        testBigRationalInputOutput();
        std::cout << std::endl;
#endif

        testBigRationalArithmeticOperators();
        std::cout << std::endl;

#if SUPPORT_MORE_OPS == 1
        testBigRationalMoreOperators();
        std::cout << std::endl;
#endif

        std::cout << "All tests passed successfully!" << std::endl;
    }

private:
    static void testBigIntegerConstructors()
    {
        std::cout << "Running BigInteger constructor tests..." << std::endl;

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

        std::cout << "BigInteger constructor tests passed!" << std::endl;
    }

    static void testBigIntegerUnaryOperators()
    {
        std::cout << "Running BigInteger unary operator tests..." << std::endl;

        BigInteger num1(-12345);
        BigInteger num2("67890");

        // Unary operator +
        assert(+num1 == num1);
        assert(+num2 == num2);

        // Unary operator -
        assert(-num1 == BigInteger("12345"));
        assert(-num2 == BigInteger("-67890"));

        std::cout << "BigInteger unary operator tests passed!" << std::endl;
    }

    static void testBigIntegerLogicalOperators()
    {
        std::cout << "Running BigInteger logical operator tests..." << std::endl;

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

        std::cout << "BigInteger logical operator tests passed!" << std::endl;
    }
#if SUPPORT_IFSTREAM == 1
    static void testBigIntegerInputOutput()
    {
        std::cout << "Running BigInteger input/output tests..." << std::endl;

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

        {
            std::istringstream input_stream("");
            BigInteger num;
            input_stream >> num;
            assert(!input_stream.good());
        }

        {
            std::istringstream input_stream("abc123");
            BigInteger num;
            input_stream >> num;
            assert(!input_stream.good());
        }

        {
            std::istringstream input_stream("-");
            BigInteger num;
            input_stream >> num;
            assert(!input_stream.good());
        }

        std::cout << "BigInteger input/output tests passed!" << std::endl;
    }
#endif

    static void testBigIntegerArithmeticOperators()
    {
        std::cout << "Running BigInteger arithmetic operator tests..." << std::endl;

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

        std::cout << "BigInteger arithmetic operator tests passed!" << std::endl;
    }

#if SUPPORT_MORE_OPS == 1

    static void testBigIntegerMoreOperators()
    {
        std::cout << "Running BigInteger more operators tests..." << std::endl;

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
                assert(false); // should not reach here
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
                assert(false); // should not reach here
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

            BigInteger num13("-7");
            assert(num13.is_prime(5) == false);

            BigInteger zero("0");
            assert(zero.is_prime(5) == false);

            BigInteger one("1");
            assert(one.is_prime(5) == false);

            BigInteger carmichael("561");
            assert(carmichael.is_prime(5) == false);
        }

        std::cout << "BigInteger more operators tests passed!" << std::endl;
    }

#endif

#if SUPPORT_EVAL == 1
    static void testBigIntegerEvaluation()
    {
        std::cout << "Running BigInteger evaluation tests..." << std::endl;

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
                "right": 456.001
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
                "op":" + ",
                "left":  123  ,
                "right": {
                    "op":"    *",
                    "left": "       12345678901234567890",
                    "right": {
                        "op":"%    ",
                        "left":"34            ",
                        "right":   1
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

        std::cout << "BigInteger evaluation tests passed!" << std::endl;
    }
#endif

    static void testBigRationalConstructors()
    {
        std::cout << "Running BigRational constructor tests..." << std::endl;

        {
            BigRational num1;
            assert(num1 == BigRational(0, 1));
        }

        {
            BigRational num2(3, 4);
            assert(num2 == BigRational("3", "4"));
        }

        {
            BigRational num3(-10, -2);
            assert(num3 == BigRational(5, 1)); // -10 / -2 -> 10/2 -> 5/1
        }

        {
            BigRational num4(-10, 2);
            assert(num4 == BigRational("-5", "1")); // -10/2 -> -5/1
        }

        {
            BigRational num5("12", "-8");
            assert(num5 == BigRational("-3", "2")); // 12 / -8 -> -3/2
        }

        {
            BigRational num6("0", "3456");
            assert(num6 == BigRational(0, 1));
        }

        try {
            BigRational num7(1, 0);
            assert(false);
        } catch(const std::invalid_argument& e) {
            // zero division
        }

        std::cout << "BigRational constructor tests passed!" << std::endl;
    }

    static void testBigRationalUnaryOperators()
    {
        std::cout << "Running BigRational unary operator tests..." << std::endl;

        {
            BigRational num1(-3, 4); // -3/4
            assert((+num1) == BigRational(-3, 4));

            BigRational num2 = -num1; // 3/4
            assert(num2 == BigRational(3, 4));
        }

        {
            BigRational num3("0", "5");
            assert((+num3) == num3);
            assert((-num3) == BigRational(0, 1));  // -0/1 == 0/1
        }

        std::cout << "BigRational unary operator tests passed!" << std::endl;
    }

    static void testBigRationalLogicalOperators()
    {
        std::cout << "Running BigRational logical operator tests..." << std::endl;

        BigRational num1(1, 2);     // 1/2
        BigRational num2(2, 4);     // 2/4 -> 1/2
        BigRational num3(-3, 4);    // -3/4
        BigRational num4(3, -4);    // 3/-4 -> -3/4
        BigRational num5(2, 3);     // 2/3
        BigRational zero(0,1);      // 0/1

        assert(num1 == num2);
        assert(num3 == num4);
        assert(num3 != num5);

        assert(num1 < num5);
        assert(num3 < num1);
        assert(num5 > num2);
        assert(zero < num1);

        assert(num1 <= num2);
        assert(num1 <= num5);
        assert(num5 >= num1);

        std::cout << "BigRational logical operator tests passed!" << std::endl;
    }
#if SUPPORT_IFSTREAM == 1
    static void testBigRationalInputOutput()
    {
        std::cout << "Running BigRational input/output tests..." << std::endl;

        {
            std::istringstream input_stream("3/4");
            BigRational num1;
            input_stream >> num1;
            assert(num1 == BigRational(3, 4));
        }

        {
            std::istringstream input_stream("-3/-4");
            BigRational num2;
            input_stream >> num2;
            assert(num2 == BigRational(3, 4));
        }

        {
            std::istringstream input_stream("12");
            BigRational num3;
            input_stream >> num3;
            assert(num3 == BigRational(12, 1));
        }

        {
            BigRational num4(-5, 2);
            std::ostringstream output_stream;
            output_stream << num4;
            assert(output_stream.str() == "-5/2");
        }

        {
            std::istringstream input_stream("abc/2");
            BigRational num5;
            input_stream >> num5;
            assert(!input_stream.good());
        }

        {
            std::istringstream input_stream("123/");
            BigRational num6;
            input_stream >> num6;
            assert(!input_stream.good());
        }

        std::cout << "BigRational input/output tests passed!" << std::endl;
    }
#endif

    static void testBigRationalArithmeticOperators()
    {
        std::cout << "Running BigRational arithmetic operator tests..." << std::endl;

        {
            // (1/2) + (1/3) = (3/6) + (2/6) = (5/6)
            BigRational num1(1, 2);
            BigRational num2(1, 3);
            BigRational result = num1 + num2;
            assert(result == BigRational(5, 6));
        }

        {
            // (-3/4) + (1/2) = (-3/4) + (2/4) = (-1/4)
            BigRational num1(-3, 4);
            BigRational num2(1, 2);
            BigRational result = num1 + num2;
            assert(result == BigRational(-1, 4));
        }

        {
            // (1/2) - (1/3) = (1/6)
            BigRational num1(1, 2);
            BigRational num2(1, 3);
            BigRational result = num1 - num2;
            assert(result == BigRational(1, 6));
        }

        {
            // (-3/4) - (1/2) = (-3/4) - (2/4) = (-5/4)
            BigRational num1(-3, 4);
            BigRational num2(1, 2);
            BigRational result = num1 - num2;
            assert(result == BigRational(-5, 4));
        }

        {
            // (2/3) * (3/4) = (6/12) -> (1/2)
            BigRational num1(2, 3);
            BigRational num2(3, 4);
            BigRational result = num1 * num2;
            assert(result == BigRational(1, 2));
        }

        {
            // (-1/2) * (3/7) = (-3/14)
            BigRational num1(-1, 2);
            BigRational num2(3, 7);
            BigRational result = num1 * num2;
            assert(result == BigRational(-3, 14));
        }

        {
            // (3/4) / (1/2) = (3/4) * (2/1) = (6/4) -> (3/2)
            BigRational num1(3, 4);
            BigRational num2(1, 2);
            BigRational result = num1 / num2;
            assert(result == BigRational(3, 2));
        }

        {
            // (-1/2) / (2/3) = (-1/2) * (3/2) = (-3/4)
            BigRational num1(-1, 2);
            BigRational num2(2, 3);
            BigRational result = num1 / num2;
            assert(result == BigRational(-3, 4));
        }

        {
            BigRational num1(1, 2);
            BigRational num2(0, 5);
            try {
                BigRational result = num1 / num2;
                assert(false);
            } catch(const std::logic_error& e) {
                // zero division
            }
        }

        std::cout << "BigRational arithmetic operator tests passed!" << std::endl;
    }

#if SUPPORT_MORE_OPS == 1
    static void testBigRationalMoreOperators()
    {
        std::cout << "Running BigRational more operators tests..." << std::endl;

        // sqrt()
        {
            BigRational num(4, 9);
            double result = num.sqrt(); // close to 0.666666...
            assert(std::abs(result - 0.666666667) < 0.000000001);
        }

        {
            BigRational num(-1, 2);
            try {
                double result = num.sqrt();
                assert(false);
            } catch(const std::exception& e) {
                // negative number
            }
        }

        // isqrt()
        {
            BigRational num(4, 9);
            BigInteger result = num.isqrt();
            assert(result == BigInteger(0));
        }

        {
            // 9/4 == 2.25 -> isqrt = floor(1.5) = 1
            BigRational num(9, 4);
            BigInteger result = num.isqrt();
            assert(result == BigInteger(1));
        }

        {
            BigRational num(-16, 1);
            try {
                BigInteger result = num.isqrt();
                assert(false);
            } catch(const std::exception& e) {
                // negative number
            }
        }

        std::cout << "BigRational more operators tests passed!" << std::endl;
    }
#endif
};
