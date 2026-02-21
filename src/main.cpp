#include "Game.hpp"
#include "Input.hpp"
#include "Render.hpp"
#include <chrono>
#include <ncurses.h>
#include <iostream>

int main() {
    Game game;
    Input input;
    Render render;

    if (initscr() == nullptr) {
        std::cerr << "Не удалось инициализировать ncurses\n";
        return 1;
    }

    noecho();
    curs_set(0);
    keypad(stdscr, TRUE);
    nodelay(stdscr, TRUE);
    flushinp();

    game.init_field();
    game.spawn_new_tetromino();

    auto last_time = std::chrono::high_resolution_clock::now();

    while (game.is_running) {
        auto now = std::chrono::high_resolution_clock::now();
        float delta_time =
            std::chrono::duration<float>(now - last_time).count();
        last_time = now;

        input.handle_input(game);
        game.update(delta_time);
        render.draw(game);

        napms(1);
    }

    endwin();
    return 0;
}
