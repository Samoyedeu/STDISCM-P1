# Project Title

## Overview
This project allows you to compile and run multiple variants of a program for different thread configurations and end-range numbers to search. You can either use the command line interface (CLI) or Visual Studio Code (VS Code) for this.

## Prerequisites

### For CLI:
1. Ensure you have **GCC** installed. You can check if GCC is installed by typing `g++ --version` in your terminal. If not, install it using:
   - **Linux (Ubuntu/Debian)**: `sudo apt install g++`
   - **macOS**: `brew install gcc`
   - **Windows**: Download from [MinGW](https://sourceforge.net/projects/mingw/).
2. **Navigate to the path/variant** where the source files (`A1B1STDISCM.cpp`, `A2B1STDISCM.cpp`, etc.) are located.

## Configuration

- `x=[NUMBER OF THREADS]`
- `y=[END RANGE NUMBER TO SEARCH]`


## Compilation & Running
For Variant 1:
In the terminal, type:
- g++ -std=c++11 -o A1B1STDISCM A1B1STDISCM.cpp -lpthread
- A1B1STDISCM

For Variant 2:
In the terminal, type:
- g++ -std=c++11 -o A2B1STDISCM A2B1STDISCM.cpp -lpthread
- A2B1STDISCM

For Variant 3:
In the terminal, type:
- g++ -std=c++11 -o A1B2STDISCM A1B2STDISCM.cpp -lpthread
- A1B2STDISCM

For Variant 4:
In the terminal, type:
- g++ -std=c++11 -o A2B2STDISCM A2B2STDISCM.cpp -lpthread
- A2B2STDISCM

### For Visual Studio Code (VS Code):
1. Open the project folder in VS Code.
2. Press Ctrl + F5 to build and run the program.
Note: Ensure MinGW is installed and configured in your system PATH for Windows.

### Troubleshooting
GCC not found: If you encounter issues with GCC, make sure it is correctly installed and available in your system's PATH.
MinGW issues: If you're using MinGW on Windows and encounter errors, ensure MinGW is properly installed and added to the system PATH.
