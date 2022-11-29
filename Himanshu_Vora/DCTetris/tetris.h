#ifndef TETRIS_H
#define TETRIS_H 

#define BLOCK (*(gameState.current_block))
enum block_t {L, I, T, S, Z, O, J};

struct game {
    char board[20][10];
    int (* current_block)[4][2];
    enum block_t block_type;
    int block_x;
    int block_y;
    unsigned int keys;
    float drop_timer;
    int color; 
    int block_selected;
};

struct game gameState; 

int game_init();
void choose_block();
void draw_board();
int drop();
void move_h(int);
void rotate_block(int);
void clear_row();

#endif