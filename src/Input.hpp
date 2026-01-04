#pragma once

#include "Game.hpp"
#include "ncurses.h"
class Input {
   public:

    bool kbhit();
    void handle_input(Game& game);
};
