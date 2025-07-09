#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include <SDL.h>

#define BOARD_SIZE 8

typedef struct {
    int x, y;
} Vector2;

short int board[BOARD_SIZE][BOARD_SIZE] = {
     1, 2, 3, 5, 4, 3, 2, 1,
     6, 6, 6, 6, 6, 6, 6, 6,
     0, 0, 0, 0, 0, 0, 0, 0,
     0, 0, 0, 0, 0, 0, 0, 0,
     0, 0, 0, 0, 0, 0, 0, 0,
     0, 0, 0, 0, 0, 0, 0, 0,
    -6,-6,-6,-6,-6,-6,-6,-6,
    -1,-2,-3,-4,-5,-3,-2,-1,
};

enum pieces {
    WHITE_TOWER = 1,
    BLACK_TOWER = -1,

    WHITE_PRIEST = 2,
    BLACK_PRIEST = -2,

    WHITE_HORSE = 3,
    BLACK_HORSE = -3,

    WHITE_KING = 4,
    BLACK_KING = -4,
        
    WHITE_QUEEN = 5,
    BLACK_QUEEN = -5,

    WHITE_PAWN = 6,
    BLACK_PAWN = -6,

    EMPTY = 0
};

const int UNIT = 32;

bool running = true,
     clicked = false,
     selected = false,
     black_turn = true;

Vector2 window_mouse_pos = { 0, 0 },
    window_pos = { 0, 0 },
    mouse_pos = { 0, 0 },
    selected_pos = { 0, 0 };

void start(SDL_Renderer*);
void draw(SDL_Renderer*);
void update();
void destroy();

typedef bool (*calc_pos_fn)(int, int);
calc_pos_fn fn_piece(short int);

bool move_pawn(int, int);
bool move_tower(int, int);
bool move_priest(int, int);
bool move_horse(int, int);
bool move_king(int, int);
bool move_queen(int, int);
bool move_empty(int, int);

int main() {
    int width = UNIT * BOARD_SIZE,
        height = UNIT * BOARD_SIZE;
        
    SDL_Window *window;
    SDL_Renderer *render;
    SDL_Event window_event;

    SDL_Init(SDL_INIT_EVERYTHING);

    window = SDL_CreateWindow("CHESS", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_ALLOW_HIGHDPI);
    if (window == ((void *)0)) {
        printf("Error creating window: %s", SDL_GetError());
        return 1;
    }

    render = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (render == ((void *)0)) {
        printf("Error creating renderer: %s", SDL_GetError());
        return 1;
    }

    start(render);

    draw(render);
    while (running) {
        clicked = false;

        while (SDL_PollEvent(&window_event))
            switch (window_event.type) {
                case SDL_QUIT: running = false; break;
                case SDL_MOUSEBUTTONDOWN: clicked = true; break;
            }

        SDL_GetGlobalMouseState(&window_mouse_pos.x, &window_mouse_pos.y);
        SDL_GetWindowPosition(window, &window_pos.x, &window_pos.y);
        mouse_pos.x = window_mouse_pos.x - window_pos.x;
        mouse_pos.y = window_mouse_pos.y - window_pos.y;

        if (clicked) {
            update();
        }
        draw(render);
    }

    destroy();
    SDL_DestroyRenderer(render);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return EXIT_SUCCESS;
}

SDL_Texture *L_BLACK_PAWN;
SDL_Texture *L_WHITE_PAWN;
SDL_Texture *L_BLACK_TOWER;
SDL_Texture *L_WHITE_TOWER;
SDL_Texture *L_BLACK_PRIEST;
SDL_Texture *L_WHITE_PRIEST;
SDL_Texture *L_BLACK_HORSE;
SDL_Texture *L_WHITE_HORSE;
SDL_Texture *L_BLACK_KING;
SDL_Texture *L_WHITE_KING;
SDL_Texture *L_BLACK_QUEEN;
SDL_Texture *L_WHITE_QUEEN;

