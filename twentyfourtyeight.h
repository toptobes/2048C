#ifndef TWENTYFOURTYEIGHT_H
#define TWENTYFOURTYEIGHT_H

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <minmax.h>
#include <memory.h>

#define SCREEN_HEIGHT 900
#define SCREEN_WIDTH 600

#define TOP_PADDING (SCREEN_HEIGHT - SCREEN_WIDTH)

#define OUTER_PADDING 20
#define TILE_PADDING 10

#define BOARD_ROWS 4
#define BOARD_COLS 4

#define TILE_SIZE ((SCREEN_WIDTH - 2 * OUTER_PADDING - (BOARD_COLS - 1) * TILE_PADDING) / (float) BOARD_COLS)

#define BACKGROUND_COLOR ((Color) {187, 173, 160, 255})
#define DARKER_BACKGROUND_COLOR ((Color) {139, 125, 112, 255})

#define ORANGE_COLOR ((Color) {215, 119, 69, 255})

#define BLANK_TILE_COLOR ((Color) {207, 195, 184, 255})
#define TWO_TILE_COLOR ((Color) {238, 228, 218, 255})
#define FOUR_TILE_COLOR ((Color) {237, 224, 200, 255})
#define EIGHT_TILE_COLOR ((Color) {242, 177, 121, 255})
#define SIXTEEN_TILE_COLOR ((Color) {245, 149, 99, 255})
#define THIRTYTWO_TILE_COLOR ((Color) {246, 124, 95, 255})
#define SIXTYFOUR_TILE_COLOR ((Color) {246, 94, 59, 255})
#define ONETWENTYEIGHT_TILE_COLOR ((Color) {237, 207, 114, 255})
#define TWOFIFTYSIX_TILE_COLOR ((Color) {237, 204, 97, 255})
#define FIVEONETWO_TILE_COLOR ((Color) {237, 200, 80, 255})
#define TENTWENTYFOUR_TILE_COLOR ((Color) {237, 197, 63, 255})
#define TWOFOURTYEIGHT_TILE_COLOR ((Color) {237, 194, 46, 255})
#define UNDEFINED_TILE_COLOR ((Color) {35, 148, 62, 255})

#define SWAP(a, b) { int temp = a; a = b; b = temp; }

typedef enum {
    UP,
    DOWN,
    LEFT,
    RIGHT
} Direction;

typedef int Board[BOARD_ROWS][BOARD_COLS];

typedef unsigned long long int Score;

bool updateDirection(Direction *direction);
void updateBoard(Board board, Board prev_board, Direction direction, Score *score);

void generateNTiles(Board board, int n);

void squashUp(Board board, Score *score);
void dezerofyUp(Board board);

void squashLeft(Board board, Score *score);
void dezerofyLeft(Board board);

void flipBoardHorizontal(Board board);
void flipBoardVertical(Board board);

void squashDown(Board board, Score *score);
void squashRight(Board board, Score *score);

Color getColor(int tile_value);

void printBoard(Board board);
#endif // TWENTYFOURTYEIGHT_H
