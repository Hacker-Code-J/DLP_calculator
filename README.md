# 🌐 Big Integer Arithmetic Library & Discrete Logarithm Calculator

[![Version Badge](https://img.shields.io/badge/version-1.0.0-blue)]() [![License Badge](https://img.shields.io/badge/license-MIT-green)]()

This repository provides implementations of **Big Integer** operations and **Discrete Logarithm Problem** calculators.

- big_int_dlp_library/
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
    - Makefile (for the C code compilation)


## 📌 Table of Contents

- [🛠 Installation](#installation)
- [💡 Usage](#usage)
- [📖 Docs](#docs)
- [🤝 Contributing](#contributing)
- [📜 License](#license)

## 🛠 Installation

### C
- **Clone and Build**:

```bash
git clone https://github.com/yourusername/big_int_dlp_library.git
cd big_int_dlp_library
make
```
### Python
- **Install via pip**:
```python
pip install ./path/to/repo
```
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

### Python
1. **Install via pip**:
```python
from big_int_dlp_library.python import bigint, dlp
```

## 📖 Docs
📁 Dive deeper with the [docs/](docs/) directory.

## 🤝 Contributing
🖋 Want to contribute? Awesome! Please refer to `CONTRIBUTING.md`.

## 📜 License
🔐 This project is under a license. Kindly check the [LICENSE](LICENSE) file.
