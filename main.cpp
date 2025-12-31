#include <cstdio>
#include <iostream>
#include <unistd.h>
#include <cstdlib>
#include <thread>
#include <chrono>
#include <termios.h>
#include <fcntl.h>

// static variables

// field
static const int FIELD_H = 20; // hight
static const int FIELD_W = 10; // width
static int field[FIELD_H][FIELD_W]; // note that 0 for empty 1 for full.

// maybe i need a more pretty name for this
static bool game_is_running = true;

//structs

struct Tetromino {
  int type;
  int rotation;
  int x;
  int y;
};
Tetromino t = {0, 0, FIELD_W / 2, 0}; // make it random 

const int shapes[7][4][4][4] = {
    // I-тип
    {
        // Поворот 0
        { {0, 0, 0, 0}, {1, 1, 1, 1}, {0, 0, 0, 0}, {0, 0, 0, 0} },
        // Поворот 1
        { {0, 1, 0, 0}, {0, 1, 0, 0}, {0, 1, 0, 0}, {0, 1, 0, 0} },
        // Поворот 2
        { {0, 0, 0, 0}, {1, 1, 1, 1}, {0, 0, 0, 0}, {0, 0, 0, 0} },
        // Поворот 3
        { {0, 1, 0, 0}, {0, 1, 0, 0}, {0, 1, 0, 0}, {0, 1, 0, 0} }
    },
    // J-тип
    {
        { {0, 0, 0, 0}, {1, 1, 1, 0}, {1, 0, 0, 0}, {0, 0, 0, 0} }, // Поворот 0
        { {1, 0, 0, 0}, {1, 1, 1, 0}, {0, 0, 0, 0}, {0, 0, 0, 0} }, // Поворот 1
        { {0, 0, 0, 0}, {0, 0, 1, 1}, {1, 1, 1, 0}, {0, 0, 0, 0} }, // Поворот 2
        { {1, 0, 0, 0}, {1, 1, 1, 0}, {0, 0, 0, 0}, {0, 0, 0, 0} }  // Поворот 3
    },
    // L-тип
    {
        { {0, 0, 0, 0}, {0, 1, 1, 1}, {0, 0, 0, 1}, {0, 0, 0, 0} }, // Поворот 0
        { {0, 1, 0, 0}, {0, 1, 0, 0}, {0, 1, 1, 1}, {0, 0, 0, 0} }, // Поворот 1
        { {0, 0, 0, 0}, {1, 0, 0, 0}, {1, 1, 1, 0}, {0, 0, 0, 0} }, // Поворот 2
        { {1, 1, 0, 0}, {0, 1, 0, 0}, {0, 1, 1, 1}, {0, 0, 0, 0} }  // Поворот 3
    },
    // O-тип
    {
        { {0, 0, 0, 0}, {0, 1, 1, 0}, {0, 1, 1, 0}, {0, 0, 0, 0} }, // Поворот 0
        { {0, 0, 0, 0}, {0, 1, 1, 0}, {0, 1, 1, 0}, {0, 0, 0, 0} }, // Поворот 1
        { {0, 0, 0, 0}, {0, 1, 1, 0}, {0, 1, 1, 0}, {0, 0, 0, 0} }, // Поворот 2
        { {0, 0, 0, 0}, {0, 1, 1, 0}, {0, 1, 1, 0}, {0, 0, 0, 0} }  // Поворот 3
    },
    // S-тип
    {
        { {0, 0, 0, 0}, {0, 1, 1, 0}, {1, 1, 0, 0}, {0, 0, 0, 0} }, // Поворот 0
        { {1, 0, 0, 0}, {1, 1, 0, 0}, {0, 1, 0, 0}, {0, 0, 0, 0} }, // Поворот 1
        { {0, 0, 0, 0}, {0, 1, 1, 0}, {1, 1, 0, 0}, {0, 0, 0, 0} }, // Поворот 2
        { {1, 0, 0, 0}, {1, 1, 0, 0}, {0, 1, 0, 0}, {0, 0, 0, 0} }  // Поворот 3
    },
    // T-тип
    {
        { {0, 0, 0, 0}, {1, 1, 1, 0}, {0, 1, 0, 0}, {0, 0, 0, 0} }, // Поворот 0
        { {0, 1, 0, 0}, {1, 1, 1, 0}, {0, 1, 0, 0}, {0, 0, 0, 0} }, // Поворот 1
        { {0, 1, 0, 0}, {1, 1, 1, 0}, {0, 0, 0, 0}, {0, 0, 0, 0} }, // Поворот 2
        { {0, 1, 0, 0}, {1, 1, 1, 0}, {0, 1, 0, 0}, {0, 0, 0, 0} }  // Поворот 3
    },
    // Z-тип
    {
        { {0, 0, 0, 0}, {1, 1, 0, 0}, {0, 1, 1, 0}, {0, 0, 0, 0} }, // Поворот 0
        { {0, 1, 0, 0}, {1, 1, 0, 0}, {1, 0, 0, 0}, {0, 0, 0, 0} }, // Поворот 1
        { {0, 0, 0, 0}, {1, 1, 0, 0}, {0, 1, 1, 0}, {0, 0, 0, 0} }, // Поворот 2
        { {0, 1, 0, 0}, {1, 1, 0, 0}, {1, 0, 0, 0}, {0, 0, 0, 0} }  // Поворот 3
    }
};




