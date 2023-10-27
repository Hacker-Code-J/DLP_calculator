# 🌐 Large Integer Arithmetic Library & Discrete Logarithm Calculator

[![Version Badge](https://img.shields.io/badge/version-1.0.0-blue)]() [![License Badge](https://img.shields.io/badge/license-MIT-green)]()

This project provides a comprehensive large integer arithmetic library, with a focus on cryptographic calculations, particularly in solving discrete logarithm problems using Pollard's Rho and the Baby-step/Giant-step algorithms.

## 📌 Table of Contents

- [🚀 Features](#features)
- [🛠 Getting Started](#getting-started)
- [💡 Usage](#usage)
  - [📊 Large Integer Arithmetic](#large-integer-arithmetic)
  - [🔑 Discrete Logarithm Calculator](#discrete-logarithm-calculator)
- [🤝 Contribute](#contribute)
- [📜 License](#license)

## 🚀 Features

- **💼 Large Integer Arithmetic**: A robust library that supports addition, subtraction, multiplication, division, and modular arithmetic operations for integers larger than machine limits.
- **🔒 Discrete Logarithm Solutions**: Efficient implementations of Pollard's Rho and Baby-step/Giant-step algorithms for solving discrete logarithms.

## 🛠 Getting Started

### Prerequisites

- GCC or any other standard C++ compiler
- [Optional] Any testing framework for running the tests

### Installation

```bash
git clone https://github.com/Hacker-Code-J/DLP_calculator.git
cd large-int-library-and-disc-log-calc
make
```

## 💡 Usage

### 📊 Large Integer Arithmetic

```cpp
#include "BigInteger.h"

// Usage example
BigInteger a("12345678901234567890");
BigInteger b("98765432109876543210");

BigInteger sum = a + b;
```

### 🔑 Discrete Logarithm Calculator

```cpp
#include "DiscreteLogarithmCalculator.h"

// Using Pollard's Rho
int x = DiscreteLogarithmCalculator::pollardsRho(base, target, modulus);

// Using Baby-step/Giant-step
int x = DiscreteLogarithmCalculator::babyStepGiantStep(base, target, modulus);
```

## 🤝 Contribute

Contributions are always welcome! Please read the [contribution guidelines](CONTRIBUTING.md) first.

## 📜 License

This project is licensed under the MIT License - see the [LICENSE.md](LICENSE.md) file for details.
