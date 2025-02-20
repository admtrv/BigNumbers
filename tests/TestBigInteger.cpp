/*
 * TestBigInteger.cpp
 */

#include <gtest/gtest.h>

#include "../bignumbers.h"

class TestBigInteger : public ::testing::Test
{
protected:
    BigInteger _pos_zero = BigInteger(0);
    BigInteger _neg_zero = BigInteger(0);

    BigInteger _pos_one = BigInteger(1);
    BigInteger _neg_one = BigInteger(-1);

    BigInteger _pos_num1 = BigInteger(12345);
    BigInteger _pos_num2 = BigInteger(67890);

    BigInteger _neg_num1 = BigInteger(-12345);
    BigInteger _neg_num2 = BigInteger(-67890);

    BigInteger _pos_large1 = BigInteger("123456789012345678901234567890");
    BigInteger _pos_large2 = BigInteger("987654321098765432109876543210");

    BigInteger _neg_large1 = BigInteger("-123456789012345678901234567890");
    BigInteger _neg_large2 = BigInteger("-987654321098765432109876543210");
};

TEST_F(TestBigInteger, TestConstructor)
{
    BigInteger default_num;
    ASSERT_EQ(default_num, _pos_zero);

    BigInteger explicit_num(12345);
    ASSERT_EQ(explicit_num, _pos_num1);

    BigInteger string_num("-0000000000067890");
    ASSERT_EQ(string_num, _neg_num2);
    
    BigInteger copy_num(explicit_num);
    ASSERT_EQ(copy_num, _pos_num1);
    
    BigInteger leading_zero_num("000012345");
    ASSERT_EQ(leading_zero_num, _pos_num1);
    
    BigInteger neg_from_int(-67890);
    ASSERT_EQ(neg_from_int, _neg_num2);
}

TEST_F(TestBigInteger, TestUnary)
{
    // Unary operator +
    ASSERT_EQ(+_neg_num1, _neg_num1);
    ASSERT_EQ(+_pos_num2, _pos_num2);

    // Unary operator -
    ASSERT_EQ(-_neg_num1, _pos_num1);
    ASSERT_EQ(-_pos_num2, _neg_num2);
}

TEST_F(TestBigInteger, TestLogicalOperators)
{
    // Copies
    BigInteger pos_num2(_pos_num2);
    BigInteger pos_large1(_pos_large1);

    // Operations
    ASSERT_NE(_pos_num1, _neg_num1);
    ASSERT_EQ(_pos_num2, pos_num2);

    ASSERT_LT(_neg_num1, _pos_num1);
    ASSERT_GT(_pos_num2, _pos_num1);

    ASSERT_LE(_pos_num1, _pos_num2);
    ASSERT_GE(_pos_num2, pos_num2);

    ASSERT_LE(_pos_zero, _pos_num1);
    ASSERT_GE(_pos_zero, _neg_num1);

    ASSERT_EQ(_pos_zero, _neg_zero);
    ASSERT_FALSE(_pos_zero < _neg_zero);
    ASSERT_FALSE(_pos_zero > _neg_zero);
    ASSERT_GE(_pos_zero, _neg_zero);
    ASSERT_LE(_pos_zero, _neg_zero);

    ASSERT_EQ(_pos_large1, pos_large1);
    ASSERT_NE(_pos_large1, _neg_large1);

    ASSERT_LT(_neg_large1, _pos_large1);
    ASSERT_GT(_pos_large1, _neg_large1);

    ASSERT_GE(_pos_large1, pos_large1);
    ASSERT_LE(_neg_large1, _pos_large1);
}

#if SUPPORT_IFSTREAM == 1

TEST_F(TestBigInteger, TestInputOutput)
{
    {
        std::istringstream input_stream("-123456");
        BigInteger num;
        input_stream >> num;
        ASSERT_EQ(num, BigInteger(-123456));
    }

    {
        std::istringstream input_stream("0008723491");
        BigInteger num;
        input_stream >> num;
        ASSERT_EQ(num, BigInteger(8723491));
    }

    {
        BigInteger num("78901");
        std::ostringstream output_stream;
        output_stream << num;
        ASSERT_EQ(output_stream.str(), "78901");
    }

    {
        BigInteger num("000009000238901");
        std::ostringstream output_stream;
        output_stream << num;
        ASSERT_EQ(output_stream.str(), "9000238901");
    }

    {
        std::istringstream input_stream("");
        BigInteger num;
        input_stream >> num;
        ASSERT_FALSE(input_stream.good());
    }

    {
        std::istringstream input_stream("abc123");
        BigInteger num;
        input_stream >> num;
        ASSERT_FALSE(input_stream.good());
    }

    {
        std::istringstream input_stream("-");
        BigInteger num;
        input_stream >> num;
        ASSERT_FALSE(input_stream.good());
    }
}

