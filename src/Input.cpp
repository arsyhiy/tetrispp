#include "Input.hpp"

#include <fcntl.h>
#include <termios.h>
#include <unistd.h>

#include <chrono>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <thread>

#include "Game.hpp"
#include "stucture.hpp"

extern Tetromino t;

bool Input::kbhit() {
    struct termios settings;
    int ch, stdinFlags;  // make sure input is a good naming

    tcgetattr(STDIN_FILENO, &settings);  // Getting the current terminal settings
    settings.c_lflag &= ~ICANON;         // Disable canonical mode (input is available immediately,
                                         // without waiting for Enter)
    settings.c_cc[VMIN] = 1;   // Require at least 1 character to return from read// Require at
                               // least 1 character to return from read
    settings.c_cc[VTIME] = 0;  // No timeout
    tcsetattr(STDIN_FILENO, TCSANOW, &settings);  // setting new terminal settings
    stdinFlags = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, stdinFlags | O_NONBLOCK);

    ch = getchar();

    if (ch != EOF) {
        ungetc(ch, stdin);  // Return the character to the input stream
        return true;
    }
    return false;
};

void Input::handle_input() {
    Game game;
    if (kbhit()) {
        char input = getchar();

        if (input == 27) {  // ESC for exit
            game_is_running = false;
        } else if (input == 'a' || input == 'A') {  // left
            game.move_left(t);
        } else if (input == 'd' || input == 'D') {  // right
            game.move_right(t);
        } else if (input == 's' || input == 'S') {  // down
            game.move_down(t);
        } else if (input == 'w' || input == 'W') {  // rotate
            game.rotate(t);
        }
    }
};
