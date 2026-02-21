#pragma once
#include "iostream"
#include <chrono>
#include <thread>
#include "ncurses.h"
#include <random>
#include <algorithm>
class Game {
   public:
    bool is_running = true;

    // field
    static const int FIELD_H = 21;      // hight
    static const int FIELD_W = 12;      // width
    int field[FIELD_H][FIELD_W] = {0};  // note that 0 for empty 1 for full.

    struct Tetromino {
        int type;
        int rotation;
        int x;
        int y;
        int points;
    };
    Tetromino t;

static constexpr int shapes[7][4][4][4] = {
    // 0: I (палка)
    {
        {{0,0,0,0}, {1,1,1,1}, {0,0,0,0}, {0,0,0,0}},  // горизонтально
        {{0,1,0,0}, {0,1,0,0}, {0,1,0,0}, {0,1,0,0}},  // вертикально
        {{0,0,0,0}, {1,1,1,1}, {0,0,0,0}, {0,0,0,0}},  // горизонтально
        {{0,1,0,0}, {0,1,0,0}, {0,1,0,0}, {0,1,0,0}}   // вертикально
    },

    // 1: J
    {
        {{1,0,0,0}, {1,1,1,0}, {0,0,0,0}, {0,0,0,0}},  //   ■
                                                        // ■■■
        {{0,1,1,0}, {0,1,0,0}, {0,1,0,0}, {0,0,0,0}},  // ■■
                                                        // ■
                                                        // ■
        {{0,0,0,0}, {1,1,1,0}, {0,0,1,0}, {0,0,0,0}},  // ■■■
                                                        //   ■
        {{0,0,1,0}, {0,0,1,0}, {0,1,1,0}, {0,0,0,0}}   //   ■
                                                        //   ■
                                                        // ■■
    },

    // 2: L
    {
        {{0,0,1,0}, {1,1,1,0}, {0,0,0,0}, {0,0,0,0}},  //     ■
                                                        // ■■■
        {{0,1,0,0}, {0,1,0,0}, {0,1,1,0}, {0,0,0,0}},  // ■
                                                        // ■
                                                        // ■■
        {{0,0,0,0}, {1,1,1,0}, {1,0,0,0}, {0,0,0,0}},  // ■■■
                                                        // ■
        {{1,1,0,0}, {0,1,0,0}, {0,1,0,0}, {0,0,0,0}}   // ■■
                                                        //  ■
                                                        //  ■
    },

    // 3: O (квадрат) — все повороты одинаковые
    {
        {{0,0,0,0}, {0,1,1,0}, {0,1,1,0}, {0,0,0,0}},
        {{0,0,0,0}, {0,1,1,0}, {0,1,1,0}, {0,0,0,0}},
        {{0,0,0,0}, {0,1,1,0}, {0,1,1,0}, {0,0,0,0}},
        {{0,0,0,0}, {0,1,1,0}, {0,1,1,0}, {0,0,0,0}}
    },

    // 4: S
    {
        {{0,1,1,0}, {1,1,0,0}, {0,0,0,0}, {0,0,0,0}},  //  ■■
                                                        // ■■
        {{0,1,0,0}, {0,1,1,0}, {0,0,1,0}, {0,0,0,0}},  //  ■
                                                        //  ■■
                                                        //   ■
        {{0,0,0,0}, {0,1,1,0}, {1,1,0,0}, {0,0,0,0}},  //  ■■
                                                        // ■■
        {{1,0,0,0}, {1,1,0,0}, {0,1,0,0}, {0,0,0,0}}   // ■
                                                        // ■■
                                                        //  ■
    },

    // 5: T
    {
        {{0,1,0,0}, {1,1,1,0}, {0,0,0,0}, {0,0,0,0}},  //  ■
                                                        // ■■■
        {{0,1,0,0}, {0,1,1,0}, {0,1,0,0}, {0,0,0,0}},  //  ■
                                                        //  ■■
                                                        //  ■
        {{0,0,0,0}, {1,1,1,0}, {0,1,0,0}, {0,0,0,0}},  // ■■■
                                                        //  ■
        {{0,1,0,0}, {1,1,0,0}, {0,1,0,0}, {0,0,0,0}}   //  ■
                                                        // ■■
                                                        //  ■
    },

    // 6: Z
    {
        {{1,1,0,0}, {0,1,1,0}, {0,0,0,0}, {0,0,0,0}},  // ■■
                                                        //  ■■
        {{0,0,1,0}, {0,1,1,0}, {0,1,0,0}, {0,0,0,0}},  //   ■
                                                        //  ■■
                                                        //  ■
        {{0,0,0,0}, {1,1,0,0}, {0,1,1,0}, {0,0,0,0}},  // ■■
                                                        //  ■■
        {{0,1,0,0}, {1,1,0,0}, {1,0,0,0}, {0,0,0,0}}   //  ■
                                                        // ■■
                                                        // ■
    }
};
    float soft_drop_speed = 0.0f;
    bool manual_drop_requested = false;
    int score = 0;

    int entry_delay_counter = 0;

    float current_gravity = 1.0f;

    float fall_timer = 0.0f;

    bool soft_drop_active = false;

    int lock_height = 0;
                                                      
    bool check_collision(const Tetromino& t);
    bool line_is_full(int row);
    bool is_game_over();

    Tetromino create_random_tetromino();

    void move_down(Tetromino& t);
    void move_left(Tetromino& t);
    void move_right(Tetromino& t);
    //void rotate(Tetromino& t);

    bool is_valid_position(const Tetromino& tet) const;
    bool rotate(Tetromino& t);

    void update(float delta_time);
    // void clean_line();
    int  clean_line();
    void clear_line(int row);
    void spawn_new_tetromino();
    void lock_tetromino(const Tetromino& t);
    int calculate_nes_are(int lock_height);

    void init_field();

    void finalize_lock();

private:
    void refill_bag();

    std::vector<int> bag;
};
