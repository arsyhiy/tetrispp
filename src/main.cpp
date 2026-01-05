#include "Game.hpp"
#include "Input.hpp"
#include "Render.hpp"
#include "iostream"
#include "ncurses.h"

int main() {
    Game game;
    Input input;
    Render render;

    initscr();
    noecho();
    curs_set(0);

    game.spawn_new_tetromino();
    while (game.is_running) {
        input.handle_input(game);
        game.update();
        render.draw(game);
    };
    endwin();

    return 0;
};
