#include "Render.hpp"

// #include <fcntl.h>
// #include <termios.h>
// #include <unistd.h>

// #include <cstdio>
// #include <cstdlib>
// #include <iostream>
// #include <thread>

// #include "Game.hpp"
// #include "Input.hpp"
// #include "stucture.hpp"


void Render::draw_frame(const Game& game) {
    // for (int fy = 0; fy < FIELD_H; fy++) {
    //     for (int fx = 0; fx < FIELD_W; fx++) {
    //         // draw left wall
    //         if (fx == 0) {
    //             std::cout << "<!";
    //             continue;
    //         }
    //         // draw right wall
    //         if (fx == FIELD_W - 1) {
    //             std::cout << "!>";
    //             continue;
    //         }
    //         // draw the ground
    //         if (fy == FIELD_H - 1) {
    //             std::cout << "==";
    //             continue;
    //         }

    //         // check if there is the block
    //         if (field[fy][fx]) {
    //             std::cout << "[]";
    //             continue;
    //         }

    //         // check if there is tetromino in current location
    //         bool drawn = false;
    //         for (int ty = 0; ty < 4 && !drawn; ty++) {
    //             for (int tx = 0; tx < 4 && !drawn; tx++) {
    //                 if (shapes[t.type][t.rotation][ty][tx]) {
    //                     int world_x = t.x + tx;
    //                     int world_y = t.y + ty;
    //                     if (world_x == fx && world_y == fy) {
    //                         std::cout << "[]";
    //                         drawn = true;
    //                     }
    //                 }
    //             }
    //         }

    //         // if there is empty space draw ..
    //         if (!drawn)
    //             std::cout << "..";
    //     }
    //     std::cout << '\n';
    // }
    // рисуем поле
    for(int y=0; y<Game::FIELD_H; ++y)
        for(int x=0; x<Game::FIELD_W; ++x)
            mvaddch(y, x*2, game.field[y][x] ? '#' : '.');

    // рисуем активное тетромино
    for(int ty=0; ty<4; ++ty)
        for(int tx=0; tx<4; ++tx)
            if(game.shapes[game.t.type][game.t.rotation][ty][tx])
                mvaddch(game.t.y + ty,
                        (game.t.x + tx)*2, '#');
};

// void Render::draw_field(ScreenBuffer& buf, int ox, int oy) {
//     /*
//       BUG: класс game все знает куда падает тетромино тоесть игра работает но вот screenbuffer
//       ничего незнает от где  находится упавшие тетромино и за чего не рисует их но поле
//       заполняется
//     */

//     // рисуем поле + рамку
//     for (int y = 0; y < FIELD_H; ++y) {
//         for (int x = 0; x < FIELD_W; ++x) {
//             int sx = ox + x * 2;  // если хочешь две клетки на символ
//             int sy = oy + y;

//             // Рисуем вертикальные стенки
//             if (x == 0 || x == FIELD_W - 1) {
//                 buf.set(sx, sy, '|');
//             }
//             // Рисуем нижнюю границу
//             else if (y == FIELD_H - 1) {  // Используем FIELD_H - 1 для нижней границы
//                 buf.set(sx, sy, '=');
//             }
//             // Заполненные клетки поля
//             else if (field[y][x]) {
//                 buf.set(sx, sy, '#');
//             }
//             // Пустые клетки поля
//             else {
//                 buf.set(sx, sy, '.');
//             }
//         }
//     }

//     // Рисуем тетромино
//     for (int ty = 0; ty < 4; ++ty) {
//         for (int tx = 0; tx < 4; ++tx) {
//             if (shapes[t.type][t.rotation][ty][tx]) {
//                 int world_x = t.x + tx;
//                 int world_y = t.y + ty;

//                 // Проверка, чтобы тетромино не выходило за пределы поля
//                 if (world_x >= 0 && world_x < FIELD_W && world_y >= 0 && world_y < FIELD_H) {
//                     buf.set(ox + world_x * 2, oy + world_y, '#');
//                 }
//             }
//         }
//     }
// };

// void Render::draw_score() {
//     std::cout << "score: " << score;
//     std::cout << "\n";
// };

// void Render::draw(DoubleBuffer& doubleBuffer) {
//     clear_screen();
//     sleep_ms(50);  // Временная задержка

//     // Рисуем на back-буфере
//     draw_field(doubleBuffer.back, 0, 0);  // rewrite it

//     // Переключаем back и front буферы
//     doubleBuffer.swap();

//     // Отображаем front буфер на экране (это то, что видит пользователь)
//     const ScreenBuffer& front = doubleBuffer.display();
//     for (int y = 0; y < FIELD_H; ++y) {
//         for (int x = 0; x < FIELD_W * 2; ++x) {  // умножаем на 2 для удлиненной клетки
//             std::cout << front.get(x, y);
//         }
//         std::cout << "\n";
//     }
// };

// //  void draw() {
// //      clear_screen();
// //      sleep_ms(50);  // must be a int variable i need to investigate that. NOTE that is
// temporay
// //                     // version because is can get faster or slower by different cpu.

// //                     // Конструктор DoubleBuffer, передаем параметры ширины и высоты
// //      DoubleBuffer doubleBuffer(FIELD_W * 2, FIELD_H);  // Умножаем FIELD_W на 2 для удлинения
// //      клетки draw_field(doubleBuffer.back, 2, 2);

// //      doubleBuffer.swap();
// //    //  draw_score();
// //    //  draw_frame();
// // draw_frame()
// // };

#include "ncurses.h"

void Render::draw(const Game& game) {
    refresh();
    sleep_ms(50);  // must be a int variable i need to investigate that. NOTE that is
    draw_frame(game);
    //mvprintw(5, 10, "Hello tetris");
};
