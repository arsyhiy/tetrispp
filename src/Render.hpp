#include "Game.hpp"
#include "ncurses.h"

class Render {
   public:
  void draw_frame(const Game& game);
    // void draw_field(ScreenBuffer& buf, int ox, int oy);

    // void draw_score();


    // interesting how refresh is working in ncures
    // void clear_screen() { std::cout << "\033[H\033[2J"; };
    void sleep_ms(int ms) {
      std::this_thread::sleep_for(std::chrono::milliseconds(ms)); };
    void draw(const Game& game);
};
