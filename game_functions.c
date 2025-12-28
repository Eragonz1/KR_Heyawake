/*
 * Пользовательские функции генератора головоломки Хэявакэ
 * Автор: Золотарев.Д бИЦ-251
 */

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include "heyawake.h"

 /**
 * Логирование ошибок
 *
 * Записывает сообщение об ошибке в файл error.log
 */
void log_error(const char* msg) {
    FILE* file = fopen("error.log", "a");

    if (file) {
        fprintf(file, "%s\n", msg);
        fclose(file);
    }
}

/**
 * Генерирует разбиение поля на комнаты
 *
 * @param pgame - указатель на структуру игры
 * @return 1 при успешной генерации
 */
int generate_rooms(game_t* pgame) {
    int row_h[ROWS];
    int col_w[COLS];

    /*подбор высоты комнаты*/
    while (1) {
        row_h[0] = 1 + rand() % 4;
        row_h[1] = 1 + rand() % 4;
        row_h[2] = N - row_h[0] - row_h[1];

        if (row_h[2] >= 1 && row_h[2] <= 4) {
            break;
        }
    }

    /*подбор ширины комнаты*/
    while (1) {
        col_w[0] = 1 + rand() % 4;
        col_w[1] = 1 + rand() % 4;
        col_w[2] = N - col_w[0] - col_w[1];

        if (col_w[2] >= 1 && col_w[2] <= 4) {
            break;
        }
    }

    int x = 0;    /*текущая координата x (строка) для блока*/
    int room = 0; /*текущая комната*/

    for (int row_block = 0; row_block < ROWS; row_block++) {

        int y = 0; /*текущая координата y (столбец) для блока*/

        for (int col_block = 0; col_block < COLS; col_block++) {

            int h = row_h[row_block]; /*высота комнаты (количество строк)*/
            int w = col_w[col_block]; /*ширина комнаты (количество столбцов)*/

            int total = h * w;

            for (int k = 0; k < total; k++) {

                int ix = x + (k / w); /*строка внутри блока*/
                int iy = y + (k % w); /*столбец внутри блока*/

                pgame->room_id[ix][iy] = room;
            }

            y += w; /*сдвигаем y (столбец) на ширину комнаты*/
            room++;
        }

        x += row_h[row_block]; /*сдвиг вниз (по строкам) после работы со строкой блоков*/
    }

    return 1;
}

/**
 * Проверяет отсутствие соседних чёрных клеток
 *
 * @param pgame - игровое поле
 * @param x - строка
 * @param y - столбец
 * @return 1 если можно ставить, 0 если нельзя
 */
int black_rule(game_t* pgame, int x, int y) {
    if (x > 0 && pgame->field[x - 1][y]) return 0;     /*левый сосед*/
    if (x < N - 1 && pgame->field[x + 1][y]) return 0; /*правый сосед*/
    if (y > 0 && pgame->field[x][y - 1]) return 0;     /*верхний сосед*/
    if (y < N - 1 && pgame->field[x][y + 1]) return 0; /*нижний сосед*/
    return 1;
}

/**
 * Проверяет правило не более двух комнат подряд между чёрными клетками
 *
 * @param pgame - игровое поле
 * @return 1 если правило соблюдено, 0 если нарушено
 */
int rooms_rule(game_t* pgame) {
    /*Проверка строк*/
    for (int x = 0; x < N; x++) {
        int last = -1;
        int count = 0;

        for (int y = 0; y < N; y++) {

            if (pgame->field[x][y] == 1) { /*клетка черная*/
                last = -1;
                count = 0;
            }
            else {
                int room = pgame->room_id[x][y];

                if (room != last) {
                    last = room;
                    count++;

                    if (count > 2) {
                        return 0;
                    }
                }
            }
        }
    }

    /*Проверка столбцов*/
    for (int y = 0; y < N; y++) {
        int last = -1;
        int count = 0;

        for (int x = 0; x < N; x++) {

            if (pgame->field[x][y] == 1) { /*клетка черная*/
                last = -1;
                count = 0;
            }
            else {
                int room = pgame->room_id[x][y];

                if (room != last) {
                    last = room;
                    count++;

                    if (count > 2) {
                        return 0;
                    }
                }
            }
        }
    }

    return 1;
}

/**
 * Генерирует чёрные клетки на поле
 *
 * @param pgame - игровое поле
 * @param pblacks - указатель для возврата количества поставленных чёрных клеток
 * @return 1 при успехе, 0 при ошибке
 */
