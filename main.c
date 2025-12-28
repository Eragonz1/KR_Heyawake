/*
 * Автор: Золотарев.Д бИЦ-251
 *
 * 
 * Программа - генератор головоломки Хэявакэ 6x6.
 * Формирует игровое поле, проверяет правила,
 * выводит головоломку, решение и сохраняет поле в файл
 */
#define _CRT_SECURE_NO_WARNINGS
#include <locale.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "heyawake.h"

int main(void) {
    game_t game;                  /*структура игры*/
    int room_blacks[ROWS * COLS]; /*черные клетки по комнатам*/
    int generated = 0;            /*флаг генерации*/

    srand((unsigned)time(NULL));

    printf("Генератор игровых полей 'Хэявакэ'\n");
    printf("Выполнил: Золотарев Даниил бИЦ-251\n");

    while (1) {
        int cmd;
        printf("\n1 - Сгенерировать поле");
        printf("\n2 - Показать решение");
        printf("\n3 - Сохранить поле в файл");
        printf("\n4 - Выход\n> ");
        scanf("%d", &cmd);

        switch (cmd) {

        case 1:
            if (generate_game(&game, room_blacks) == 1) {
                generated = 1;
                print_field(&game, room_blacks, SHOW_PUZZLE);
            }
            else {
                printf("Ошибка генерации поля\n");
                log_error("main: Ошибка генерации поля");
            }
            break;

        case 2:
            if (generated == 1) {
                print_field(&game, room_blacks, SHOW_SOLUTION);
            }
            else {
                printf("Поле еще не создано\n");
                log_error("main: Попытка вывода несозданного поля");
            }
            break;

        case 3:
            if (generated == 1) {
                if (save_field_to_file(&game, room_blacks) == 1) {
                    printf("Поле сохранено в файл\n");
                }
                else {
                    printf("Ошибка открытия файла\n");
                    log_error("main: Ошибка открытия файла");
                }
            }
            else {
                printf("Поле ещё не создано\n");
                log_error("main: Попытка сохранения в файл несозданного поля");
            }
            break;

        case 4:
            return 0;

        default:
            printf("Неверный пункт\n");
            break;
        }
    }
}