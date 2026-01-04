/*
  TODO: переработать так что бы работать с  doublebuffer.
*/

#include "Game.hpp"

bool Game::can_move_down(const Tetromino& t) {
    // Let's check if we can move the tetromino down
    for (int ty = 0; ty < 4; ++ty) {
        for (int tx = 0; tx < 4; ++tx) {
            // If the tetromino cell is active
            if (shapes[t.type][t.rotation][ty][tx]) {
                int world_x = t.x + tx;
                int world_y = t.y + ty + 1;  // We check 1 cell below
                                             //
                // Проверяем, не выходит ли за пределы поля или не сталкивается с другим блоком
                if (world_y >= FIELD_H - 1 ||
                    (world_x >= 1 && world_x < FIELD_W - 1 && field[world_y][world_x])) {
                    return false;  // Невозможно двигаться вниз, либо достигли дна, либо
                    // столкнулись с блоком
                }
            }
        }
    }
    return true;  // Тетромино можно двигать вниз
};

void Game::move_down(Tetromino& t) {
    // Let's check if we can move the tetromino down
    if (can_move_down(t)) {
        t.y++;  // If we can, we move the tetromino down one square.
    } else {
        // If we can't, we fix the tetromino in the field
        // maybe is a good idea move to another Function called lock_in or something likethat
        for (int ty = 0; ty < 4; ++ty) {
            for (int tx = 0; tx < 4; ++tx) {
                if (shapes[t.type][t.rotation][ty][tx]) {
                    int world_x = t.x + tx;
                    int world_y = t.y + ty;
                    if (world_y < FIELD_H - 1) {  // in fact FIELD_H is 1 hight smaller that it
                                                  // execpted but there is differents if we will
                                                  // write FIELD_H -1 or without - 1. but i will
                                                  // write with -1 tho avoid unexpected   consequences
                        field[world_y][world_x] = 1;  // We fix the block on the field
                    }
                }
            }
        }
        // Tetromino is no longer moving, you can launch the next one
        t.y =
            -1;  // We set a special value to signal that the tetromino is locked. NOTE: check that
                 // thing because i don't see  a special value where it's commin because i dumb
    }
};

// Функция для перемещения тетромино влево/вправ
void Game::move_left(Tetromino& t) {
    // Проверяем, можем ли мы переместиться влево
    for (int ty = 0; ty < 4; ++ty) {
        for (int tx = 0; tx < 4; ++tx) {
            if (shapes[t.type][t.rotation][ty][tx]) {
                int world_x = t.x + tx - 1;
                int world_y = t.y + ty;

                if (world_x < 1 || (world_y >= 0 && world_y < FIELD_H &&
                field[world_y][world_x])) {
                    return;  // Не можем двигаться влево
                }
            }
        }
    }

    t.x--;  // Перемещаем влево
};

// Функция для перемещения тетромино вправо
void Game::move_right(Tetromino& t) {
    // We check if it goes out of bounds or collides with another block.
    for (int ty = 0; ty < 4; ++ty) {
        for (int tx = 0; tx < 4; ++tx) {
            if (shapes[t.type][t.rotation][ty][tx]) {
                int world_x = t.x + tx + 1;
                int world_y = t.y + ty;

                if (world_x >= FIELD_W - 1 ||
                    (world_y >= 0 && world_y < FIELD_H && field[world_y][world_x])) {
                    return;  // We can't move to the right
                }
            }
        }
    }

    t.x++;  // Move to the right
};

// Function for rotating a tetromino
void Game::rotate(Tetromino& t) {
    int new_rotation = (t.rotation + 1) % 4;
    // Let's check if we can rotate the tetromino

    for (int ty = 0; ty < 4; ++ty) {
        for (int tx = 0; tx < 4; ++tx) {
            if (shapes[t.type][new_rotation][ty][tx]) {
                int world_x = t.x + tx;
                int world_y = t.y + ty;

                if (world_x < 0 || world_x >= FIELD_W || world_y >= FIELD_H ||
                    field[world_y][world_x]) {
                    return;  // We can't turn
                }
            }
        }
    }

    t.rotation = new_rotation;  // We are making a turn
};

bool Game::line_is_full(int row) {
    for (int b = 1; b < FIELD_W - 1; ++b) {
        if (field[row][b] == 0) {
            return false;
        };
    };
    return true;
};

void Game::clear_line(int row) {
    for (int i = row; i > 0; --i) {
        for (int j = 0; j < FIELD_W; ++j) {
            if (i < FIELD_H - 1) {
                field[i][j] = field[i - 1][j];
            }
        }
    }

    for (int j = 0; j < FIELD_W; ++j) {
        field[0][j] = 0;
    }
}

void Game::clean_line() {
    for (int i = FIELD_H - 1; i >= 1; --i) {
        if (line_is_full(i)) {
            clear_line(i);
            score = score + 100;  // rewrite to tetromino score point
            i++;
        }
    }
};







void Game::update() {
    // we need colishion
    move_down(t);  // i think move_down most be a update_game Function.
    clean_line();
};