int place_black(game_t* pgame, int* pblacks) {
    int placed = 0;   /*счетчик размещенных черных клеток*/
    int attempts = 0; /*счетчик попыток размещения*/

    /*Очистка поля*/
    for (int x = 0; x < N; x++) {
        for (int y = 0; y < N; y++) {
            pgame->field[x][y] = 0;
        }
    }

    int target = MIN_BLACK + rand() % (MAX_BLACK - MIN_BLACK + 1); /*случайное количество точек для выставления (Мин-Макс)*/

    while (placed < target && attempts < MAX_ATTEMPTS) {
        int x = rand() % N; /*случайная строка*/
        int y = rand() % N; /*случайный столбец*/
        attempts++;

        if (pgame->field[x][y] == 1) { /*клетка уже черная*/
            continue;
        }

        if (black_rule(pgame, x, y) == 0) {
            continue;
        }

        pgame->field[x][y] = 1; /*ставим черную*/
        placed++;
    }

    *pblacks = placed;
    return (placed >= MIN_BLACK);
}

/**
 * Генерация игрового поля
 *
 * @param pgame - игровое поле
 * @param room_blacks - массив для хранения количества черных
 * @return 1 при успехе, 0 при ошибке
 */
int generate_game(game_t* pgame, int room_blacks[]) {
    for (int attempt = 0; attempt < MAX_ATTEMPTS; attempt++) {

        generate_rooms(pgame);

        int blacks;

        if (place_black(pgame, &blacks) == 0) {
            continue;
        }

        if (rooms_rule(pgame) == 0) {
            continue;
        }

        count_blacks_in_rooms(pgame, room_blacks);

        return 1;
    }

    log_error("Не удалось сгенерировать корректное поле");
    return 0;
}

/*
 * Подсчитывает количество чёрных клеток в комнатах
 *
 * @param pgame - игровое поле
 * @param room_blacks -  массив для заполнения количества черных
 * @return 1
 */
int count_blacks_in_rooms(game_t* pgame, int room_blacks[]) {
    for (int i = 0; i < ROWS * COLS; i++) {
        room_blacks[i] = 0;
    }

    for (int x = 0; x < N; x++) {

        for (int y = 0; y < N; y++) {

            if (pgame->field[x][y]) {
                room_blacks[pgame->room_id[x][y]]++;
            }

        }

    }

    return 1;
}

/**
 * Выводит поле на экран
 *
 * @param pgame - игровое поле
 * @param room_blacks - массив с количеством черных клеток в комнатах
 * @param mode - режим вывода
 */
void print_field(game_t* pgame, int room_blacks[], int show_solution) {
    int printed[ROWS * COLS] = { 0 };

    /*Верхняя граница*/
    for (int y = 0; y < N; y++) {
        printf("+===");
    }

    printf("+\n");

    /*Вывод строк*/
    for (int x = 0; x < N; x++) {

        /*Вывод ячеек строки*/
        for (int y = 0; y < N; y++) {

            if (y == 0 || pgame->room_id[x][y] != pgame->room_id[x][y - 1]) {
                printf("#");
            }
            else {
                printf("|");
            }

            if (show_solution) {
                printf(pgame->field[x][y] ? " X " : "   ");
            }
            else {
                int id = pgame->room_id[x][y];

                if (!printed[id]) {
                    printed[id] = 1;
                    printf(room_blacks[id] ? " %d " : "   ", room_blacks[id]);
                }
                else {
                    printf("   ");
                }
            }
        }

        printf("#\n");

        /*Вывод нижних границ*/
        for (int y = 0; y < N; y++) {
            printf("+");

            if (x == N - 1 || pgame->room_id[x][y] != pgame->room_id[x + 1][y]) {
                printf("===");
            }
            else {
                printf("---");
            }

        }
        printf("+\n");
    }
}

/**
 * Сохраняет игровое поле в файл
 *
 * @param pgame - игровое поле
 * @param room_blacks - чёрные клетки по комнатам
 * @return 1 при успехе, 0 при ошибке
 */
int save_field_to_file(game_t* pgame, int room_blacks[]) {
    FILE* file;
    int x;
    int y;
    int printed[ROWS * COLS] = { 0 };

    file = fopen("heyawake.txt", "w");

    if (file == NULL) {
        return 0;
    }

    /*Верхняя граница*/
    for (y = 0; y < N; y++) {
        fprintf(file, "+===");
    }

    fprintf(file, "+\n");

    /*Вывод строк*/
    for (x = 0; x < N; x++) {

        /*Вывод ячеек строки*/
        for (y = 0; y < N; y++) {

            if (y == 0 || pgame->room_id[x][y] != pgame->room_id[x][y - 1]) {
                fprintf(file, "#");
            }
            else {
                fprintf(file, "|");
            }

            int id = pgame->room_id[x][y];
            if (!printed[id]) {
                printed[id] = 1;
                fprintf(file, room_blacks[id] ? " %d " : "   ",
                    room_blacks[id]);
            }
            else {
                fprintf(file, "   ");
            }

        }

        fprintf(file, "#\n");

        /*Вывод нижних границ*/
        for (y = 0; y < N; y++) {
            fprintf(file, "+");

            if (x == N - 1 ||
                pgame->room_id[x][y] != pgame->room_id[x + 1][y]) {
                fprintf(file, "===");
            }
            else {
                fprintf(file, "---");
            }

        }

        fprintf(file, "+\n");
    }

    fclose(file);
    return 1;
}