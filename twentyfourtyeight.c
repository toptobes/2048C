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

    reset:;

    //-Initialize automatic game vars--------------------------------------------------------------------------
        Direction direction = UP;

        Board board = {
                {0, 0, 0, 0},
                {0, 0, 0, 0},
                {0, 0, 0, 0},
                {0, 0, 0, 0}
        };

        Board prev_board = {
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
            updateBoard(board, prev_board, direction, &score);
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
            Rectangle background_rect = {0, 0, GetScreenWidth(), GetScreenHeight()};
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
                    int tt_x_offset = OUTER_PADDING + c * TILE_SIZE + c * TILE_PADDING + TILE_SIZE / 2;
                    int tt_y_offset = OUTER_PADDING + r * TILE_SIZE + r * TILE_PADDING + TILE_SIZE / 2 + TOP_PADDING;

                    int len = snprintf(NULL, 0, "%d", board[r][c]);
                    char str[len + 1];
                    snprintf(str, len + 1, "%d", board[r][c]);

                    if (board[r][c] != 0) {
                        DrawText(str, tt_x_offset, tt_y_offset, TILE_SIZE / 2.5f,BLACK);
                    }
                }
            }
        }
        //-----------------------------------------------------------------------------------------------------

        //-Render score-----------------------------------------------------------------------------------------
        {
            const float NUM_FONT_SIZE = TILE_SIZE / 3.5f;
            const float TEXT_FONT_SIZE = NUM_FONT_SIZE / 1.3f;

            const float RECT_HEIGHT = NUM_FONT_SIZE + TEXT_FONT_SIZE + 2 * TILE_PADDING;

            int s_len = snprintf(NULL, 0, "%d", score);
            char s_num[s_len + 1];
            snprintf(s_num, s_len + 1, "%d", score);

            float s_text_size = MeasureText(s_num, TEXT_FONT_SIZE);
            float s_num_size = MeasureText("Score:", NUM_FONT_SIZE );
            float s_max_size = max(s_text_size, s_num_size);

            int s_pos_x = GetScreenWidth() - OUTER_PADDING - s_max_size;

            Rectangle s_rect = {
                    s_pos_x - TILE_PADDING,
                    OUTER_PADDING - TILE_PADDING,
                    s_max_size + 2 * TILE_PADDING,
                    RECT_HEIGHT,
            };

            DrawRectangleRounded(s_rect, .1f, 100, DARKER_BACKGROUND_COLOR);

            DrawText("Score:", s_pos_x, OUTER_PADDING, TEXT_FONT_SIZE, BLACK);

            DrawText(s_num, s_pos_x, OUTER_PADDING + TEXT_FONT_SIZE + TILE_PADDING, NUM_FONT_SIZE, BLACK);
        //-----------------------------------------------------------------------------------------------------

        //-Render high score------------------------------------------------------------------------------------
            int hs_len = snprintf(NULL, 0, "%d", high_score);
            char hs_num[hs_len + 1];
            snprintf(hs_num, hs_len + 1, "%d", high_score);

            float hs_text_size = MeasureText(hs_num, TEXT_FONT_SIZE);
            float hs_num_size = MeasureText("H. Score:", NUM_FONT_SIZE );
            float hs_max_size = max(hs_text_size, hs_num_size);

            int hs_pos_x = GetScreenWidth() - OUTER_PADDING - hs_max_size - (GetScreenWidth() - s_pos_x) - TILE_PADDING;

            Rectangle hs_rect = {
                    hs_pos_x - TILE_PADDING,
                    OUTER_PADDING - TILE_PADDING,
                    hs_max_size + 2 * TILE_PADDING,
                    RECT_HEIGHT,
            };

            DrawRectangleRounded(hs_rect, .1f, 100, DARKER_BACKGROUND_COLOR);

            DrawText("H. Score:", hs_pos_x, OUTER_PADDING, TEXT_FONT_SIZE, BLACK);

            DrawText(hs_num, hs_pos_x, OUTER_PADDING + TEXT_FONT_SIZE + TILE_PADDING , NUM_FONT_SIZE, BLACK);
        //-----------------------------------------------------------------------------------------------------

        //-Render 'play again' button---------------------------------
            float pa_text_size = MeasureText("Play again", TEXT_FONT_SIZE);

            Rectangle pa_rect = {
                    TILE_PADDING,
                    TILE_PADDING,
                    hs_pos_x - 3 * TILE_PADDING,
                    RECT_HEIGHT,
            };

            float pa_text_pos_x = TILE_PADDING + (pa_rect.width - pa_text_size) / 2;
            float pa_text_pos_y = TILE_PADDING + (pa_rect.height - TEXT_FONT_SIZE) / 2;

            DrawRectangleRounded(pa_rect, .1f, 100, ORANGE_COLOR);

            DrawText("Play again", pa_text_pos_x, pa_text_pos_y, TEXT_FONT_SIZE, RAYWHITE);

            if (CheckCollisionPointRec(GetMousePosition(), pa_rect)){
                if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
                    EndDrawing();
                    goto reset;
                }
            }
        //-----------------------------------------------------------------------------------------------------

        //-Render big 2048-------------------------------------------------------------------------------------
            Vector2 _2084_text_size = MeasureTextEx(GetFontDefault(), "2048", TILE_SIZE * 1.1, TILE_SIZE / 2);

            int y = OUTER_PADDING - TILE_PADDING + RECT_HEIGHT;

            Vector2 _2084_pos = {
                    (GetScreenWidth() - _2084_text_size.x) / 2.0f,
                    y + 12 + (TOP_PADDING - y - _2084_text_size.y) / 2.0f,
            };

            DrawTextEx(GetFontDefault(), "2048", _2084_pos, TILE_SIZE * 1.1, TILE_SIZE / 2, BLACK);
        }
        //-----------------------------------------------------------------------------------------------------


        high_score = max(high_score, score);

        EndDrawing();
    }

    CloseWindow();

    return 0;
}

#pragma clang diagnostic pop

void updateBoard(Board board, Board prev_board, Direction direction, Score *score) {
    prev_board = memcpy(prev_board, board, 16 * sizeof(int));

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
    printBoard(prev_board);
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