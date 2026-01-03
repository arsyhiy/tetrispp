#pragma once
#include <thread>
#include <chrono>
#include "stucture.hpp"
#include <iostream>

class Render{


    public:
        void draw_frame();
        void draw_field(ScreenBuffer& buf, int ox, int oy);
        void draw_score();
        void disableEcho();
        void enableEcho();

        TerminalState saveTerminal();
        void restoreTerminal(const TerminalState& state);
        
        void clear_screen() { std::cout << "\033[H\033[2J"; };
        void sleep_ms(int ms) { std::this_thread::sleep_for(std::chrono::milliseconds(ms)); };
        void draw(DoubleBuffer& doubleBuffer);
};
