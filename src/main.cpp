#include "Game.hpp"
#include "Input.hpp"
#include "Render.hpp"

int main() {
    Game game;
    Input input;
    Render render;

    // initialization
    initscr();
    noecho();
    curs_set(0);

    // NOTE: проверить как работает спавн тетро так как он  спавнит один из типов и только.
    game.spawn_new_tetromino();
    while (game.is_running) {
        input.handle_input(game);
        game.update();
        render.draw(game);
        nodelay(stdscr, TRUE);  // ← здесь обязательно!
    };
    endwin();

    return 0;
};
