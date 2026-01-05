#include "Game.hpp"

class Render {
public:

    void draw(const Game& game);
    void draw_interface(const Game& game);
    void draw_frame(const Game& game);
    void sleep_ms(int ms) {
    std::this_thread::sleep_for(std::chrono::milliseconds(ms)); };
};
