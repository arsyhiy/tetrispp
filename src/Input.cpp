#include "Input.hpp"

#include <fcntl.h>
#include <termios.h>
#include <unistd.h>

#include "Game.hpp"

bool Input::kbhit() {
    struct termios settings;
    int stdinFlags;

    tcgetattr(STDIN_FILENO, &settings);  // Get the current terminal settings
    settings.c_lflag &= ~ICANON;         // Disable canonical mode (input is available immediately,
                                         // without waiting for Enter)
    settings.c_cc[VMIN] = 1;             // Require at least 1 character to return from read
    settings.c_cc[VTIME] = 0;            // No timeout
    tcsetattr(STDIN_FILENO, TCSANOW, &settings);  // set new terminal settings
    stdinFlags =
        fcntl(STDIN_FILENO, F_GETFL, 0);  // saving current flags for standard input (stdin)

    // Set the O_NONBLOCK flag for stdin, making it non-blocking.
    // In non-blocking mode, read operations won’t block the program. It will immediately return if
    // there's no input.
    fcntl(STDIN_FILENO, F_SETFL, stdinFlags | O_NONBLOCK);

    // Try to read a character from stdin
    int ch = getchar();

    // If a character was successfully read (ch is not EOF), return it to the input stream
    // and return true, indicating that there is input available.
    if (ch != EOF) {
        ungetc(ch, stdin);  // Return the character to the input stream
        return true;        // Return true because input is available
    }
    // If no character was read (EOF), return false
    return false;
};

void Input::handle_input(Game& game) {
    if (kbhit()) {
        char input = getchar();

        // 27 means esc button
        if (input == 27 || input == KEY_EXIT) {
            game.is_running = false;
        } else if (input == 'a' || input == 'A') {
            game.move_left(game.t);
        } else if (input == 'd' || input == 'D') {
            game.move_right(game.t);
        } else if (input == 's' || input == 'S') {
            game.move_down(game.t);
        } else if (input == 'w' || input == 'W') {
            game.rotate(game.t);
        }
    }
};
