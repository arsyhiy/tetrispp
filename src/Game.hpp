#include "stucture.hpp"


class Game{

    public:
        void move_down(Tetromino& t); 
        void move_left(Tetromino& t);
        void move_right(Tetromino& t);
        void rotate(Tetromino& t);
        void update();
}; 
