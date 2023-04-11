#include <stdio.h>
#include <kos.h>
#include <png/png.h>
#include <GL/gl.h>
#include <GL/glut.h>
#include "png_draw.h"
#include "tetris.h"
#include <stdlib.h>

extern struct game gameState; 
struct texture block_texture; 
struct texture orange; 
struct texture * colors[] = {&block_texture, &orange};

int L_block[4][2] = {
    {0,1},
    {1,1},
    {2,1},
    {2,2}
};


int J_block[4][2] = {
    {0,1},
    {1,1},
    {2,0},
    {2,1}
};


int S_block[4][2] = {
    {0,1},
    {0,2},
    {1,0},
    {1,1}
};


int Z_block[4][2] = {
    {0,0},
    {0,1},
    {1,1},
    {1,2}
};

int I_block[4][2] = {
    {1,0},
    {1,1},
    {1,2},
    {1,3}
};


int T_block[4][2] = {
    {0,1},
    {1,0},
    {1,1},
    {1,2}
};


int O_block[4][2] = {
    {0,0},
    {0,1},
    {1,0},
    {1,1}
};


int game_init() {
    if (png_to_gl_texture(&block_texture, "/rd/red_block.png")) {
        printf("Cannot load texture");
        return 1; 
    }

    if (png_to_gl_texture(&orange, "/rd/orange.png")) {
        printf("Cannot load texture");
        return 1; 
    }

    memset(gameState.board, 0, 20*10);
    /*for (int i = 0; i < 20; i++) {
        for (int j = 0; j < 10; j++) {
            if (i%2 == 0 && j%2 == 0) {
                gameState.board[i][j] = 1;
            } else if (i%2 == 0 && j%2 == 0) {
                gameState.board[i][j] = 1;
            } else {
                gameState.board[i][j] = 0;
            }
        }
    }*/
    gameState.block_selected = 0;
    gameState.drop_timer = 0;
    gameState.color = 1; 
    srand(rtc_boot_time());
    return 0;
}

void choose_block() {
    gameState.block_type = rand()%7;
    gameState.color = rand()%2 + 1;
    switch (gameState.block_type) {
        case L:
            gameState.current_block = &L_block;
            break;

        case I:
            gameState.current_block = &I_block;
            break;

        case T:
            gameState.current_block = &T_block;
            break;

        case S:
            gameState.current_block = &S_block;
            break;

        case Z:
            gameState.current_block = &Z_block;
            break;

        case O:
            gameState.current_block = &O_block;
            break;

        case J:
            gameState.current_block = &J_block;
            break;
    }
    gameState.block_x = 3;
    gameState.block_y = 18;
    gameState.block_selected = 1; 
    gameState.drop_timer = 3;
}


void draw_board() {
    for (int i = 0; i < 4; i++) {
        gameState.board[gameState.block_y - BLOCK[i][0]][gameState.block_x + BLOCK[i][1]] = gameState.color;
    }

    for (int i = 0; i < 20; i++) {
        for (int j = 0; j < 10; j++) {
            if (gameState.board[i][j] != 0) {
                draw_textured_quad(colors[gameState.board[i][j] - 1] , j * 16, i * 16);
            }
        }
    }

    /*remove for next frame if block stil moving*/
    if (gameState.block_selected) {
        for (int i = 0; i < 4; i++) {
            gameState.board[gameState.block_y - BLOCK[i][0]][gameState.block_x + BLOCK[i][1]] = 0;
        }
    }
}

int drop() {
    for (int i = 0; i < 4; i++) {
        if (gameState.board[gameState.block_y - BLOCK[i][0] - 1][gameState.block_x + BLOCK[i][1]] != 0 ||
            gameState.block_y-BLOCK[i][0] - 1 < 0) {
            printf("collide\n");
            gameState.block_selected = 0;
            return 1;
        }
    }
    if (gameState.drop_timer > 0) {
        gameState.drop_timer--;
    } else {
        gameState.block_y -= 1;
        gameState.drop_timer = 3;
    }
    return 0; 
}

void move_h(int left) {
    if (left) {
        for (int i = 0; i < 4; i++) {
            if (gameState.board[gameState.block_y - BLOCK[i][0]][gameState.block_x + BLOCK[i][1] - 1] != 0 ||
                gameState.block_x+BLOCK[i][1] - 1 < 0) {
                return;
            }
        }
        gameState.block_x -= 1;
    } else {
        for (int i = 0; i < 4; i++) {
            if (gameState.board[gameState.block_y + BLOCK[i][0]][gameState.block_x + BLOCK[i][1] + 1] != 0 ||
                gameState.block_x+BLOCK[i][1] + 1 >= 10) {
                return;
            }
        }
        gameState.block_x += 1;
    }
}

void rotate_block(int left) {
    if (gameState.block_type == O) return;
    int temp_block[4][2];
    memcpy(temp_block, BLOCK, sizeof(int) * 8);

    if (left) {
        for (int point = 0; point < 4; point++) {
            int x = temp_block[point][0];
            temp_block[point][0] = temp_block[point][1];
            temp_block[point][1] = x;

            temp_block[point][0] -= 1;
            temp_block[point][0] *= -1;
            temp_block[point][0] += 1;

            if (gameState.board[gameState.block_y - temp_block[point][0]][gameState.block_x + temp_block[point][1]] != 0 ||
                gameState.block_y - temp_block[point][0] < 0 || gameState.block_x + temp_block[point][1] < 0 || gameState.block_x + temp_block[point][1] >= 10) return;
        }
    } else {
        for (int point = 0; point < 4; point++) {
            int x = temp_block[point][0];
            temp_block[point][0] = temp_block[point][1];
            temp_block[point][1] = x;

            temp_block[point][1] -= 1;
            temp_block[point][1] *= -1;
            temp_block[point][1] += 1;

            if (gameState.board[gameState.block_y - temp_block[point][0]][gameState.block_x + temp_block[point][1]] != 0 ||
                gameState.block_y - temp_block[point][0] < 0 || gameState.block_x + temp_block[point][1] < 0 || gameState.block_x + temp_block[point][1] >= 10) return;
        }
    }

    memcpy(BLOCK, temp_block, 8 * sizeof(int));
}

void clear_row() {
    /*for (int i = 0; i < 20; i++) {
        char boolean = 1;
        for (int j = 0; j < 10; j++) {
            if (gameState.board[i][j] == 1) {
                boolean = 0;
                break;
            }
        }
        if (!boolean) continue;
        //char tempBoard[20][10];
        if (i < 19) {
            memcpy(gameState.board[i], gameState.board[i+1], 10 * (19-i) * sizeof(char));
        } 
        memset(gameState.board[19], 0, 10 * sizeof(char));
    }*/
    for (int i = 19; i >= 0; i--) {
        char boolean = 1; 
        for (int j = 0; j < 10; j++) {
            if (gameState.board[i][j] == 0) {
                boolean = 0;
                break;
            }
        }
        if (!boolean) continue; 
        if (i != 19) memcpy(gameState.board[i], gameState.board[i+1], (200 - (10 * i)) * sizeof(char));
        memset(gameState.board[19], 0, 10 * sizeof(char));
    }
}