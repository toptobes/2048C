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

    //-Window dragging variables (for desktop) ----------------------------------------------------------------
        Vector2 mousePosition = {0};
        Vector2 windowPosition = {500, 200};
        Vector2 panOffset = mousePosition;
        bool dragWindow = false;
    //--------------------------------------------------------------------------------------------------------

    //-Initialize long-lived game vars-------------------------------------------------------------------------
        Score high_score = 0;
    //---------------------------------------------------------------------------------------------------------

    //-Initialize automatic game vars--------------------------------------------------------------------------
        Direction direction = UP;

        Board board = {
                {0, 0, 0, 0},
                {0, 0, 0, 0},
                {0, 0, 0, 0},
                {0, 0, 0, 0}
        };

        Score score = 0;

        srand((unsigned) time(NULL));

        generateNTiles(board, 2);
    //---------------------------------------------------------------------------------------------------------

    printBoard(board);

    while (!WindowShouldClose()) {

        if (updateDirection(&direction)) {
            updateBoard(board, direction, &score);
        }

        //-Drag window-------------------------------------------------------------------------------------------
        {
            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                dragWindow = true;
                panOffset = GetMousePosition();
            }

            if (dragWindow) {
                windowPosition.x = GetMousePosition().x - panOffset.x + windowPosition.x;
                windowPosition.y = GetMousePosition().y - panOffset.y + windowPosition.y;
            }

            if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) {
                dragWindow = false;
            }

            SetWindowPosition(windowPosition.x, windowPosition.y);
        }
        //---------------------------------------------------------------------------------------------------------

        BeginDrawing();

        //-Set the background----------------------------------------------------------------------------------
            ClearBackground(BLANK);
            Rectangle background_rect = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
            DrawRectangleRounded(background_rect, .05f, 250, BACKGROUND_COLOR);
        //-----------------------------------------------------------------------------------------------------

        //-Render board----------------------------------------------------------------------------------------
        {
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
        }
        //-----------------------------------------------------------------------------------------------------

        //-Render tile text------------------------------------------------------------------------------------
        {
            for (int r = 0; r < BOARD_ROWS; r++) {
                for (int c = 0; c < BOARD_COLS; c++) {
                    int tt_x_offset = OUTER_PADDING + c * TILE_SIZE + c * TILE_PADDING;
                    int tt_y_offset = OUTER_PADDING + r * TILE_SIZE + r * TILE_PADDING + TOP_PADDING;

                    int len = snprintf(NULL, 0, "%d", board[r][c]);
                    char str[len + 1];
                    snprintf(str, len + 1, "%d", board[r][c]);

                    if (board[r][c] != 0) {
                        DrawText(str, tt_x_offset + TILE_SIZE / 2, tt_y_offset + TILE_SIZE / 2, TILE_SIZE / 2.5f,
                                 BLACK);
                    }
                }
            }
        }
        //-----------------------------------------------------------------------------------------------------

        //-Render score-----------------------------------------------------------------------------------------
        {
            int s_len = snprintf(NULL, 0, "%d", score);
            char s_num[s_len + 1];
            snprintf(s_num, s_len + 1, "%d", score);

            float s_num_font_size = TILE_SIZE / 3.5f;
            float s_text_font_size = s_num_font_size / 1.3f;

            float s_text_size = MeasureText(s_num, s_text_font_size);
            float s_num_size = MeasureText("Score:", s_num_font_size );
            float s_max_size = max(s_text_size, s_num_size);

            int s_pos_x = SCREEN_WIDTH - OUTER_PADDING - s_max_size;

            Rectangle s_rect = {
                    s_pos_x - TILE_PADDING,
                    OUTER_PADDING - TILE_PADDING,
                    s_max_size + 2 * TILE_PADDING,
                    s_num_font_size + s_text_font_size + 2 * TILE_PADDING
            };

            DrawRectangleRounded(s_rect, .1f, 100, DARKER_BACKGROUND_COLOR);

            DrawText("Score:", s_pos_x, OUTER_PADDING, s_text_font_size, BLACK);

            DrawText(s_num, s_pos_x, OUTER_PADDING + s_text_font_size + TILE_PADDING, s_num_font_size, BLACK);
        //-----------------------------------------------------------------------------------------------------

        //-Render high score------------------------------------------------------------------------------------
            int hs_len = snprintf(NULL, 0, "%d", high_score);
            char hs_num[hs_len + 1];
            snprintf(hs_num, hs_len + 1, "%d", high_score);

            float hs_num_font_size = TILE_SIZE / 3.5f;
            float hs_text_font_size = hs_num_font_size / 1.3f;

            float hs_text_size = MeasureText(hs_num, hs_text_font_size);
            float hs_num_size = MeasureText("H. Score:", hs_num_font_size );
            float hs_max_size = max(hs_text_size, hs_num_size);

            int hs_pos_x = SCREEN_WIDTH - OUTER_PADDING - hs_max_size - (SCREEN_WIDTH - s_pos_x) - TILE_PADDING;

            Rectangle hs_rect = {
                    hs_pos_x - TILE_PADDING,
                    OUTER_PADDING - TILE_PADDING,
                    hs_max_size + 2 * TILE_PADDING,
                    hs_num_font_size + hs_text_font_size + 2 * TILE_PADDING
            };

            DrawRectangleRounded(hs_rect, .1f, 100, DARKER_BACKGROUND_COLOR);

            DrawText("H. Score:", hs_pos_x, OUTER_PADDING, hs_text_font_size,  BLACK);

            DrawText(hs_num, hs_pos_x, OUTER_PADDING + hs_text_font_size + TILE_PADDING , hs_num_font_size, BLACK);
        }
        //-----------------------------------------------------------------------------------------------------

        high_score = max(high_score, score);

        EndDrawing();
    }

    CloseWindow();

    return 0;
}