void start(SDL_Renderer* render) {
    SDL_Surface *S_BLACK_PAWN     = SDL_LoadBMP( "black_pawn.bmp" );     
    SDL_Surface *S_WHITE_PAWN     = SDL_LoadBMP( "white_pawn.bmp" );
    SDL_Surface *S_BLACK_TOWER    = SDL_LoadBMP( "black_tower.bmp" );
    SDL_Surface *S_WHITE_TOWER    = SDL_LoadBMP( "white_tower.bmp" );
    SDL_Surface *S_BLACK_PRIEST   = SDL_LoadBMP( "black_priest.bmp" );
    SDL_Surface *S_WHITE_PRIEST   = SDL_LoadBMP( "white_priest.bmp" );
    SDL_Surface *S_BLACK_HORSE    = SDL_LoadBMP( "black_horse.bmp" );
    SDL_Surface *S_WHITE_HORSE    = SDL_LoadBMP( "white_horse.bmp" );
    SDL_Surface *S_BLACK_KING     = SDL_LoadBMP( "black_king.bmp" );
    SDL_Surface *S_WHITE_KING     = SDL_LoadBMP( "white_king.bmp" );
    SDL_Surface *S_BLACK_QUEEN    = SDL_LoadBMP( "black_queen.bmp" );
    SDL_Surface *S_WHITE_QUEEN    = SDL_LoadBMP( "white_queen.bmp" );

    L_BLACK_PAWN     = SDL_CreateTextureFromSurface( render, S_BLACK_PAWN );     
    L_WHITE_PAWN     = SDL_CreateTextureFromSurface( render, S_WHITE_PAWN );
    L_BLACK_TOWER    = SDL_CreateTextureFromSurface( render, S_BLACK_TOWER );
    L_WHITE_TOWER    = SDL_CreateTextureFromSurface( render, S_WHITE_TOWER );
    L_BLACK_PRIEST   = SDL_CreateTextureFromSurface( render, S_BLACK_PRIEST );
    L_WHITE_PRIEST   = SDL_CreateTextureFromSurface( render, S_WHITE_PRIEST );
    L_BLACK_HORSE    = SDL_CreateTextureFromSurface( render, S_BLACK_HORSE );
    L_WHITE_HORSE    = SDL_CreateTextureFromSurface( render, S_WHITE_HORSE );
    L_BLACK_KING     = SDL_CreateTextureFromSurface( render, S_BLACK_KING );
    L_WHITE_KING     = SDL_CreateTextureFromSurface( render, S_WHITE_KING );
    L_BLACK_QUEEN    = SDL_CreateTextureFromSurface( render, S_BLACK_QUEEN );
    L_WHITE_QUEEN    = SDL_CreateTextureFromSurface( render, S_WHITE_QUEEN );
        
    SDL_FreeSurface( S_BLACK_PAWN );
    SDL_FreeSurface( S_WHITE_PAWN );
    SDL_FreeSurface( S_BLACK_TOWER );
    SDL_FreeSurface( S_WHITE_TOWER );
    SDL_FreeSurface( S_BLACK_PRIEST );
    SDL_FreeSurface( S_WHITE_PRIEST );
    SDL_FreeSurface( S_BLACK_HORSE );
    SDL_FreeSurface( S_WHITE_HORSE );
    SDL_FreeSurface( S_BLACK_KING );
    SDL_FreeSurface( S_WHITE_KING );
    SDL_FreeSurface( S_BLACK_QUEEN );
    SDL_FreeSurface( S_WHITE_QUEEN );
}

void destroy() {
    SDL_DestroyTexture( L_BLACK_PAWN );
    SDL_DestroyTexture( L_WHITE_PAWN );
    SDL_DestroyTexture( L_BLACK_TOWER );
    SDL_DestroyTexture( L_WHITE_TOWER );
    SDL_DestroyTexture( L_BLACK_PRIEST );
    SDL_DestroyTexture( L_WHITE_PRIEST );
    SDL_DestroyTexture( L_BLACK_HORSE );
    SDL_DestroyTexture( L_WHITE_HORSE );
    SDL_DestroyTexture( L_BLACK_KING );
    SDL_DestroyTexture( L_WHITE_KING );
    SDL_DestroyTexture( L_BLACK_QUEEN );
    SDL_DestroyTexture( L_WHITE_QUEEN );
}

SDL_Texture** current_piece;
bool is_black = true;
SDL_Rect r;

calc_pos_fn current_fn;

