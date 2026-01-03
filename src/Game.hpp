#include "stucture.hpp"

extern Tetromino t;

class Game{

    public:
        void move_down(Tetromino& t);
        void move_left(Tetromino& t);
        void move_right(Tetromino& t);
        void rotate(Tetromino& t);
        void  update();
        void clean_line();
        bool line_is_full(int row);
        void clear_line(int row);
};