#pragma clang diagnostic pop

void updateBoard(Board board, Direction direction, Score *score) {
    switch (direction) {
        case UP:
            squashUp(board, score);
            break;
        case DOWN:
            squashDown(board, score);
            break;
        case LEFT:
            squashLeft(board, score);
            break;
        case RIGHT:
            squashRight(board, score);
            break;
        default:
            break;
    }

    generateNTiles(board, 1);
    printBoard(board);
}

bool updateDirection(Direction *direction) {
    int _direction;
    bool changed = false;

    while ((_direction = GetKeyPressed()) != 0) {
        switch (_direction) {
            case KEY_W:
            case KEY_K:
            case KEY_UP:
                *direction = UP;
                changed = true;
                break;
            case KEY_A:
            case KEY_H:
            case KEY_LEFT:
                *direction = LEFT;
                changed = true;
                break;
            case KEY_S:
            case KEY_J:
            case KEY_DOWN:
                *direction = DOWN;
                changed = true;
                break;
            case KEY_D:
            case KEY_L:
            case KEY_RIGHT:
                *direction = RIGHT;
                changed = true;
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
                    board[r][c] = (rand() % 10 != 0) ? 2 : 4;
                    goto cont;
                } else {
                    rn--;
                }
            }
        }
        cont:;
    }
}

void squashUp(Board board, Score *score) {
    dezerofyUp(board);

    for (int c = 0; c < BOARD_COLS; c++) {
        for (int r = BOARD_ROWS - 1; r > 0; r--) {
            if (board[r - 1][c] == board[r][c]) {
                board[r - 1][c] += board[r][c];
                *score += board[r - 1][c];
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

void squashLeft(Board board, Score *score) {
    dezerofyLeft(board);

    for (int r = 0; r < BOARD_ROWS; r++) {
        for (int c = BOARD_COLS - 1; c > 0; c--) {
            if (board[r][c - 1] == board[r][c]) {
                board[r][c - 1] += board[r][c];
                *score += board[r][c - 1];
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

void squashRight(Board board, Score *score) {
    flipBoardHorizontal(board);
    squashLeft(board, score);
    flipBoardHorizontal(board);
}

void squashDown(Board board, Score *score) {
    flipBoardVertical(board);
    squashUp(board, score);
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