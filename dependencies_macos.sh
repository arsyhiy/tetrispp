#!/bin/bash

# for macOS only purposes

# Install Homebrew if missing
if ! command -v brew &>/dev/null; then
	echo "Homebrew not found. Installing..."
	/bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"
fi

# Install dependencies
brew install \
	cmake \
	llvm \
	clang-format \
	ncurses

echo
echo "Done."
echo
echo "Build example:"
echo "cmake -B build -DCMAKE_BUILD_TYPE=Debug"
echo "cmake --build build"
