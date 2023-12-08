# 🌐 Big Integer Arithmetic Library & Discrete Logarithm Calculator
[![Version Badge](https://img.shields.io/badge/version-1.0.0-blue)]() [![License Badge](https://img.shields.io/badge/license-MIT-green)]()

**_Visit [P.A.N.D.A.'s PUBAO Homepage](https://hacker-code-j.notion.site/2023-Fall-AAP-Team-3-P-A-N-D-A-FUBAO-8a09720a080c4ad5859913331f832d55?pvs=4)_**


This repository provides implementations of **Big Integer operations** and **Discrete Logarithm Problem** calculators.

- DLP_calculator/
    - hash/
      - hash.c
      - hash.h
      - main.c
      - Makefile
    - images/
      - PANDA_logo.png
    - Tests/
      - measure.c
      - measure.h
    - Views/
      - compare_chart.py
      - DIV_compare_chart.py
      - FastRed_compare_chart.py
      - MUL_compare_chart.py
      - SQU_compare_chart.py
      - success_chart.py
    - .gitignore
    - arithmetic.h
    - arithmetic.c
    - config.h
    - Doxyfile
    - Doxyfile.bak
    - libpubao.a
    - LICENSE.md
    - main.c
    - Makefile
    - README.md
    - utils.c
    - utils.h

<!-- - big_int_dlp_library/
    - docs/
        - installation.md
        - usage.md
        - ...
    - src/
        - c/
            - bigint/
                - bigint.h
                - bigint.c
            - dlp/
                - dlp.h
                - dlp.c
        - python/
            - bigint/
                - __init__.py
                - bigint.py
            - dlp/
                - __init__.py
                - dlp.py
    - tests/
        - c/
            - bigint/
                - test_bigint.c
            - dlp/
                - test_dlp.c
        - python/
            - bigint/
                - test_bigint.py
            - dlp/
                - test_dlp.py
    - examples/
        - c/
            - example_bigint.c
            - example_dlp.c
        - python/
            - example_bigint.py
            - example_dlp.py
    - scripts/
        - build.sh
        - test.sh
    - README.md
    - LICENSE
    - .gitignore
    - setup.py (for the Python package)
    - Makefile (for the C code compilation) -->


## 📌 Table of Contents

- [🌐 Big Integer Arithmetic Library \& Discrete Logarithm Calculator](#-big-integer-arithmetic-library--discrete-logarithm-calculator)
  - [📌 Table of Contents](#-table-of-contents)
  - [🛠 Installation](#-installation)
    - [C](#c)
    - [💡 Usage](#-usage)
  - [C](#c-1)
  - [📖 Docs](#-docs)
  - [🤝 Contributing](#-contributing)
  - [📜 License](#-license)

## 🛠 Installation

### C
- **Clone and Build**:

```bash
git clone https://github.com/Hacker-Code-J/DLP_calculator.git
cd DLP_calculator
make rebuild
make success
```
<!-- ### Python
- **Install via pip**:
```python
pip install ./path/to/repo
``` -->

### 💡 Usage
## C
- **Big Integer**:
```bash
./bigint
```
- **DLP**:
```bash
./dlp
```

<!-- ### Python
1. **Install via pip**:
```python
from big_int_dlp_library.python import bigint, dlp
``` -->

## 📖 Docs
📁 Dive deeper with the [docs/](docs/) directory.

## 🤝 Contributing
🖋 Want to contribute? Awesome! Please refer to `CONTRIBUTING.md`.

## 📜 License
🔐 This project is under a license. Kindly check the [LICENSE](LICENSE) file.
