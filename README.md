# BigNumbers

This is a C++ header-only library that implements classes to represent and operate on arbitrarily large numbers. It provides basic arithmetic, comparison, and additional operations suitable for high-precision computations.

## `BigInteger`

### Features

#### Constructors
- **Default Constructor**: Initializes the value to 0.
- **Integer Constructor**: Initializes from `int64_t`.
- **String Constructor**: Initializes from a string representing an integer (supports `-`, `+`, and leading zeros).

#### Supported Operations

##### Arithmetic Operators
```cpp
BigInteger a("1234567891011121314151617181920");
BigInteger b(-6789);

BigInteger sum = a + b;         // Addition
BigInteger diff = a - b;        // Subtraction
BigInteger prod = a * b;        // Multiplication
BigInteger quot = a / b;        // Division
BigInteger rem = a % b;         // Modulus
```

##### Compound Assignment Operators
```cpp
a += b;
a -= b;
a *= b;
a /= b;
a %= b;
```

##### Unary Operators
```cpp
BigInteger c = +a;  // Don't change sign
BigInteger d = -b;  // Change sign to opposite
```

##### Comparison Operators
```cpp
if (a == b) { ... }
if (a != b) { ... }
if (a < b) { ... }
if (a > b) { ... }
if (a <= b) { ... }
if (a >= b) { ... }
```

##### Additional Operations
- **Square Root (Double Precision)**
  ```cpp
  double root = a.sqrt();
  ```
- **Integer Square Root (Only if defined)**
  ```cpp
  BigInteger root = a.isqrt();
  ```
- **Prime Check (Miller-Rabin)**
  ```cpp
  bool isPrime = a.is_prime(5); // 5 rounds of Miller-Rabin
  ```

##### Stream Input and Output
```cpp
std::cout << a << std::endl;
std::cin >> a;
```

##### JSON Expression Evaluation
Supports arithmetic expressions in JSON format, e.g.:
```cpp
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
BigInteger result = eval(json);   // Parse and evaluate expression
std::cout << result << std::endl; // 123
```

### Note
To enable extra features like `sqrt`, `isqrt`, and `is_prime`, set the appropriate macros (`SUPPORT_IFSTREAM`, `SUPPORT_MORE_OPS`, `SUPPORT_EVAL`) to `1` before including the header.


## `BigRational`

### Features

#### Constructors
- **Default Constructor**: Initializes the value to 0.
- **Fraction Constructor (Integer)**: Initializes from two `int64_t` values (numerator and denominator).
- **Fraction Constructor (String)**: Initializes from two strings representing the numerator and denominator.

#### Supported Operations

##### Arithmetic Operators
```cpp
BigRational a("3", "4");    // 3/4
BigRational b(2, 5);        // 2/5

BigRational sum = a + b;    // Addition
BigRational diff = a - b;   // Subtraction
BigRational prod = a * b;   // Multiplication
BigRational quot = a / b;   // Division
```

##### Compound Assignment Operators
```cpp
a += b;
a -= b;
a *= b;
a /= b;
```

##### Unary Operators
```cpp
BigRational c = +a;
BigRational d = -b;
```

##### Comparison Operators
```cpp
if (a == b) { ... }
if (a != b) { ... }
if (a < b) { ... }
if (a > b) { ... }
if (a <= b) { ... }
if (a >= b) { ... }
```

##### Additional Operations
- **Square Root (Double Precision)**
  ```cpp
  double root = a.sqrt();
  ```
- **Integer Square Root (Only if defined)**
  ```cpp
  BigInteger root = a.isqrt();
  ```

##### Stream Input and Output
```cpp
std::cout << a << std::endl;
std::cin >> a;
```

### Note
The `BigRational` class automatically reduces fractions to their simplest form upon creation or after any operation. The numerator and denominator are guaranteed to remain coprime.

```cpp
BigRational a(6, 4);
std::сout << a << std::endl; // "3/2"
```