#include "raylib.h"
#include "twentyfourtyeight.h"

#pragma clang diagnostic push
#pragma ide diagnostic ignored "cppcoreguidelines-narrowing-conversions"

int main(void) {
    SetConfigFlags(FLAG_WINDOW_TRANSPARENT | FLAG_WINDOW_UNDECORATED | FLAG_WINDOW_TOPMOST);
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "raylib [core] example - basic window");

    SetTargetFPS(60);

    Direction direction = UP;

    Board board = {
            {0, 0, 0, 0},
            {0, 0, 0, 0},
            {0, 0, 0, 0},
            {0, 0, 0, 0}
    };

    while (!WindowShouldClose()) {

        if (updateDirection(&direction)) {
            switch (direction) {
                case UP:
                case DOWN:
                case LEFT:
                case RIGHT:
                default:
                    break;
            }
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
                    Rectangle tile = {
                            OUTER_PADDING + c * TILE_SIZE + c * TILE_PADDING,
                            OUTER_PADDING + r * TILE_SIZE + r * TILE_PADDING + TOP_PADDING,
                            TILE_SIZE,
                            TILE_SIZE
                    };
                    Color color = getColor(board[r][c]);
                    DrawRectangleRounded(tile, .05f, 100, color);
                }
            }
        //-----------------------------------------------------------------------------------------------------

        EndDrawing();
    }

    CloseWindow();

    return 0;
}

#pragma clang diagnostic pop

int updateDirection(Direction *direction) {
    Direction _direction;
    int changed = 0;

    while ((_direction = GetCharPressed()) != 0) {
        switch (_direction) {
            case 'k':
                *direction = UP;
                changed++;
                break;
            case 'h':
                *direction = LEFT;
                changed++;
                break;
            case 'j':
                *direction = DOWN;
                changed++;
                break;
            case 'l':
                *direction = RIGHT;
                changed++;
                break;
            default:
                break;
        }
    }

    return changed;
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

void squashUp(Board board) {
    for (int c = 0; c < BOARD_COLS; c++) {
        for (int r = 0; r < BOARD_ROWS - 1; r++) {
            if (board[r][c] == board[r + 1][c]) {
                board[r][c] += board[r + 1][c];
            }
        }
    }
}