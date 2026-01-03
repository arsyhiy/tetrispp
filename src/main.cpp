//
// #include <fcntl.h>
// #include <termios.h>
// #include <unistd.h>
//
// #include <chrono>
// #include <cstdio>
// #include <cstdlib>
// #include <iostream>
// #include <thread>
//
// // #include <vector>
//
// #include "stucture.hpp"

// Tetromino t = {0, 0, FIELD_W / 2, 0};  // make it random
// // static variables
//
// class Game {
//    public:
//     void move_down(Tetromino& t) {
//         // Let's check if we can move the tetromino down
//         if (can_move_down(t)) {
//             t.y++;  // If we can, we move the tetromino down one square.
//         } else {
//             // If we can't, we fix the tetromino in the field
//             // maybe is a good idea move to another Function called lock_in or something like
//             that for (int ty = 0; ty < 4; ++ty) {
//                 for (int tx = 0; tx < 4; ++tx) {
//                     if (shapes[t.type][t.rotation][ty][tx]) {
//                         int world_x = t.x + tx;
//                         int world_y = t.y + ty;
//                         if (world_y <
//                             FIELD_H - 1) {  // in fact FIELD_H is 1 hight smaller that it
//                             execpted
//                                             // but there is differents if we will write FIELD_H -
//                                             1
//                                             // or without - 1. but i will write with -1 tho avoid
//                                             // unexpected consequences
//                             field[world_y][world_x] = 1;  // We fix the block on the field
//                         }
//                     }
//                 }
//             }
//             // Tetromino is no longer moving, you can launch the next one
//             t.y = -1;  // We set a special value to signal that the tetromino is locked. NOTE:
//             check
//                        // that thing because i don't see  a special value where it's commin
//                        because
//                        // i dumb
//         }
//     };
//
//     void clear_line(int row) {
//         for (int i = row; i > 0; --i) {
//             for (int j = 0; j < FIELD_W; ++j) {
//                 if (i < FIELD_H - 1) {
//                     field[i][j] = field[i - 1][j];
//                 }
//             }
//         }
//
//         for (int j = 0; j < FIELD_W; ++j) {
//             field[0][j] = 0;
//         }
//     }
//     void clean_line() {
//         for (int i = FIELD_H - 1; i >= 1; --i) {
//             if (line_is_full(i)) {
//                 clear_line(i);
//                 score = score + 100;  // rewrite to tetromino score point
//                 i++;
//             }
//         }
//     }
//
//     bool line_is_full(int row) {
//         for (int b = 1; b < FIELD_W - 1; ++b) {
//             if (field[row][b] == 0) {
//                 return false;
//             };
//         };
//         return true;
//     };
//
//     bool can_move_down(const Tetromino& t) {
//         // Let's check if we can move the tetromino down
//         for (int ty = 0; ty < 4; ++ty) {
//             for (int tx = 0; tx < 4; ++tx) {
//                 // If the tetromino cell is active
//                 if (shapes[t.type][t.rotation][ty][tx]) {
//                     int world_x = t.x + tx;
//                     int world_y = t.y + ty + 1;  // We check 1 cell below
//
//                     // Проверяем, не выходит ли за пределы поля или не сталкивается с другим
//                     блоком if (world_y >= FIELD_H - 1 ||
//                         (world_x >= 1 && world_x < FIELD_W - 1 && field[world_y][world_x])) {
//                         return false;  // Невозможно двигаться вниз, либо достигли дна, либо
//                                        // столкнулись с блоком
//                     }
//                 }
//             }
//         }
//         return true;  // Тетромино можно двигать вниз
//     };
//
//     // Функция для перемещения тетромино влево/вправ
//     void move_left(Tetromino& t) {
//         // Проверяем, можем ли мы переместиться влево
//         for (int ty = 0; ty < 4; ++ty) {
//             for (int tx = 0; tx < 4; ++tx) {
//                 if (shapes[t.type][t.rotation][ty][tx]) {
//                     int world_x = t.x + tx - 1;
//                     int world_y = t.y + ty;
//
//                     if (world_x < 1 ||
//                         (world_y >= 0 && world_y < FIELD_H && field[world_y][world_x])) {
//                         return;  // Не можем двигаться влево
//                     }
//                 }
//             }
//         }
//
//         t.x--;  // Перемещаем влево
//     };
//
//     // Функция для перемещения тетромино вправо
//     void move_right(Tetromino& t) {
//         // We check if it goes out of bounds or collides with another block.
//         for (int ty = 0; ty < 4; ++ty) {
//             for (int tx = 0; tx < 4; ++tx) {
//                 if (shapes[t.type][t.rotation][ty][tx]) {
//                     int world_x = t.x + tx + 1;
//                     int world_y = t.y + ty;
//
//                     if (world_x >= FIELD_W - 1 ||
//                         (world_y >= 0 && world_y < FIELD_H && field[world_y][world_x])) {
//                         return;  // We can't move to the right
//                     }
//                 }
//             }
//         }
//
//         t.x++;  // Move to the right
//     };
//
//     // Function for rotating a tetromino
//     void rotate(Tetromino& t) {
//         int new_rotation = (t.rotation + 1) % 4;
//         // Let's check if we can rotate the tetromino
//
//         for (int ty = 0; ty < 4; ++ty) {
//             for (int tx = 0; tx < 4; ++tx) {
//                 if (shapes[t.type][new_rotation][ty][tx]) {
//                     int world_x = t.x + tx;
//                     int world_y = t.y + ty;
//
//                     if (world_x < 0 || world_x >= FIELD_W || world_y >= FIELD_H ||
//                         field[world_y][world_x]) {
//                         return;  // We can't turn
//                     }
//                 }
//             }
//         }
//
//         t.rotation = new_rotation;  // We are making a turn
//     };
//
//     void update() {
//         // we need colishion
//         move_down(t);  // i think move_down most be a update_game Function.
//         clean_line();
//     };
// };
//
// class Input {
//    public:
//     bool kbhit() {
//         struct termios settings;
//         int ch, stdinFlags;  // make sure input is a good naming
//
//         tcgetattr(STDIN_FILENO, &settings);  // Getting the current terminal settings
//         settings.c_lflag &= ~ICANON;  // Disable canonical mode (input is available immediately,
//                                       // without waiting for Enter)
//         settings.c_cc[VMIN] = 1;   // Require at least 1 character to return from read// Require
//         at
//                                    // least 1 character to return from read
//         settings.c_cc[VTIME] = 0;  // No timeout
//         tcsetattr(STDIN_FILENO, TCSANOW, &settings);  // setting new terminal settings
//         stdinFlags = fcntl(STDIN_FILENO, F_GETFL, 0);
//         fcntl(STDIN_FILENO, F_SETFL, stdinFlags | O_NONBLOCK);
//
//         ch = getchar();
//
//         if (ch != EOF) {
//             ungetc(ch, stdin);  // Return the character to the input stream
//             return true;
//         }
//         return false;
//     };
//
//     void handle_input() {
//         Game game;
//         if (kbhit()) {
//             char input = getchar();
//
//             if (input == 27) {  // ESC for exit
//                 game_is_running = false;
//             } else if (input == 'a' || input == 'A') {  // left
//                 game.move_left(t);
//             } else if (input == 'd' || input == 'D') {  // right
//                 game.move_right(t);
//             } else if (input == 's' || input == 'S') {  // down
//                 game.move_down(t);
//             } else if (input == 'w' || input == 'W') {  // rotate
//                 game.rotate(t);
//             }
//         }
//     };
// };
//
// class Render {
//    public:
//     // draw field and tetromino
//     void draw_frame() {
//         for (int fy = 0; fy < FIELD_H; fy++) {
//             for (int fx = 0; fx < FIELD_W; fx++) {
//                 // draw left wall
//                 if (fx == 0) {
//                     std::cout << "<!";
//                     continue;
//                 }
//                 // draw right wall
//                 if (fx == FIELD_W - 1) {
//                     std::cout << "!>";
//                     continue;
//                 }
//                 // draw the ground
//                 if (fy == FIELD_H - 1) {
//                     std::cout << "==";
//                     continue;
//                 }
//
//                 // check if there is the block
//                 if (field[fy][fx]) {
//                     std::cout << "[]";
//                     continue;
//                 }
//
//                 // check if there is tetromino in current location
//                 bool drawn = false;
//                 for (int ty = 0; ty < 4 && !drawn; ty++) {
//                     for (int tx = 0; tx < 4 && !drawn; tx++) {
//                         if (shapes[t.type][t.rotation][ty][tx]) {
//                             int world_x = t.x + tx;
//                             int world_y = t.y + ty;
//                             if (world_x == fx && world_y == fy) {
//                                 std::cout << "[]";
//                                 drawn = true;
//                             }
//                         }
//                     }
//                 }
//
//                 // if there is empty space draw ..
//                 if (!drawn)
//                     std::cout << "..";
//             }
//             std::cout << '\n';
//         }
//     };
//     //
//     // void draw_frame_border() {
//     //     // Рисуем верхнюю и нижнюю границу
//     //     std::cout << "<!";
//     //     for (int fx = 1; fx < FIELD_W - 1; fx++) {
//     //         std::cout << "  ";  // Пустое пространство внутри
//     //     }
//     //     std::cout << "!>" << std::endl;
//     //
//     //     // Рисуем левую и правую границу
//     //     for (int fy = 1; fy < FIELD_H - 1; fy++) {
//     //         std::cout << "  ";  // Пустое пространство слева
//     //         for (int fx = 1; fx < FIELD_W - 1; fx++) {
//     //             std::cout << "..";  // Пустое пространство внутри
//     //         }
//     //         std::cout << "  " << std::endl;
//     //     }
//     //
//     //     std::cout << "<!";
//     //     for (int fx = 1; fx < FIELD_W - 1; fx++) {
//     //         std::cout << "==";  // Рисуем землю
//     //     }
//     //     std::cout << "!>" << std::endl;
//     // }
//
//     void draw_field(ScreenBuffer& buf, int ox, int oy) {
//         // рисуем поле + рамку
//         for (int y = 0; y < FIELD_H; ++y) {
//             for (int x = 0; x < FIELD_W; ++x) {
//                 int sx = ox + x * 2;  // если хочешь две клетки на символ
//                 int sy = oy + y;
//
//                 // Рисуем вертикальные стенки
//                 if (x == 0 || x == FIELD_W - 1) {
//                     buf.set(sx, sy, '|');
//                 }
//                 // Рисуем нижнюю границу
//                 else if (y == FIELD_H - 1) {  // Используем FIELD_H - 1 для нижней границы
//                     buf.set(sx, sy, '=');
//                 }
//                 // Заполненные клетки поля
//                 else if (field[y][x]) {
//                     buf.set(sx, sy, '#');
//                 }
//                 // Пустые клетки поля
//                 else {
//                     buf.set(sx, sy, '.');
//                 }
//             }
//         }
//
//         // Рисуем тетромино
//         for (int ty = 0; ty < 4; ++ty) {
//             for (int tx = 0; tx < 4; ++tx) {
//                 if (shapes[t.type][t.rotation][ty][tx]) {
//                     int world_x = t.x + tx;
//                     int world_y = t.y + ty;
//
//                     // Проверка, чтобы тетромино не выходило за пределы поля
//                     if (world_x >= 0 && world_x < FIELD_W && world_y >= 0 && world_y < FIELD_H) {
//                         buf.set(ox + world_x * 2, oy + world_y, '#');
//                     }
//                 }
//             }
//         }
//     }
//
//     void draw_score() {
//         std::cout << "score: " << score;
//         std::cout << "\n";
//     };
//
//     // Функция для отключения отображения символов при вводе
//     void disableEcho() {
//         struct termios settings;
//         tcgetattr(STDIN_FILENO, &settings);
//         settings.c_lflag &= ~ECHO;  // Отключаем отображение символов
//         tcsetattr(STDIN_FILENO, TCSANOW, &settings);
//     }
//
//     // Функция для включения отображения символов
//     void enableEcho() {
//         struct termios settings;
//         tcgetattr(STDIN_FILENO, &settings);
//         settings.c_lflag |= ECHO;  // Включаем отображение символов
//         tcsetattr(STDIN_FILENO, TCSANOW, &settings);
//     }
//
//     struct TerminalState {
//         termios settings;
//     };
//
//     // Сохраняем состояние терминала
//     TerminalState saveTerminal() {
//         TerminalState state;
//         tcgetattr(STDIN_FILENO, &state.settings);
//         return state;
//     }
//     // Восстанавливаем состояние терминала
//     void restoreTerminal(const TerminalState& state) {
//         tcsetattr(STDIN_FILENO, TCSANOW, &state.settings);
//     }
//
//     void clear_screen() { std::cout << "\033[H\033[2J"; };
//
//     void sleep_ms(int ms) { std::this_thread::sleep_for(std::chrono::milliseconds(ms)); };
//
//     // void draw() {
//     //     clear_screen();
//     //     sleep_ms(50);  // must be a int variable i need to investigate that. NOTE that is
//     //     temporay
//     //                    // version because is can get faster or slower by different cpu.
//     //
//     //
//     //                    // Конструктор DoubleBuffer, передаем параметры ширины и высоты
//     //     DoubleBuffer doubleBuffer(FIELD_W * 2, FIELD_H);  // Умножаем FIELD_W на 2 для
//     удлинения
//     //     клетки draw_field(doubleBuffer.back, 2, 2);
//     //
//     //     doubleBuffer.swap();
//     //   //  draw_score();
//     //   //  draw_frame();
//     // };
//
//     void draw(DoubleBuffer& doubleBuffer) {
//         clear_screen();
//         sleep_ms(50);  // Временная задержка
//
//         // Рисуем на back-буфере
//         draw_field(doubleBuffer.back, 0, 0);  // rewrite it
//
//         // Переключаем back и front буферы
//         doubleBuffer.swap();
//
//         // Отображаем front буфер на экране (это то, что видит пользователь)
//         const ScreenBuffer& front = doubleBuffer.display();
//         for (int y = 0; y < FIELD_H; ++y) {
//             for (int x = 0; x < FIELD_W * 2; ++x) {  // умножаем на 2 для удлиненной клетки
//                 std::cout << front.get(x, y);
//             }
//             std::cout << "\n";
//         }
//     }
// };
//
// // int main() {
// //     Render render;
// //     Input input;
// //     Game game;
// //
// //     // Сохраняем старое состояние терминала
// //     Render::TerminalState oldState = render.saveTerminal();
// //
// //     // entering alt buffer
// //     std::cout << "\033[?1049h";
// //     // hide cursor
// //     std::cout << "\033[?25l";
// //     render.disableEcho();
// //
// //     while (game_is_running == true) {
// //         input.handle_input();
// //         game.update();
// //         render.draw();
// //     };
// //
// //     render.restoreTerminal(oldState);
// //     // exeting alt buffer
// //     std::cout << "\033[?1049l";
// //     // show cursor again
// //     std::cout << "\033[?25h";
// //     // enableEcho();
// //
// //     return 0;
// // };
// //
//
int main() {
    // Render render;
    // Input input;
    // Game game;
    //
    // // Сохраняем старое состояние терминала
    // Render::TerminalState oldState = render.saveTerminal();
    //
    // // entering alt buffer
    // std::cout << "\033[?1049h";
    // // hide cursor
    // std::cout << "\033[?25l";
    // render.disableEcho();
    //
    // // Создаем объект DoubleBuffer один раз
    // DoubleBuffer doubleBuffer(FIELD_W * 2, FIELD_H);  // Умножаем FIELD_W на 2 для удлинения
    // клетки
    //
    // while (game_is_running) {
    //     input.handle_input();
    //     game.update();
    //
    //     // Используем doubleBuffer для рисования
    //     render.draw(doubleBuffer);  // передаем doubleBuffer в draw
    // };
    //
    // render.restoreTerminal(oldState);
    // // exiting alt buffer
    // std::cout << "\033[?1049l";
    // // show cursor again
    // std::cout << "\033[?25h";
    // enableEcho();

    return 0;
};

/*
 что еще написать:
управление сбоку
счетчик очков
 */