// Функция для отключения отображения символов при вводе
void disableEcho() {
    struct termios settings;
    tcgetattr(STDIN_FILENO, &settings);
    settings.c_lflag &= ~ECHO;  // Отключаем отображение символов
    tcsetattr(STDIN_FILENO, TCSANOW, &settings);
}

// Функция для включения отображения символов
void enableEcho() {
    struct termios settings;
    tcgetattr(STDIN_FILENO, &settings);
    settings.c_lflag |= ECHO;  // Включаем отображение символов
    tcsetattr(STDIN_FILENO, TCSANOW, &settings);
}


struct TerminalState {
    termios settings;
};

// Сохраняем состояние терминала
TerminalState saveTerminal() {
    TerminalState state;
    tcgetattr(STDIN_FILENO, &state.settings);
    return state;
}
// Восстанавливаем состояние терминала
void restoreTerminal(const TerminalState &state) {
    tcsetattr(STDIN_FILENO, TCSANOW, &state.settings);
}

class Input{

public:

    bool kbhit() {

        struct termios settings;
        int ch, stdinFlags; // make sure input is a good naming
    
        tcgetattr(STDIN_FILENO, &settings);  // Getting the current terminal settings
        settings.c_lflag &= ~ICANON; //Disable canonical mode (input is available immediately, without waiting for Enter)
        settings.c_cc[VMIN] = 1; // Require at least 1 character to return from read// Require at least 1 character to return from read
        settings.c_cc[VTIME] = 0; // No timeout
        tcsetattr(STDIN_FILENO, TCSANOW, &settings); // setting new terminal settings
        stdinFlags = fcntl(STDIN_FILENO, F_GETFL, 0);
        fcntl(STDIN_FILENO, F_SETFL,  stdinFlags | O_NONBLOCK);
    
        ch = getchar();

        if(ch != EOF) {
            ungetc(ch, stdin);  // Return the character to the input stream
            return true;
        }
        return false;
    };

    bool can_move_down(const Tetromino& t) {

        // Let's check if we can move the tetromino down
        for (int ty = 0; ty < 4; ++ty) {
            for (int tx = 0; tx < 4; ++tx) {
                // If the tetromino cell is active
                if (shapes[t.type][t.rotation][ty][tx]) {
                    int world_x = t.x + tx;
                    int world_y = t.y + ty + 1;  // We check 1 cell below


                    // Проверяем, не выходит ли за пределы поля или не сталкивается с другим блоком
                    if (world_y >= FIELD_H || (world_x >= 0 && world_x < FIELD_W && field[world_y][world_x])) {
                        return false;  // Невозможно двигаться вниз, либо достигли дна, либо столкнулись с блоком
                    }
                }
            }
        }
        return true;  // Тетромино можно двигать вниз
    };

