#pragma once

#include <fcntl.h>
#include <termios.h>
#include <unistd.h>
#include "Game.hpp"

class Input {
   public:
   void handle_input(Game& game);
};
