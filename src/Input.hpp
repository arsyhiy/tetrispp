#pragma once

#include <fcntl.h>
#include <termios.h>
#include <unistd.h>
#include "Game.hpp"

class Input {
   public:

    bool kbhit();
    void handle_input(Game& game);
};
