#include "Game.hpp"

void Game::init_field() {
    for (int y = 0; y < FIELD_H; ++y) {
        for (int x = 0; x < FIELD_W; ++x) {
            field[y][x] = 0;

            if (x == 0 || x == FIELD_W - 1 || y == FIELD_H - 1) {
                field[y][x] = 2;
            }
        }
    }
}

void Game::finalize_lock() {
    lock_tetromino(t);

    last_cleared_lines = clean_line();
    total_lines_cleared += last_cleared_lines;

    int lines = last_cleared_lines;

    level = total_lines_cleared / 10;
    current_gravity = base_gravity + level * 0.5f;

    entry_delay_counter = calculate_nes_are(t.y) + ((lines > 0) ? 17 + lines * 3 : 0);

    fall_timer = 0;

    t.y = -1;
}

int Game::calculate_nes_are(int lock_height) {
    if (lock_height >= 16)
        return 10;
    if (lock_height >= 12)
        return 12;
    if (lock_height >= 8)
        return 14;
    if (lock_height >= 4)
        return 16;
    return 18;
}

bool Game::check_collision(const Tetromino& t) {
    for (int ty = 0; ty < 4; ++ty) {
        for (int tx = 0; tx < 4; ++tx) {
            if (shapes[t.type][t.rotation][ty][tx]) {
                int world_x = t.x + tx;
                int world_y = t.y + ty;
                if (world_y >= FIELD_H - 1 ||
                    (world_x >= 1 && world_x < FIELD_W - 1 && field[world_y][world_x])) {
                    return false;
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

                if (world_x > 0 && world_x < FIELD_W - 1 && world_y < FIELD_H - 1) {
                    field[world_y][world_x] = 1;
                }
            }
        }
    }
}

void Game::move_down(Tetromino& t) {
    if (entry_delay_counter > 0)
        return;

    Tetromino temp = t;
    temp.y++;

    if (check_collision(temp)) {
        t.y++;
    } else {
        finalize_lock();
    }
}

void Game::move_left(Tetromino& t) {
    for (int ty = 0; ty < 4; ++ty) {
        for (int tx = 0; tx < 4; ++tx) {
            if (shapes[t.type][t.rotation][ty][tx]) {
                int world_x = t.x + tx - 1;
                int world_y = t.y + ty;

                if (world_x < 1 || (world_y >= 0 && world_y < FIELD_H && field[world_y][world_x])) {
                    return;
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
                }
            }
        }
    }

    t.x++;
};

bool Game::is_valid_position(const Tetromino& tet) const {
    for (int ty = 0; ty < 4; ++ty) {
        for (int tx = 0; tx < 4; ++tx) {
            if (shapes[tet.type][tet.rotation][ty][tx]) {
                int wx = tet.x + tx;
                int wy = tet.y + ty;

                if (wx <= 0 || wx >= FIELD_W - 1 || wy >= FIELD_H - 1) {
                    return false;
                }
            }
        }
    }
    return true;
}

bool Game::rotate(Tetromino& t) {
    int new_rotation = (t.rotation + 1) % 4;

    Tetromino original = t;

    t.rotation = new_rotation;

    if (is_valid_position(t)) {
        return true;
    }

    static const std::pair<int, int> kicks[] = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}, {-2, 0}, {2, 0}};

    for (const auto& kick : kicks) {
        Tetromino kicked = t;
        kicked.x += kick.first;
        kicked.y += kick.second;

        if (is_valid_position(kicked)) {
            t = kicked;
            return true;
        }
    }

    t = original;
    return false;
}

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

int Game::clean_line() {
    int cleared = 0;

    for (int i = FIELD_H - 2; i >= 1; --i) {
        if (line_is_full(i)) {
            clear_line(i);
            cleared++;
            i++;
        }
    }

    static const int score_table[5] = {0, 100, 300, 500, 800};

    score += score_table[std::min(cleared, 4)];

    return cleared;
}

static std::mt19937 rng(std::random_device{}());

std::mt19937& global_rng() { return rng; }

static const int points_table[7] = {
    100,  // I
    200,  // J
    300,  // L
    150,  // O
    250,  // S
    200,  // T
    300   // Z
};

void Game::refill_bag() {
    bag = {0, 1, 2, 3, 4, 5, 6};
    std::shuffle(bag.begin(), bag.end(), global_rng());
}

Game::Tetromino Game::create_random_tetromino() {
    if (bag.empty()) {
        refill_bag();
    }

    int type = bag.back();
    bag.pop_back();

    Tetromino t{};
    t.type = type;
    t.rotation = 0;
    t.x = FIELD_W / 2 - 2;
    t.y = 0;
    t.points = points_table[type];

    return t;
}

void Game::spawn_new_tetromino() { t = create_random_tetromino(); }

bool Game::is_game_over() {
    for (int x = 0; x < FIELD_W; ++x) {
        if (field[0][x] != 0 && field[0][x] != 2) {
            return true;
        }
    }
    return false;
};

void Game::update(float delta_time) {
    if (is_game_over()) {
        is_running = false;
        return;
    }

    if (entry_delay_counter > 0) {
        entry_delay_counter--;

        if (entry_delay_counter == 0) {
            flushinp();
            spawn_new_tetromino();

            if (is_game_over()) {
                is_running = false;
                return;
            }
        }

        return;
    }

    fall_timer += delta_time;

    float gravity_interval = 1.0f / current_gravity;

    if (soft_drop_active) {
        gravity_interval = std::min(gravity_interval, 0.05f);
    }

    while (fall_timer >= gravity_interval) {
        fall_timer -= gravity_interval;

        Tetromino temp = t;
        temp.y++;

        if (check_collision(temp)) {
            t.y++;
        } else {
            finalize_lock();

            current_gravity = base_gravity + level * 0.5f;

            int lock_height = t.y;

            return;
        }
    }
}
