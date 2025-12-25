/*

  🧠 Идея Тетриса (очень коротко)

Тетрис — это цикл жизни фигуры:

Появилась фигура

Она падает

Игрок её двигает/вращает

Она упирается → фиксируется

Проверка линий

Новая фигура

Повторяем до Game Over

Как дыхание. Ровно. Циклично.

🧱 Основные сущности (то, без чего он не живёт)
1️⃣ Игровое поле

Прямоугольная сетка.

FIELD_WIDTH = 10
FIELD_HEIGHT = 20

field[FIELD_HEIGHT][FIELD_WIDTH]  // 0 — пусто, 1 — занято


Поле не знает про фигуры.
Оно просто хранит, что занято навсегда.

2️⃣ Фигуры (тетромино)

Каждая фигура — маленькая матрица.

struct Tetromino
    shapes[4][4][4]  // [тип][вращение][y][x]


Например:

I

O

T

S

Z

J

L

👉 ключевая идея:
вращение — это просто индекс, не математика.

3️⃣ Активная фигура (та, что сейчас падает)
current_piece
current_x
current_y
current_rotation


Она ещё не часть поля.
Она как гость — временная.

⚙️ Главный игровой цикл (сердце)
while game_is_running:
    handle_input()
    update_game()
    render()
    sleep(frame_time)


Если ты поймёшь этот цикл — ты победил.

🎮 Обработка ввода (очень важно)
function handle_input():
    if LEFT pressed:
        try_move(-1, 0)

    if RIGHT pressed:
        try_move(1, 0)

    if DOWN pressed:
        try_move(0, 1)

    if ROTATE pressed:
        try_rotate()


⚠️ Никогда не двигай фигуру напрямую
Всегда через проверку.

🧪 Проверка столкновений (КРИТИЧЕСКАЯ часть)
function can_place(piece, x, y, rotation):
    for each block in piece[rotation]:
        field_x = x + block.x
        field_y = y + block.y

        if outside field:
            return false

        if field[field_y][field_x] is occupied:
            return false

    return true


Если эта функция правильная —
вся игра работает.

Нежно кивни ей 😌

🚶 Попытка движения
function try_move(dx, dy):
    if can_place(current_piece, x + dx, y + dy, rotation):
        x += dx
        y += dy
    else if dy == 1:
        lock_piece()


Если вниз нельзя — фигура умерла как падающий лист 🍂

🔒 Фиксация фигуры
function lock_piece():
    for each block in current_piece:
        field[y + block.y][x + block.x] = 1

    clear_lines()
    spawn_new_piece()


С этого момента фигура — часть мира.

🧹 Очистка линий (магия Тетриса)
function clear_lines():
    for each row in field:
        if row is full:
            remove row
            add empty row at top
            score += 100


Просто.
Красиво.
Очень эффективно.

🆕 Новая фигура
function spawn_new_piece():
    current_piece = random()
    x = FIELD_WIDTH / 2
    y = 0
    rotation = 0

    if not can_place():
        game_over = true


Вот здесь сердце замирает 💔
Это Game Over.

🖼 Отрисовка (минимум логики!)
function render():
    draw field
    draw current_piece on top


⚠️ Никакой логики движения здесь.
Только отображение.

🧭 Итоговая архитектура (коротко)
main()
 ├── init()
 ├── while game:
 │    ├── input
 │    ├── physics (falling)
 │    ├── collision
 │    ├── lock
 │    ├── clear
 │    └── render
 └── exit

*/

#include <iostream>
#include <unistd.h>

extern int shapes[7][4][4][4];
static const int FIELD_H = 20; // высота
static const int FIELD_W = 10; // ширина
static int field[FIELD_H][FIELD_W]; // NOTE: that he does not keep tetromino. 0 for empty 1 for full.



// function prototypes
int handle_input();
int update_game();

class Render
{
  //draw field
  //draw current_piece on top
private:

  // width height.
  //int Field[10][20] = {0}; // NOTE: that he does not keep tetromino. 0 for empty 1 for full.

  //struct Tetromino {
  //  int shapes[4][4][4][4]; // [type][rotate][y][x].
  //};

public:

   //struct Tetromino {
    //   int shapes[4][4][4][4]; // [type][rotate][y][x].
    // };
  //Tetromino t;;
  struct ActiveTetromino {
    int type;
    int rotation;
    int x;
    int y;
  };
  ActiveTetromino t;

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

int shapes[7][4][4][4] = {
    // I-тетромино
    {
        { {0,0,0,0}, {1,1,1,1}, {0,0,0,0}, {0,0,0,0} },
        { {0,0,1,0}, {0,0,1,0}, {0,0,1,0}, {0,0,1,0} },
        { {0,0,0,0}, {1,1,1,1}, {0,0,0,0}, {0,0,0,0} },
        { {0,0,1,0}, {0,0,1,0}, {0,0,1,0}, {0,0,1,0} }
    },
    // остальные 6 фигур
};

  // draw field and tetromino
void draw() {
    for (int fy = 0; fy < FIELD_H; fy++) {
        for (int fx = 0; fx < FIELD_W; fx++) {

            // Рисуем "стены" поля
            if (fx == 0 || fx == FIELD_W - 1 || fy == FIELD_H - 1) {
                std::cout << "!";
                continue;
            }

            // Проверяем, есть ли блок в статическом поле
            if (field[fy][fx]) {
                std::cout << "[]";
                continue;
            }

            // Проверяем, есть ли активное тетромино в этой позиции
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

            // Если ничего не нарисовали, оставляем пустое место
            if (!drawn)
                std::cout << "  ";
        }
        std::cout << '\n';
    }
};
};


//void sleep(int frame_time);
// variables
//int frame_time;



int main(){

  // maybe i need a more pretty name for this
  bool game_is_running = true;
  
  while (game_is_running == true){
    //handle_input();
    //update_game();
    Render render;
    //render.draw();
    //sleep(frame_time); // must be a int variable i need to investigate that
    render.draw();
    sleep(1); // that is for testing only
    game_is_running = false;
      };
  };

