#include "Render.hpp"

void Render::draw_interface(const Game& game) {
    mvprintw(1, 2, "Press 'esc' to quit");
    mvprintw(2, 2, "Level: %d", game.level);
    mvprintw(3, 2, "Score: %d", game.score);
    mvprintw(4, 2, "lines cleared: %d", game.total_lines_cleared);
};

void Render::draw_frame(const Game& game) {
    for (int y = 0; y < Game::FIELD_H; ++y) {
        for (int x = 0; x < Game::FIELD_W; ++x) {
            int screen_x = (x + Game::FIELD_W) * 2 + 5;
            int screen_y = y + 1;

            if (x == 0) {
                mvaddch(screen_y, screen_x, '<');
                mvaddch(screen_y, screen_x + 1, '!');

            } else if (x == Game::FIELD_W - 1) {
                mvaddch(screen_y, screen_x, '!');
                mvaddch(screen_y, screen_x + 1, '>');
            } else if (y == Game::FIELD_H - 1) {
                mvaddch(screen_y, screen_x, '#');
            } else {
                if (game.field[y][x]) {
                    mvaddch(screen_y, screen_x, '#');
                } else {
                    mvaddch(screen_y, screen_x, '.');
                }
            }
        }
    }

    for (int ty = 0; ty < 4; ++ty) {
        for (int tx = 0; tx < 4; ++tx) {
            if (Game::shapes[game.t.type][game.t.rotation][ty][tx]){
                int world_x = game.t.x + tx;
                int world_y = game.t.y + ty;

                if (world_x >= 0 && world_x < Game::FIELD_W && world_y >= 0 &&
                    world_y < Game::FIELD_H) {
                    mvaddch(world_y + 1, (world_x + Game::FIELD_W) * 2 + 5, '#');
                }
            }
        }
    }
}

void Render::draw(const Game& game) {
    refresh();
    draw_interface(game);
    draw_frame(game);
};
