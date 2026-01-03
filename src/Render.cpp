#include "Render.hpp"

#include <fcntl.h>
#include <termios.h>
#include <unistd.h>

#include <chrono>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <thread>


#include "Game.hpp"
#include "Input.hpp"

#include "stucture.hpp"

extern Tetromino t;

void Render::draw_frame() {
    for (int fy = 0; fy < FIELD_H; fy++) {
        for (int fx = 0; fx < FIELD_W; fx++) {
            // draw left wall
            if (fx == 0) {
                std::cout << "<!";
                continue;
            }
            // draw right wall
            if (fx == FIELD_W - 1) {
                std::cout << "!>";
                continue;
            }
            // draw the ground
            if (fy == FIELD_H - 1) {
                std::cout << "==";
                continue;
            }

            // check if there is the block
            if (field[fy][fx]) {
                std::cout << "[]";
                continue;
            }

            // check if there is tetromino in current location
            bool drawn = false;
            for (int ty = 0; ty < 4 && !drawn; ty++) {
                for (int tx = 0; tx < 4 && !drawn; tx++) {
                    if (shapes[t.type][t.rotation][ty][tx]) {
                        int world_x = t.x + tx;
                        int world_y = t.y + ty;
                        if (world_x == fx && world_y == fy) {
                            std::cout << "[]";
                            drawn = true;
                        }
                    }
                }
            }

            // if there is empty space draw ..
            if (!drawn)
                std::cout << "..";
        }
        std::cout << '\n';
    }
};

void Render::draw_field(ScreenBuffer& buf, int ox, int oy) {

};

void Render::draw_score() {

};

void Render::disableEcho() {

};

void Render::enableEcho() {

};

void restoreTerminal(const TerminalState& state);

void Render::draw(DoubleBuffer& doubleBuffer) {

};

// draw field and tetromino
void draw_frame() {};
//
// void draw_frame_border() {
//     // Рисуем верхнюю и нижнюю границу
//     std::cout << "<!";
//     for (int fx = 1; fx < FIELD_W - 1; fx++) {
//         std::cout << "  ";  // Пустое пространство внутри
//     }
//     std::cout << "!>" << std::endl;
//
//     // Рисуем левую и правую границу
//     for (int fy = 1; fy < FIELD_H - 1; fy++) {
//         std::cout << "  ";  // Пустое пространство слева
//         for (int fx = 1; fx < FIELD_W - 1; fx++) {
//             std::cout << "..";  // Пустое пространство внутри
//         }
//         std::cout << "  " << std::endl;
//     }
//
//     std::cout << "<!";
//     for (int fx = 1; fx < FIELD_W - 1; fx++) {
//         std::cout << "==";  // Рисуем землю
//     }
//     std::cout << "!>" << std::endl;
// }

void draw_field(ScreenBuffer& buf, int ox, int oy) {
    // рисуем поле + рамку
    for (int y = 0; y < FIELD_H; ++y) {
        for (int x = 0; x < FIELD_W; ++x) {
            int sx = ox + x * 2;  // если хочешь две клетки на символ
            int sy = oy + y;

            // Рисуем вертикальные стенки
            if (x == 0 || x == FIELD_W - 1) {
                buf.set(sx, sy, '|');
            }
            // Рисуем нижнюю границу
            else if (y == FIELD_H - 1) {  // Используем FIELD_H - 1 для нижней границы
                buf.set(sx, sy, '=');
            }
            // Заполненные клетки поля
            else if (field[y][x]) {
                buf.set(sx, sy, '#');
            }
            // Пустые клетки поля
            else {
                buf.set(sx, sy, '.');
            }
        }
    }

    // Рисуем тетромино
    for (int ty = 0; ty < 4; ++ty) {
        for (int tx = 0; tx < 4; ++tx) {
            if (shapes[t.type][t.rotation][ty][tx]) {
                int world_x = t.x + tx;
                int world_y = t.y + ty;

                // Проверка, чтобы тетромино не выходило за пределы поля
                if (world_x >= 0 && world_x < FIELD_W && world_y >= 0 && world_y < FIELD_H) {
                    buf.set(ox + world_x * 2, oy + world_y, '#');
                }
            }
        }
    }
}

void draw_score() {
    std::cout << "score: " << score;
    std::cout << "\n";
};

// Функция для отключения отображения символов при вводе
void disableEcho() {
    struct termios settings;
    tcgetattr(STDIN_FILENO, &settings);
    settings.c_lflag &= ~ECHO;  // Отключаем отображение символов
    tcsetattr(STDIN_FILENO, TCSANOW, &settings);
}

// Функция для включения отображения символов
void enableEcho() {
    struct termios settings;
    tcgetattr(STDIN_FILENO, &settings);
    settings.c_lflag |= ECHO;  // Включаем отображение символов
    tcsetattr(STDIN_FILENO, TCSANOW, &settings);
}

// Сохраняем состояние терминала
TerminalState saveTerminal() {
    TerminalState state;
    tcgetattr(STDIN_FILENO, &state.settings);
    return state;
}
// Восстанавливаем состояние терминала
void restoreTerminal(const TerminalState& state) {
    tcsetattr(STDIN_FILENO, TCSANOW, &state.settings);
}

void clear_screen() { std::cout << "\033[H\033[2J"; };

void sleep_ms(int ms) { std::this_thread::sleep_for(std::chrono::milliseconds(ms)); };

// void draw() {
//     clear_screen();
//     sleep_ms(50);  // must be a int variable i need to investigate that. NOTE that is
//     temporay
//                    // version because is can get faster or slower by different cpu.
//
//
//                    // Конструктор DoubleBuffer, передаем параметры ширины и высоты
//     DoubleBuffer doubleBuffer(FIELD_W * 2, FIELD_H);  // Умножаем FIELD_W на 2 для удлинения
//     клетки draw_field(doubleBuffer.back, 2, 2);
//
//     doubleBuffer.swap();
//   //  draw_score();
//   //  draw_frame();
// };

void draw(DoubleBuffer& doubleBuffer) {
    clear_screen();
    sleep_ms(50);  // Временная задержка

    // Рисуем на back-буфере
    draw_field(doubleBuffer.back, 0, 0);  // rewrite it

    // Переключаем back и front буферы
    doubleBuffer.swap();

    // Отображаем front буфер на экране (это то, что видит пользователь)
    const ScreenBuffer& front = doubleBuffer.display();
    for (int y = 0; y < FIELD_H; ++y) {
        for (int x = 0; x < FIELD_W * 2; ++x) {  // умножаем на 2 для удлиненной клетки
            std::cout << front.get(x, y);
        }
        std::cout << "\n";
    }
}
