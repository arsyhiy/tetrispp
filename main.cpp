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

// tetromino
struct ActiveTetromino {
  int type;
  int rotation;
  int x;
  int y;
};
ActiveTetromino t = {0, 0, FIELD_W / 2, 0};

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



class Input{

public:

    // Функция для захвата символа с клавиатуры без ожидания Enter
    char getch() {

        struct termios oldt, newt;
        char ch;

        tcgetattr(STDIN_FILENO, &oldt);  // Получаем текущие настройки терминала
        newt = oldt;
        newt.c_lflag &= ~ICANON;  // Отключаем режим канонического ввода
        newt.c_lflag &= ~ECHO;    // Отключаем эхо (не отображать введенный символ)
        tcsetattr(STDIN_FILENO, TCSANOW, &newt);  // Применяем новые настройки
    
        ch = getchar();  // Читаем символ
    
        tcsetattr(STDIN_FILENO, TCSANOW, &oldt);  // Восстанавливаем старые настройки
        return ch;

    };

    bool kbhit() {

        struct termios oldt;
        struct termios newt;
        int ch;
        int oldf;
    
        tcgetattr(STDIN_FILENO, &oldt);  // Получаем текущие настройки терминала
        newt = oldt;
        newt.c_lflag &= ~ICANON;
        newt.c_lflag &= ~ECHO;
        newt.c_cc[VMIN] = 1;
        newt.c_cc[VTIME] = 0;
        tcsetattr(STDIN_FILENO, TCSANOW, &newt);
        oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
        fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);
    
        ch = getchar();
    
        tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
        fcntl(STDIN_FILENO, F_SETFL, oldf);
    
        if(ch != EOF) {
            ungetc(ch, stdin);  // Возвращаем символ в поток ввода
            return true;
        }
        return false;
    };

    // rewrite it case switch statements
    //void handle_input(){
    //if (kbhit()) {
    //char input = getchar();  // Считываем нажатую клавишу
    //if (input == 'q'){
    // std::cout << " exiting";
    // game_is_running = false;
    // system("clear");
    //    };
    //  }
    //};

    bool can_move_down(const ActiveTetromino& t) {

        // Проверяем, можем ли мы опустить тетромино вниз
        for (int ty = 0; ty < 4; ++ty) {
            for (int tx = 0; tx < 4; ++tx) {
                // Если ячейка тетромино активна
                if (shapes[t.type][t.rotation][ty][tx]) {
                    int world_x = t.x + tx;
                    int world_y = t.y + ty + 1;  // Мы проверяем на 1 клетку ниже

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
    void move_left(ActiveTetromino& t) {

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
    void move_right(ActiveTetromino& t) {

        // Проверяем, можем ли мы переместиться вправо
        for (int ty = 0; ty < 4; ++ty) {
            for (int tx = 0; tx < 4; ++tx) {
                if (shapes[t.type][t.rotation][ty][tx]) {
                    int world_x = t.x + tx + 1;
                    int world_y = t.y + ty;

                    if (world_x >= FIELD_W || (world_y >= 0 && world_y < FIELD_H && field[world_y][world_x])) {
                        return;  // Не можем двигаться вправо
                    }
                }
            }
        }

        t.x++;  // Перемещаем вправо
    };

    // Функция для вращения тетромино
    void rotate(ActiveTetromino& t) {

        int new_rotation = (t.rotation + 1) % 4;
        // Проверяем, можем ли мы повернуть тетромино
        for (int ty = 0; ty < 4; ++ty) {
            for (int tx = 0; tx < 4; ++tx) {
                if (shapes[t.type][new_rotation][ty][tx]) {
                    int world_x = t.x + tx;
                    int world_y = t.y + ty;

                    if (world_x < 0 || world_x >= FIELD_W || world_y >= FIELD_H || field[world_y][world_x]) {
                        return;  // Не можем повернуть
                    }
                }
            }
        }

        t.rotation = new_rotation;  // Осуществляем поворот
    };

    void move_down(ActiveTetromino& t) {

        // Проверяем, можем ли опустить тетромино вниз
        if (can_move_down(t)) {
            t.y++;  // Если можем, сдвигаем тетромино на одну клетку вниз
        } else {
            // Если не можем, фиксируем тетромино в поле
            for (int ty = 0; ty < 4; ++ty) {
                for (int tx = 0; tx < 4; ++tx) {
                    if (shapes[t.type][t.rotation][ty][tx]) {
                        int world_x = t.x + tx;
                        int world_y = t.y + ty;
                        if (world_y < FIELD_H) {
                            field[world_y][world_x] = 1;  // Фиксируем блок на поле
                        }
                    }
                }
            }
            // Тетромино теперь не двигается, можно запускать следующее
            t.y = -1;  // Устанавливаем специальное значение, чтобы сигнализировать, что тетромино зафиксировано
        }
    };

    void handle_input() {

        if (kbhit()) {
            char input = getch();

            if (input == 27) {  // ESC для выхода
                game_is_running = false;
            } else if (input == 'a' || input == 'A') {  // Влево
                move_left(t);
            } else if (input == 'd' || input == 'D') {  // Вправо
                move_right(t);
            } else if (input == 's' || input == 'S') {  // Вниз
                move_down(t);
            } else if (input == 'w' || input == 'W') {  // Поворот
                rotate(t);
            }
        }
    };
};

void update_game(){

  Input input;
  input.move_down(t);
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
  // entering alt buffer
  std::cout << "\033[?1049h";
  
  while (game_is_running == true){
    input.handle_input();
    update_game();
    render.draw();
      };

  // exeting alt buffer
  std::cout << "\033[?1049l";

  return 0;

};