#endif

TEST_F(TestBigInteger, TestArithmeticOperators)
{
    // Addition
    {
        BigInteger sum = _pos_num1 + _neg_num1;
        ASSERT_EQ(sum, BigInteger(0));
    }
    {
        BigInteger sum = _pos_num2 + _pos_zero;
        ASSERT_EQ(sum, _pos_num2);
    }
    {
        BigInteger sum = _pos_num1 + _pos_num2;
        ASSERT_EQ(sum, BigInteger("80235"));
    }
    {
        BigInteger sum = _pos_large1 + _pos_large2;
        ASSERT_EQ(sum, BigInteger("1111111110111111111011111111100"));
    }

    // Subtraction
    {
        BigInteger diff = _pos_num2 - _pos_num1;
        ASSERT_EQ(diff, BigInteger("55545"));
    }
    {
        BigInteger diff = _pos_num1 - _neg_num1;
        ASSERT_EQ(diff, BigInteger("24690"));
    }
    {
        BigInteger diff = _pos_large2 - _pos_large1;
        ASSERT_EQ(diff, BigInteger("864197532086419753208641975320"));
    }

    // Multiplication
    {
        BigInteger prod = _pos_num1 * _neg_num1;
        ASSERT_EQ(prod, BigInteger("-152399025"));
    }
    {
        BigInteger prod = _pos_num1 * _pos_zero;
        ASSERT_EQ(prod, _pos_zero);
    }

    // Division
    {
        BigInteger div = _pos_num2/ _neg_num1;
        ASSERT_EQ(div, BigInteger("-5"));
    }
    {
        BigInteger div = _neg_num2/ _neg_num1;
        ASSERT_EQ(div, BigInteger("5"));
    }
    {
        BigInteger div = _pos_large2/ _pos_large1;
        ASSERT_EQ(div, BigInteger("8"));
    }

    // Mod
    {
        BigInteger rem = _pos_num2 % _pos_num1;
        ASSERT_EQ(rem, BigInteger("6165"));
    }
    {
        BigInteger rem = _pos_large2 % _pos_large1;
        ASSERT_EQ(rem, BigInteger("9000000000900000000090"));
    }
}

#if SUPPORT_MORE_OPS == 1

TEST_F(TestBigInteger, TestMoreOperators)
{
    // sqrt
    {
        double result = _pos_num1.sqrt();
        ASSERT_NEAR(result, std::sqrt(12345.0), 1e-6);
    }
    {
        ASSERT_THROW({
            double tmp = _neg_num1.sqrt();
            (void)tmp;
        }, std::exception);
    }

    // isqrt
    {
        BigInteger num("144");
        ASSERT_EQ(num.isqrt(), BigInteger("12"));
    }
    {
        BigInteger num("150");
        ASSERT_EQ(num.isqrt(), BigInteger("12"));
    }
    {
        ASSERT_EQ(_pos_zero.isqrt(), BigInteger("0"));
    }
    {
        ASSERT_EQ(_pos_one.isqrt(), BigInteger("1"));
    }
    {
        ASSERT_THROW({
            BigInteger tmp = _neg_num1.isqrt();
            (void)tmp;
        }, std::exception);
    }

    // is_prime
    {
        BigInteger num("2");
        ASSERT_TRUE(num.is_prime(5));
    }
    {
        BigInteger num("3");
        ASSERT_TRUE(num.is_prime(5));
    }
    {
        BigInteger num("4");
        ASSERT_FALSE(num.is_prime(5));
    }
    {
        ASSERT_FALSE(_neg_num1.is_prime(5));
    }
    {
        BigInteger num("104729");
        ASSERT_TRUE(num.is_prime(5));
    }
    {
        BigInteger num("104730");
        ASSERT_FALSE(num.is_prime(5));
    }
    {
        BigInteger carmichael("561");
        ASSERT_FALSE(carmichael.is_prime(5));
    }
}

#endif

#if SUPPORT_EVAL == 1

TEST_F(TestBigInteger, TestEvaluation)
{
    {
        std::string json = R"(
        {
            "op":"+",
            "left": 123,
            "right": 456
        }
        )";
        BigInteger result = eval(json);
        ASSERT_EQ(result, BigInteger(123 + 456));
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
        ASSERT_EQ(result, BigInteger(123 + 456));
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
        ASSERT_EQ(result, BigInteger(123 + (12345678901234567890 * (34 % 1))));
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
        ASSERT_EQ(result, BigInteger(123 + (12345678901234567890 * (34 % 1))));
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
        ASSERT_EQ(result, BigInteger(1000 - 200 + (123456 / 123)));
    }
}
#endif
