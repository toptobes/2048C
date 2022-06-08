#pragma clang diagnostic push
#pragma ide diagnostic ignored "cert-msc51-cpp"
#pragma ide diagnostic ignored "cert-msc50-cpp"

#include "raylib.h"
#include "twentyfourtyeight.h"

#pragma clang diagnostic push
#pragma ide diagnostic ignored "cppcoreguidelines-narrowing-conversions"

int main(void) {
    SetConfigFlags(FLAG_WINDOW_TRANSPARENT | FLAG_WINDOW_UNDECORATED | FLAG_WINDOW_TOPMOST);
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "raylib [core] example - basic window");

    SetTargetFPS(60);

    //-Initialize----------------------------------------------------------------------------------------------
        Direction direction = UP;

        Board board = {
            {0, 0, 0, 0},
            {0, 0, 0, 0},
            {0, 0, 0, 0},
            {0, 0, 0, 0}
        };

        srand((unsigned) time(NULL));

        generateNTiles(board, 2);
    //---------------------------------------------------------------------------------------------------------

    printBoard(board);

    while (!WindowShouldClose()) {

        if (updateDirection(&direction)) {
            updateBoard(board, direction);
        }

        BeginDrawing();

        //-Set the background----------------------------------------------------------------------------------
        ClearBackground(BLANK);
        Rectangle background_rect = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
        DrawRectangleRounded(background_rect, .05f, 250, BACKGROUND_COLOR);
        //-----------------------------------------------------------------------------------------------------

        //-Draw the board--------------------------------------------------------------------------------------
        for (int r = 0; r < BOARD_ROWS; r++) {
            for (int c = 0; c < BOARD_COLS; c++) {
                int x_offset = OUTER_PADDING + c * TILE_SIZE + c * TILE_PADDING;
                int y_offset = OUTER_PADDING + r * TILE_SIZE + r * TILE_PADDING + TOP_PADDING;

                Rectangle tile = {
                        x_offset,
                        y_offset,
                        TILE_SIZE,
                        TILE_SIZE
                };
                Color color = getColor(board[r][c]);
                DrawRectangleRounded(tile, .05f, 100, color);

                int len = snprintf(NULL, 0, "%d", board[r][c]);
                char *str = malloc(len + 1);
                snprintf(str, len + 1, "%d", board[r][c]);

                if (board[r][c] != 0) {
                    DrawText(str, x_offset + TILE_SIZE / 2, y_offset + TILE_SIZE / 2, TILE_SIZE / 2, BLACK);
                }

                free(str);
            }
        }
        //-----------------------------------------------------------------------------------------------------

        EndDrawing();
    }

    CloseWindow();

    return 0;
}

#pragma clang diagnostic pop

void updateBoard(Board board, Direction direction) {
    switch (direction) {
        case UP:
            squashUp(board);
            break;
        case DOWN:
            squashDown(board);
            break;
        case LEFT:
            squashLeft(board);
            break;
        case RIGHT:
            squashRight(board);
            break;
        default:
            break;
    }

    generateNTiles(board, 1);
    printBoard(board);
}

int updateDirection(Direction *direction) {
    int _direction;
    int changed = 0;

    while ((_direction = GetKeyPressed()) != 0) {
        printf("%d\n", _direction);
        switch (_direction) {
            case KEY_W:
            case KEY_K:
            case KEY_UP:
                *direction = UP;
                changed++;
                break;
            case KEY_A:
            case KEY_H:
            case KEY_LEFT:
                *direction = LEFT;
                changed++;
                break;
            case KEY_S:
            case KEY_J:
            case KEY_DOWN:
                *direction = DOWN;
                changed++;
                break;
            case KEY_D:
            case KEY_L:
            case KEY_RIGHT:
                *direction = RIGHT;
                changed++;
                break;
            default:
                break;
        }
    }

    return changed;
}

