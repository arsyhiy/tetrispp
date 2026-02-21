#include "Game.hpp"

void Game::init_field() {
    // оказалось что это является хорошей идеей для rotate заполить края и пол значение 2
    for (int y = 0; y < FIELD_H; ++y) {
        field[y][0] = 2;
        field[y][FIELD_W - 1] = 2;
    }
    for (int x = 0; x < FIELD_W; ++x) {
        field[FIELD_H - 1][x] = 2;
    }
}


void Game::finalize_lock() {
    lock_tetromino(t);
    int lines = clean_line();

    // Устанавливаем задержку перед появлением следующей фигуры (ARE)
    entry_delay_counter = calculate_nes_are(t.y) + ((lines > 0) ? 17 + lines * 3 : 0);
    
    // Сбрасываем таймер падения, чтобы новая фигура не прыгнула вниз сразу
    fall_timer = 0;
    
    // Помечаем текущую фигуру как "неактивную" (можно через y = -1 или спец. флаг)
    t.y = -1; 
}



int Game::calculate_nes_are(int lock_height) {
    // lock_height — самая нижняя строка, которую заняла фигура (чем больше — тем ниже)
    // В NES ARE уменьшается с ростом высоты фиксации

    if (lock_height >= 16)
        return 10;  // самые нижние позиции
    if (lock_height >= 12)
        return 12;
    if (lock_height >= 8)
        return 14;
    if (lock_height >= 4)
        return 16;
    return 18;  // самые верхние (lock_height ≈ 0–3)
}



bool Game::check_collision(const Tetromino& t) {

    for (int ty = 0; ty < 4; ++ty) {
        for (int tx = 0; tx < 4; ++tx) {
            if (shapes[t.type][t.rotation][ty][tx]) {
                int world_x = t.x + tx;
                int world_y = t.y + ty; //+ 1;
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

    //clean_line();
};

// void Game::move_down(Tetromino& t) {
//     if (check_collision(t)) {
//         t.y++;
//     } else {
//         lock_tetromino(t);
//         // if we can move down lock in
//         t.y = -1;
//     }
// };

void Game::move_down(Tetromino& t) {
    // Если мы уже в режиме паузы (ARE), игнорируем ввод
    if (entry_delay_counter > 0) return;

    Tetromino temp = t;
    temp.y++;

    if (check_collision(temp)) {
        t.y++;
    } else {
        finalize_lock(); // Используем общий метод
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

bool Game::is_valid_position(const Tetromino& tet) const {
    for (int ty = 0; ty < 4; ++ty) {
        for (int tx = 0; tx < 4; ++tx) {
            if (shapes[tet.type][tet.rotation][ty][tx]) {
                int wx = tet.x + tx;
                int wy = tet.y + ty;

                // Проверяем ВСЕ границы и занятость
                if (wx < 0 || wx >= FIELD_W || wy < 0 || wy >= FIELD_H || field[wy][wx] != 0) {
                    return false;
                }
            }
        }
    }
    return true;
}

bool Game::rotate(Tetromino& t) {
    int new_rotation = (t.rotation + 1) % 4;

    Tetromino original = t;  // копия на случай отката

    // Пробуем базовый поворот (без сдвига)
    t.rotation = new_rotation;

    if (is_valid_position(t)) {
        return true;  // успех без сдвига
    }

    // Таблица простых kick-отскоков (пробуем по порядку)
    // Порядок: влево, вправо, вверх, вниз (можно добавить больше)
    static const std::pair<int, int> kicks[] = {
        {-1, 0},  // влево
        {1, 0},   // вправо
        {0, -1},  // вверх
        {0, 1},   // вниз
        {-2, 0},  // влево на 2 (для I-фигуры полезно)
        {2, 0}    // вправо на 2
    };

    for (const auto& kick : kicks) {
        Tetromino kicked = t;
        kicked.x += kick.first;
        kicked.y += kick.second;

        if (is_valid_position(kicked)) {
            t = kicked;  // применяем сдвиг + поворот
            return true;
        }
    }

    // Ничего не подошло — откатываем
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

    for (int i = FIELD_H - 2; i >= 1; --i) {  // начинаем с предпоследней, пол не трогаем
        if (line_is_full(i)) {
            clear_line(i);  // сдвигаем строки вниз, очищаем i-ю
            cleared++;

            // начисление очков — пример классической системы (можно подогнать)
            int points_per_line[] = {0, 40, 100, 300, 1200};  // 1..4 линии
            score += points_per_line[cleared] * (1 + 1);      // × (level+1)

            // НЕ делаем i++ здесь — после сдвига проверяем ту же строку заново
            // цикл for сам сделает --i → вернётся на i
        }
    }

    return cleared;  // возвращаем, сколько линий очистили за этот lock
}

namespace {
    std::mt19937& global_rng() {
        static std::mt19937 rng(std::random_device{}());
        return rng;
    }

    constexpr int points_table[7] = {
        100, // I
        200, // J
        300, // L
        150, // O
        250, // S
        200, // T
        300  // Z
    };
}

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

void Game::spawn_new_tetromino() {
    t = create_random_tetromino();

}

bool Game::is_game_over() {
    for (int x = 0; x < FIELD_W; ++x) {
        if (field[0][x] != 0 && field[0][x] != 2)  {
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

    // ===============================
    // Entry delay (ARE delay)
    // ===============================
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

    // ===============================
    // Gravity timer
    // ===============================
    fall_timer += delta_time;

    float gravity_interval = 1.0f / current_gravity;

    // Soft drop acceleration
    if (soft_drop_active) {
        gravity_interval = std::min(gravity_interval, 0.05f);
    }

    // ===============================
    // Movement loop (important for low FPS)
    // ===============================
    while (fall_timer >= gravity_interval) {
        fall_timer -= gravity_interval;

        Tetromino temp = t;
        temp.y++;

        if (check_collision(temp)) {
            t.y++;
        }else {
          t.y++;
          finalize_lock(); // Используем тот же общий метод  
    //lock_tetromino(t);

    int lines = clean_line();

    entry_delay_counter =
        calculate_nes_are(t.y)
        + ((lines > 0) ? 17 + lines * 3 : 0);

    return;   // важно выйти, чтобы не продолжать цикл
}
    }
}
