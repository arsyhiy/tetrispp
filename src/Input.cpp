#include "Input.hpp"

void Input::handle_input(Game& game) {
    int ch = getch();
    if (ch == ERR)
        return;

    switch (ch) {
        case 27:  // this is Esc
        case 'q':
        case 'Q':
            game.is_running = false;
            break;

        case 'a':
        case 'A':
        case KEY_LEFT:
            game.move_left(game.t);
            break;

        case 'd':
        case 'D':
        case KEY_RIGHT:
            game.move_right(game.t);
            break;

        case 's':
        case 'S':
        case KEY_DOWN:
            game.move_down(game.t);
            break;

        case 'w':
        case 'W':
        case KEY_UP:
            game.rotate(game.t);
            break;
    }
}