void generateNTiles(Board board, int n) {
    for (int i = 0, e = 0; i < n; i++, e = 0) {
        for (int r = 0; r < BOARD_ROWS; r++) {
            for (int c = 0; c < BOARD_COLS; c++) {
                if (board[r][c] == 0) e++;
            }
        }

        int rn = rand() % e + 1;

        for (int r = 0; r < BOARD_ROWS; r++) {
            for (int c = 0; c < BOARD_COLS; c++) {
                if (board[r][c] != 0) continue;

                if (rn == 1) {
                    board[r][c] = 2;
                    goto cont;
                } else {
                    rn--;
                }
            }
        }
        cont:;
    }
}

void squashUp(Board board) {
    dezerofyUp(board);

    for (int c = 0; c < BOARD_COLS; c++) {
        for (int r = BOARD_ROWS - 1; r > 0; r--) {
            if (board[r - 1][c] == board[r][c]) {
                board[r - 1][c] += board[r][c];
                board[r--][c] = 0;
            }
        }
    }
    dezerofyUp(board);
}

void dezerofyUp(Board board) {
    for (int c = 0; c < BOARD_COLS; c++) {
        int i = 0;
        for (int r = 0; r < BOARD_ROWS; r++) {
            if (board[r][c] != 0) board[i++][c] = board[r][c];
        }
        while (i < BOARD_ROWS) board[i++][c] = 0;
    }
}

void squashLeft(Board board) {
    dezerofyLeft(board);

    for (int r = 0; r < BOARD_ROWS; r++) {
        for (int c = BOARD_COLS - 1; c > 0; c--) {
            if (board[r][c - 1] == board[r][c]) {
                board[r][c - 1] += board[r][c];
                board[r][c] = 0;
            }
        }
    }
    dezerofyLeft(board);
}

void dezerofyLeft(Board board) {
    for (int r = 0; r < BOARD_ROWS; r++) {
        int i = 0;
        for (int c = 0; c < BOARD_COLS; c++) {
            if (board[r][c] != 0) board[r][i++] = board[r][c];
        }
        while (i < BOARD_COLS) board[r][i++] = 0;
    }
}

void squashRight(Board board) {
    flipBoardHorizontal(board);
    squashLeft(board);
    flipBoardHorizontal(board);
}

void squashDown(Board board) {
    flipBoardVertical(board);
    squashUp(board);
    flipBoardVertical(board);
}

void flipBoardHorizontal(Board board) {
    for (int r = 0; r < BOARD_ROWS; r++) {
        for (int c = 0; c < BOARD_COLS / 2; c++) {
            SWAP(board[r][c], board[r][BOARD_COLS - c - 1])
        }
    }
}

void flipBoardVertical(Board board) {
    for (int r = 0; r < BOARD_ROWS / 2; r++) {
        for (int c = 0; c < BOARD_COLS; c++) {
            SWAP(board[r][c], board[BOARD_ROWS - 1 - r][c])
        }
    }
}

Color getColor(int tile_value) {
    switch (tile_value) {
        case 0:
            return BLANK_TILE_COLOR;
        case 2:
            return TWO_TILE_COLOR;
        case 4:
            return FOUR_TILE_COLOR;
        case 8:
            return EIGHT_TILE_COLOR;
        case 16:
            return SIXTEEN_TILE_COLOR;
        case 32:
            return THIRTYTWO_TILE_COLOR;
        case 64:
            return SIXTYFOUR_TILE_COLOR;
        case 128:
            return ONETWENTYEIGHT_TILE_COLOR;
        case 256:
            return TWOFIFTYSIX_TILE_COLOR;
        case 512:
            return FIVEONETWO_TILE_COLOR;
        case 1024:
            return TENTWENTYFOUR_TILE_COLOR;
        case 2048:
            return TWOFOURTYEIGHT_TILE_COLOR;
        default:
            return UNDEFINED_TILE_COLOR;
    }
}

void printBoard(Board board) {
    for (int r = 0; r < BOARD_ROWS; r++) {
        for (int c = 0; c < BOARD_COLS; c++) {
            printf("%d ", board[r][c]);
        }
        printf("\n");
    }
    printf("\n");
}

#pragma clang diagnostic pop