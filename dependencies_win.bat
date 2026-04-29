@echo off

echo Installing development environment...

winget install Kitware.CMake -e
winget install MSYS2.MSYS2 -e

echo.
echo ============================================
echo Open "MSYS2 UCRT64" and run:
echo.
echo pacman -Syu
echo.
echo Restart MSYS2 UCRT64 and then run:
echo.
echo pacman -S --needed ^
mingw-w64-ucrt-x86_64-gcc ^
mingw-w64-ucrt-x86_64-cmake ^
mingw-w64-ucrt-x86_64-make ^
mingw-w64-ucrt-x86_64-ncurses ^
mingw-w64-ucrt-x86_64-clang ^
clang-format
echo ============================================

pause