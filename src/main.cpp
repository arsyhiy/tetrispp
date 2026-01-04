
// // // int main() {
// // //     Render render;
// // //     Input input;
// // //     Game game;
// // //
// // //     // Сохраняем старое состояние терминала
// // //     Render::TerminalState oldState = render.saveTerminal();
// // //
// // //     // entering alt buffer
// // //     std::cout << "\033[?1049h";
// // //     // hide cursor
// // //     std::cout << "\033[?25l";
// // //     render.disableEcho();
// // //
// // //     while (game_is_running == true) {
// // //         input.handle_input();
// // //         game.update();
// // //         render.draw();
// // //     };
// // //
// // //     render.restoreTerminal(oldState);
// // //     // exeting alt buffer
// // //     std::cout << "\033[?1049l";
// // //     // show cursor again
// // //     std::cout << "\033[?25h";
// // //     // enableEcho();
// // //
// // //     return 0;
// // // };
// // //
// //

// // #include "Game.hpp"
// // #include "Input.hpp"
// // #include "Render.hpp"
// // #include "game_state.hpp"
// #include "ncurses.h"

// int main() {
//     // Render render;
//     // Input input;
//     // Game game;

//     // // Сохраняем старое состояние терминала
//     // //    TerminalState oldState = render.saveTerminal();

//     // // entering alt buffer
//     // std::cout << "\033[?1049h";
//     // // hide cursor
//     // std::cout << "\033[?25l";
//     // render.disableEcho();

//     // // Создаем объект DoubleBuffer один раз
//     // // перенести определение куда то еще
//     // DoubleBuffer doubleBuffer(FIELD_W * 2, FIELD_H);  // Умножаем FIELD_W на 2 для удлинения
//     // // клетки

//     // while (game_is_running) {
//     //     input.handle_input();
//     //     game.update();

//     //     // Используем doubleBuffer для рисования
//     //     // я думаю что этот файл не должен знать про doubleBuffer
//     //     render.draw(doubleBuffer);  // передаем doubleBuffer в draw.
//     // };

//     // //  render.restoreTerminal(oldState);
//     // // exiting alt buffer
//     // std::cout << "\033[?1049l";
//     // // show cursor again
//     // std::cout << "\033[?25h";
//     // render.enableEcho();

//     return 0;
// };

/*
 что еще написать:
управление сбоку
счетчик очков
 */

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
    while (game.is_running) {
      input.handle_input(game);
        game.update();
        render.draw(game);
    };
    endwin();
    // ncurses приложение в миниатюре
    // initscr();
    // noecho();
    // curs_set(0);
    // keypad(stdscr, TRUE);
    // mvprintw(5, 10, "Hello ncurses!");
    // refresh();
    // getch();
    // endwin();

    return 0;
};
