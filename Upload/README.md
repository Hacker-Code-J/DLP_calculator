# Using Doxygen with Multiple Interdependent Files

When documenting a set of interdependent files like `config.h`, `utils.h`, `utils.c`, `opr.h`, and `opr.c`, ensure that Doxygen processes these files correctly by following the steps below:

## Step 1: Install Doxygen

Install Doxygen on your Linux system using the package manager:

```bash
sudo apt-get update
sudo apt-get install doxygen
```

## Step 2: Generate Default Doxygen Configuration

Generate a default Doxygen configuration file named `Doxyfile`:

```bash
doxygen -g
```

## Step 3: Configure Doxygen

Edit the `Doxyfile` to include all relevant source files:

```plaintext
INPUT = ./config.h ./utils.h ./utils.c ./opr.h ./opr.c
```

Set the `RECURSIVE` tag to `YES` if your files are in different subdirectories:

```plaintext
RECURSIVE = YES
```

Enable source browsing and graphs if needed:

```plaintext
SOURCE_BROWSER = YES
HAVE_DOT = YES
CALL_GRAPH = YES
CALLER_GRAPH = YES
```

*Note*: Install Graphviz for graph generation:

```bash
sudo apt-get install graphviz
```

## Step 4: Document Your Source Code

Make sure your source files have Doxygen-compatible comments:

```c
/**
 * @file config.h
 * @brief Configuration settings for the project.
 */
```

## Step 5: Run Doxygen

Generate the documentation:

```bash
doxygen Doxyfile
```

## Step 6: Review the Documentation

Check the output directory for the generated documentation, typically found in the `html` directory.

## Step 7: Fine-Tune Configuration (Optional)

Adjust the `Doxyfile` as needed for additional customization:

```plaintext
EXTRACT_ALL = YES
```

By following these instructions, Doxygen will respect the dependencies between the included files in your project.

## Step 8: Manual Document

```plaintext
cd latex
make all
```

Open refman.pdf