    // Функция для перемещения тетромино влево/вправ
    void move_left(Tetromino& t) {

        // Проверяем, можем ли мы переместиться влево
        for (int ty = 0; ty < 4; ++ty) {
            for (int tx = 0; tx < 4; ++tx) {
                if (shapes[t.type][t.rotation][ty][tx]) {
                    int world_x = t.x + tx - 1;
                    int world_y = t.y + ty;

                    if (world_x < 0 || (world_y >= 0 && world_y < FIELD_H && field[world_y][world_x])) {
                        return;  // Не можем двигаться влево
                    }
                }
            }
        }

        t.x--;  // Перемещаем влево
    };

    // Функция для перемещения тетромино вправо
    void move_right(Tetromino& t) {

        // We check if it goes out of bounds or collides with another block.
        for (int ty = 0; ty < 4; ++ty) {
            for (int tx = 0; tx < 4; ++tx) {
                if (shapes[t.type][t.rotation][ty][tx]) {
                    int world_x = t.x + tx + 1;
                    int world_y = t.y + ty;

                    if (world_x >= FIELD_W || (world_y >= 0 && world_y < FIELD_H && field[world_y][world_x])) {
                        return;  // We can't move to the right
                    }
                }
            }
        }

        t.x++;  // Move to the right
    };

    // Function for rotating a tetromino
    void rotate(Tetromino& t) {

        int new_rotation = (t.rotation + 1) % 4;
        // Let's check if we can rotate the tetromino

        for (int ty = 0; ty < 4; ++ty) {
            for (int tx = 0; tx < 4; ++tx) {
                if (shapes[t.type][new_rotation][ty][tx]) {
                    int world_x = t.x + tx;
                    int world_y = t.y + ty;

                    if (world_x < 0 || world_x >= FIELD_W || world_y >= FIELD_H || field[world_y][world_x]) {
                        return;  // We can't turn
                    }
                }
            }
        }

        t.rotation = new_rotation;  // We are making a turn
    };

    void move_down(Tetromino& t) {

        // Let's check if we can move the tetromino down
        if (can_move_down(t)) {
            t.y++;  // If we can, we move the tetromino down one square.
        } else {
            // If we can't, we fix the tetromino in the field
            for (int ty = 0; ty < 4; ++ty) {
                for (int tx = 0; tx < 4; ++tx) {
                    if (shapes[t.type][t.rotation][ty][tx]) {
                        int world_x = t.x + tx;
                        int world_y = t.y + ty;
                        if (world_y < FIELD_H) {
                            field[world_y][world_x] = 1;  // We fix the block on the field

                        }
                    }
                }
            }
            // Tetromino is no longer moving, you can launch the next one
            t.y = -1;  // We set a special value to signal that the tetromino is locked
        }
    };

    void handle_input() {

        if (kbhit()) {
            char input = getchar();

            if (input == 27) {  // ESC for exit
                game_is_running = false;
            } else if (input == 'a' || input == 'A') {  // left
                move_left(t);
            } else if (input == 'd' || input == 'D') {  // right
                move_right(t);
            } else if (input == 's' || input == 'S') {  // down
                move_down(t);
            } else if (input == 'w' || input == 'W') {  // rotate
                rotate(t);
            }
        }
    };
};

class Game
{

    public:
        void update_game(){
            Input input;
            input.move_down(t); // i think move_down most be a update_game Function.
            // we need colishion
        };

};


class Render
{
  
public:
   //struct Tetromino {
    //   int shapes[4][4][4][4]; // [type][rotate][y][x].
    // };
  //Tetromino t;;
  //  struct ActiveTetromino {
  //  int type;
  //  int rotation;
  //  int x;
  //  int y;
  //};
  //ActiveTetromino t;

