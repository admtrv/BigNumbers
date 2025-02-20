/*
 * TestBigRational.cpp
 */

#include <gtest/gtest.h>

#include "../bignumbers.h"

class TestBigRational : public ::testing::Test
{
protected:
    BigRational _zero = BigRational(0, 1);
    BigRational _one = BigRational(1, 1);
    BigRational _half = BigRational(1, 2);
    BigRational _third = BigRational(1, 3);

    BigRational _pos = BigRational(3, 4);
    BigRational _neg= BigRational(-3, 4);

};

TEST_F(TestBigRational, TestConstructor)
{
    {
        BigRational def;
        ASSERT_EQ(def, BigRational(0,1));
    }
    {
        BigRational num(3, 4);
        ASSERT_EQ(num, BigRational("3", "4"));
    }
    {
        // -6/-8 => +6/8 => 3/4
        BigRational num(-6, -8);
        ASSERT_EQ(num, _pos);
    }
    {
        // -6/8 => -3/4
        BigRational num(-6, 8);
        ASSERT_EQ(num, _neg);
    }
    {
        // 6/-8 => -3/4
        BigRational num("6", "-8");
        ASSERT_EQ(num, _neg);
    }
    {
        // 0/3456 => 0/1
        BigRational num("0", "3456");
        ASSERT_EQ(num, _zero);
    }
    {
        // zero division
        ASSERT_THROW({
            BigRational tmp(1, 0);
        }, std::invalid_argument);
    }
}

TEST_F(TestBigRational, TestUnary)
{
    {
        ASSERT_EQ(+_neg, _neg);
        ASSERT_EQ(-_neg, _pos);
    }
    {
        ASSERT_EQ(+_zero, _zero);
        ASSERT_EQ(-_zero, _zero);
    }
}

TEST_F(TestBigRational, TestLogicalOperators)
{
    BigRational two_four(2,4);
    BigRational neg_divisor(3, -4);

    ASSERT_EQ(_half, two_four);
    ASSERT_EQ(_neg, neg_divisor);
    ASSERT_NE(_neg, _third);

    ASSERT_LT(_neg, _half);
    ASSERT_LT(_half, _pos);
    ASSERT_LT(_zero, _half);

    ASSERT_LE(_half, two_four);
    ASSERT_LE(_half, _pos);
    ASSERT_GE(_pos, _half);
}

#if SUPPORT_IFSTREAM == 1

TEST_F(TestBigRational, TestInputOutput)
{
    {
        std::istringstream input_stream("3/4");
        BigRational num;
        input_stream >> num;
        ASSERT_EQ(num, _pos);
    }
    {
        std::istringstream input_stream("-3/-4");
        BigRational num;
        input_stream >> num;
        ASSERT_EQ(num, _pos);
    }
    {
        std::istringstream input_stream("1");
        BigRational num;
        input_stream >> num;
        // 1 => 1/1
        ASSERT_EQ(num, _one);
    }
    {
        std::ostringstream oss;
        oss << _pos;
        ASSERT_EQ(oss.str(), "3/4");
    }
    {
        std::istringstream input_stream("abc/2");
        BigRational num;
        input_stream >> num;
        ASSERT_FALSE(input_stream.good());
    }
    {
        std::istringstream input_stream("123/");
        BigRational num;
        input_stream >> num;
        ASSERT_FALSE(input_stream.good());
    }
}

#endif

TEST_F(TestBigRational, TestArithmeticOperators)
{
    // Addition
    {
        BigRational sum = _half + _third;
        ASSERT_EQ(sum, BigRational(5,6));
    }
    {
        BigRational sum = _neg + _half;
        ASSERT_EQ(sum, BigRational(-1,4));
    }

    // Subtraction
    {
        BigRational diff = _half - _third;
        ASSERT_EQ(diff, BigRational(1,6));
    }
    {
        BigRational diff = _neg - _half;
        ASSERT_EQ(diff, BigRational(-5,4));
    }

    // Multiplication
    {
        BigRational prod = _pos * _half;
        ASSERT_EQ(prod, BigRational(3,8));
    }
    {
        BigRational prod = _neg * _third;
        ASSERT_EQ(prod, BigRational(-1,4));
    }

    // Division
    {
        BigRational quot = _pos / _half;
        ASSERT_EQ(quot, BigRational(3,2));
    }
    {
        BigRational quot = _neg / _third;
        ASSERT_EQ(quot, BigRational(-9,4));
    }
    {
        // zero division
        ASSERT_THROW({
            BigRational tmp = _pos / _zero;
            (void)tmp;
        }, std::logic_error);
    }
}

#if SUPPORT_MORE_OPS == 1

TEST_F(TestBigRational, TestMoreOperators)
{
    // sqrt
    {
        BigRational num(4,9);
        double result = num.sqrt(); // ~ 0.6666667
        ASSERT_NEAR(result, 2.0/3.0, 1e-9);
    }
    {
        // negative sqrt
        ASSERT_THROW({
            double tmp = _neg.sqrt();
            (void)tmp;
        }, std::exception);
    }

    // isqrt
    {
        BigRational num(4,9);
        BigInteger result = num.isqrt();
        ASSERT_EQ(result, BigInteger(0));
    }
    {
        BigRational num(9,4);
        BigInteger result = num.isqrt();
        ASSERT_EQ(result, BigInteger(1));
    }
    {
        // negative sqrt
        ASSERT_THROW({
            BigInteger tmp = _neg.isqrt();
            (void)tmp;
        }, std::exception);
    }
}

#endif
