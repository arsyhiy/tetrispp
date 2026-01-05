#include "Game.hpp"

bool Game::check_collision(const Tetromino& t) {
    for (int ty = 0; ty < 4; ++ty) {
        for (int tx = 0; tx < 4; ++tx) {
            if (shapes[t.type][t.rotation][ty][tx]) {
                int world_x = t.x + tx;
                int world_y = t.y + ty + 1;
                if (world_y >= FIELD_H - 1 ||
                    (world_x >= 1 && world_x < FIELD_W - 1 && field[world_y][world_x])) {
                    return false;
                    // TL;DR it check if y greater or equal  than field_h or if x is within valid
                    // bounds but the cell is occupied; if either is true, it returns false
                }
            }
        }
    }
    return true;
};

void Game::lock_tetromino(const Tetromino& t) {
    for (int ty = 0; ty < 4; ++ty) {
        for (int tx = 0; tx < 4; ++tx) {
            if (shapes[t.type][t.rotation][ty][tx]) {
                int world_x = t.x + tx;
                int world_y = t.y + ty;
                if (world_y < FIELD_H - 1) {
                    field[world_y][world_x] = 1;
                }
            }
        }
    }

    clean_line();
};

void Game::move_down(Tetromino& t) {
    if (check_collision(t)) {
        t.y++;
    } else {
        lock_tetromino(t);
        // if we can move down lock in
        t.y = -1;
    }
};

void Game::move_left(Tetromino& t) {
    for (int ty = 0; ty < 4; ++ty) {
        for (int tx = 0; tx < 4; ++tx) {
            if (shapes[t.type][t.rotation][ty][tx]) {
                int world_x = t.x + tx - 1;
                int world_y = t.y + ty;

                if (world_x < 1 || (world_y >= 0 && world_y < FIELD_H && field[world_y][world_x])) {
                    return;
                    // TL;DR Exit the function if out of bounds or if the cell is occupied
                }
            }
        }
    }

    t.x--;
};

void Game::move_right(Tetromino& t) {
    for (int ty = 0; ty < 4; ++ty) {
        for (int tx = 0; tx < 4; ++tx) {
            if (shapes[t.type][t.rotation][ty][tx]) {
                int world_x = t.x + tx + 1;
                int world_y = t.y + ty;

                if (world_x >= FIELD_W - 1 ||
                    (world_y >= 0 && world_y < FIELD_H && field[world_y][world_x])) {
                    return;
                    // TL;DR Exit the function if out of bounds or if the cell is occupied
                }
            }
        }
    }

    t.x++;
};

void Game::rotate(Tetromino& t) {
    int new_rotation = (t.rotation + 1) % 4;

    for (int ty = 0; ty < 4; ++ty) {
        for (int tx = 0; tx < 4; ++tx) {
            if (shapes[t.type][new_rotation][ty][tx]) {
                int world_x = t.x + tx;
                int world_y = t.y + ty;

                if (world_x < 0 || world_x >= FIELD_W || world_y >= FIELD_H ||
                    field[world_y][world_x]) {
                    return;
                    // TL;DR Exit the function if out of bounds or if the cell is occupied
                }
            }
        }
    }

    t.rotation = new_rotation;
};

bool Game::line_is_full(int row) {
    for (int b = 1; b < FIELD_W - 1; ++b) {
        if (field[row][b] == 0) {
            return false;
        }
    }
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
};

void Game::clean_line() {
    for (int i = FIELD_H - 1; i >= 1; --i) {
        if (line_is_full(i)) {
            clear_line(i);
            score = score + t.points;
            i++;
        }
    }
};

Game::Tetromino Game::create_random_tetromino() {
    Tetromino t;
    t.type = rand() % 7;

    t.rotation = 0;
    t.x = FIELD_W / 2 - 2;
    t.y = 0;

    switch (t.type) {
        case 0:  // I-образное тетромино
            t.points = 100;
            break;
        case 1:  // J-образное тетромино
            t.points = 200;
            break;
        case 2:  // L-образное тетромино
            t.points = 300;
            break;
        case 3:  // O-образное тетромино
            t.points = 150;
            break;
        case 4:  // S-образное тетромино
            t.points = 250;
            break;
        case 5:  // T-образное тетромино
            t.points = 200;
            break;
        case 6:  // Z-образное тетромино
            t.points = 300;
            break;
    }

    return t;
};

void Game::spawn_new_tetromino() { t = create_random_tetromino(); };

bool Game::is_game_over() {
    for (int x = 0; x < FIELD_W; ++x) {
        if (field[0][x] != 0) {
            return true;
        }
    }
    return false;
};

void Game::update() {
    static int tick = 0;        // fps meter
    const int FALL_DELAY = 30;  // Tetromino fall frequency (number of cycles between down steps)

    if (is_game_over()) {
        is_running = false;
        std::cout << "Game Over!\n";
        return;
    }

    tick++;
    if (tick >= FALL_DELAY) {  // Move the tetromino down every FALL_DELAY cycles
        tick = 0;

        // If tetromino can move down
        if (check_collision(t)) {
            t.y++;
        } else {
            // It can't - we fix and clean the lines
            lock_tetromino(t);

            // After fixing, we create a new tetromino
            spawn_new_tetromino();

            // We immediately check the game over for the new tetromino
            if (is_game_over()) {
                is_running = false;
                std::cout << "Game Over!\n";
                return;
            }
        }
    }
}