  /*int drawfiled(){*/
  /**/
  /*  for(int a = 0; a < 20 ; a++)*/
  /*    {*/
  /*      for(int b = 0; b < 10; b++)*/
  /*        {*/
  /*          std::cout << Field[b][a] << " " ;*/
  /*        }*/
  /*        std::cout << "\n";*/
  /*    }  */
  /*    return 0;*/
  /*};*/
  /**/
  /*void drawtetromino(){ // i dont sure  we need it const*/
  /**/
  /*  Tetromino t = {*/
  /*    {*/
  /*      {   // type 0*/
  /*        {   // rotate 0*/
  /*          {0,0,0,0},*/
  /*          {1,1,1,1},*/
  /*          {0,0,0,0},*/
  /*          {0,0,0,0}*/
  /*        },*/
  /*        {   // rotate 1*/
  /*          {0,0,1,0},*/
  /*          {0,0,1,0},*/
  /*          {0,0,1,0},*/
  /*          {0,0,1,0}*/
  /*        },*/
  /*        {0}, // rotate 2*/
  /*        {0}  // rotate 3*/
  /*      },*/
  /*      {0}, {0}, {0} // остальные типы*/
  /*    }*/
  /*  };*/
  /*  int type = 0;*/
  /*  int rot  = 0;*/
  /**/
  /*  for (int y = 0; y < 4; y++) {*/
  /*      for (int x = 0; x < 4; x++) {*/
  /*          std::cout << (t.shapes[type][rot][y][x] ? "[]" : ".");*/
  /*      }*/
  /*      std::cout << '\n';*/
  /*  };*/
  /*};*/


  // draw field and tetromino
  void draw_frame() {

    for (int fy = 0; fy < FIELD_H; fy++) {
      for (int fx = 0; fx < FIELD_W; fx++) {

        // draw left wall
        if (fx == 0) {
          std::cout << "<!"; 
          continue;
        }
        // draw right wall
        if (fx == FIELD_W - 1) {
          std::cout << "!>";
          continue;
        }       
        // draw the ground
        if (fy == FIELD_H - 1) {
          std::cout << "==";
          continue;
        } 
        
        // check if there is the block 
        if (field[fy][fx]) {
          std::cout << "[]";
          continue;
        }

        // check if there is tetromino in current location
        bool drawn = false;
        for (int ty = 0; ty < 4 && !drawn; ty++) {
          for (int tx = 0; tx < 4 && !drawn; tx++) {
            if (shapes[t.type][t.rotation][ty][tx]) {
              int world_x = t.x + tx;
              int world_y = t.y + ty;
              if (world_x == fx && world_y == fy) {
                std::cout << "[]";
                drawn = true;
              }
            }
          }
        }

        // if there is empty space draw ..
        if (!drawn)
          std::cout << "..";
      }
      std::cout << '\n';
    }
  };

  void clear_screen(){

    std::cout << "\033[H\033[2J" ;
  };

  void sleep_ms(int ms) {

    std::this_thread::sleep_for(std::chrono::milliseconds(ms));
  };
  
  void draw(){

    clear_screen();
    sleep_ms(16); // must be a int variable i need to investigate that. NOTE that is temporay version because is can get faster or slower by different cpu.
    draw_frame();
  };

 };


int main(){

  Render render;
  Input input;
  Game game; 

  // Сохраняем старое состояние терминала
  TerminalState oldState = saveTerminal();

  // entering alt buffer
  std::cout << "\033[?1049h";
  // hide cursor
  std::cout << "\033[?25l";
  disableEcho();

  while (game_is_running == true){
    input.handle_input();
    game.update_game();
    render.draw();
      };


  restoreTerminal(oldState);
  // exeting alt buffer
  std::cout << "\033[?1049l";
  // show cursor again
  std::cout << "\033[?25h"; 
  // enableEcho();

  return 0;

};



/*
 что еще написать:
управление сбоку
счетчик очков
 */