void update() {
    for ( int i = 0; i < BOARD_SIZE; i++ ) {
        for ( int j = 0; j < BOARD_SIZE; j++ ) {
            if (mouse_pos.x > i * UNIT &&
                mouse_pos.x < (i + 1) * UNIT &&
                mouse_pos.y > j * UNIT &&
                mouse_pos.y < (j + 1) * UNIT) {
                if (selected)            
                    selected = false;
                else {
                    selected = true;
                    selected_pos.x = j;
                    selected_pos.y = i;
                    current_fn = fn_piece(board[j][i]);
                }
            }

            if (selected) {
                if ( current_fn(i, j) ) {
                    if (board[i][j] > 0)
                        board[i][j] += 7;

                    if (board[i][j] < 0)
                        board[i][j] += -7;
                }
                else {
                    if (board[i][j] > 0)
                        board[i][j] -= 7;

                    if (board[i][j] < 0)
                        board[i][j] -= -7;
                }
            }
        }
    }
}

void draw(SDL_Renderer* render) {
    SDL_RenderClear( render );

    r.w = UNIT;
    r.h = UNIT;

    for ( int i = 0; i < BOARD_SIZE; i++ ) {
        for ( int j = 0; j < BOARD_SIZE; j++ ) {
            short int piece = board[j][i];
            r.x = i * UNIT;
            r.y = j * UNIT;

            if ( is_black ) SDL_SetRenderDrawColor( render, 137, 80, 49, 255 );
            else            SDL_SetRenderDrawColor( render, 248, 222, 157, 255 );
            
            if (piece > 6) {
                piece -= 7;
                SDL_SetRenderDrawColor(render, 215, 155, 0, 255);
            }
            
            if (piece < -6) {
                piece -= -7;
                SDL_SetRenderDrawColor(render, 215, 155, 0, 255);
            }

            if ( selected && j == selected_pos.x && i == selected_pos.y )
                SDL_SetRenderDrawColor(render, 215, 0, 0, 255);

            if (mouse_pos.x > r.x &&
                mouse_pos.x < r.x + r.w &&
                mouse_pos.y > r.y &&
                mouse_pos.y < r.y + r.h)
                    SDL_SetRenderDrawColor(render, 60, 179, 113, 255);
            
            SDL_RenderFillRect( render, &r );
            is_black = !is_black;

            switch (piece) {
                case -1: current_piece = &L_BLACK_TOWER;   break;  
                case  1: current_piece = &L_WHITE_TOWER;   break;
                case -2: current_piece = &L_BLACK_PRIEST;  break;
                case  2: current_piece = &L_WHITE_PRIEST;  break;
                case -3: current_piece = &L_BLACK_HORSE;   break;
                case  3: current_piece = &L_WHITE_HORSE;   break;
                case -4: current_piece = &L_BLACK_KING;    break;
                case  4: current_piece = &L_WHITE_KING;    break;
                case -5: current_piece = &L_BLACK_QUEEN;   break;
                case  5: current_piece = &L_WHITE_QUEEN;   break;
                case -6: current_piece = &L_BLACK_PAWN;    break;
                case  6: current_piece = &L_WHITE_PAWN;    break;
                default: current_piece = NULL;
            }

            if ( current_piece != NULL )
                SDL_RenderCopy( render, *current_piece, NULL, &r );
        }
        is_black = !is_black;
    }

    SDL_SetRenderDrawColor( render, 0, 0, 0, 255 );
    SDL_RenderPresent( render );
}

calc_pos_fn fn_piece(short int piece) {
    switch (piece) {
        case WHITE_PAWN:
        case BLACK_PAWN: 
            return move_pawn;
        case WHITE_TOWER:
        case BLACK_TOWER: 
            return move_tower;
        case WHITE_PRIEST:
        case BLACK_PRIEST: 
            return move_priest;
        case WHITE_HORSE:
        case BLACK_HORSE: 
            return move_horse;
        case WHITE_KING:
        case BLACK_KING: 
            return move_king;
        case WHITE_QUEEN:
        case BLACK_QUEEN: 
            return move_queen;
        default:
            return move_empty;
    }
}

bool move_pawn(int x, int y) {
    return false;
}

bool move_tower(int x, int y) {
    return false;
}

bool move_priest(int x, int y) {
    printf("(%d %d) - (%d %d) = (%d %d)\n", selected_pos.x, selected_pos.y, x, y, x - selected_pos.x, y - selected_pos.y);
    return (selected_pos.x - x == selected_pos.y - y);
}

bool move_horse(int x, int y) {
    return false;
}

bool move_king(int x, int y) {
    return false;
}

bool move_queen(int x, int y) {
    return false;
}

bool move_empty(int x, int y) {
    return false;
}
