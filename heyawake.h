/*
 * Заголовочный файл генератора головоломки Хэявакэ
 * Автор: Золотарев.Д бИЦ-251
 */

#ifndef HEYAWAKE_H
#define HEYAWAKE_H

#define N 6              /*размер поля*/
#define ROWS 3           /*кол-во блоков по строкам*/
#define COLS 3           /*кол-во блоков по столбцам*/
#define MIN_BLACK 5      /*минимальное число черных клеток*/
#define MAX_BLACK 10     /*максимальное число черных клеток*/
#define MAX_ATTEMPTS 500 /*максимальное число попыток генерации*/
#define SHOW_PUZZLE 0    /*режим вывода головоломки*/
#define SHOW_SOLUTION 1  /*режим вывода решения*/

typedef struct {
    int field[N][N];   /*поле с: 0 — белая, 1 — чёрная*/
    int room_id[N][N]; /*ID комнат*/
} game_t;

void log_error(const char* msg);
int generate_rooms(game_t* pgame);
int black_rule(game_t* pgame, int x, int y);
int rooms_rule(game_t* pgame);
int place_black(game_t* pgame, int* pblacks);
int count_blacks_in_rooms(game_t* pgame, int room_blacks[]);
int generate_game(game_t* pgame, int room_blacks[]);
void print_field(game_t* pgame, int room_blacks[], int show_solution);
int save_field_to_file(game_t* pgame, int room_blacks[]);

#endif