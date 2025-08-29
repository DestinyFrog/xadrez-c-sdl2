#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include <SDL.h>

#define BOARD_SIZE 8

enum PIECE {
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

typedef struct {
    int value;
    bool mouse_down;
    bool is_black;
    bool target;
} piece;

typedef struct {
    int x, y;
} vec2;

piece board[BOARD_SIZE][BOARD_SIZE];
SDL_Texture** current_piece;

const int UNIT = 32;

bool running = true;
bool black_turn = true;
int selected_x, selected_y;

SDL_Rect r;
SDL_Texture** current_piece;

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

void start( SDL_Renderer* );
void draw( SDL_Renderer* );
void on_click( int, int );
void on_mouse_move( int, int );
void destroy();
void int_to_sprite( int );
void clear_targets();

typedef void ( *calc_pos_fn ) ( int, int );
calc_pos_fn fn_piece( short int );
void move_pawn  ( int, int );
void move_tower ( int, int );
void move_priest( int, int );
void move_horse ( int, int );
void move_king  ( int, int );
void move_queen ( int, int );

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

    start( render );

    while (running) {
        while (SDL_PollEvent(&window_event)) {
            if ( window_event.type == SDL_QUIT ) {
                running = false;
            }
        
            int window_mouse_pos_x;
            int window_mouse_pos_y;
            int window_pos_x;
            int window_pos_y;
            SDL_GetGlobalMouseState( &window_mouse_pos_x, &window_mouse_pos_y );
            SDL_GetWindowPosition( window, &window_pos_x, &window_pos_y );
            int mouse_pos_x = window_mouse_pos_x - window_pos_x;
            int mouse_pos_y = window_mouse_pos_y - window_pos_y;
            on_mouse_move( mouse_pos_x, mouse_pos_y );

            if ( window_event.type == SDL_MOUSEBUTTONDOWN ) {
                on_click( mouse_pos_x, mouse_pos_y );
            }
        }

        draw( render );
    }

    destroy();
    SDL_DestroyRenderer( render );
    SDL_DestroyWindow( window );
    SDL_Quit();

    return EXIT_SUCCESS;
}

void start( SDL_Renderer* render ) {
    short int board_num[BOARD_SIZE][BOARD_SIZE] = {
         1,  2,  3,  5,  4,  3,  2,  1,
         6,  6,  6,  6,  6,  6,  6,  6,
         0,  0,  0,  0,  0,  0,  0,  0,
         0,  0,  0,  0,  0,  0,  0,  0,
         0,  0,  0,  0,  0,  0,  0,  0,
         0,  0,  0,  0,  0,  0,  0,  0,
        -6, -6, -6, -6, -6, -6, -6, -6,
        -1, -2, -3, -4, -5, -3, -2, -1,
    };

    bool is_black = true;
    for ( int i = 0; i < BOARD_SIZE; i++ ) {
        for ( int j = 0; j < BOARD_SIZE; j++ ) {
            board[j][i] = (piece) {
                board_num[i][j],
                false,
                is_black,
                false
            };
            is_black = !is_black;
        }
        is_black = !is_black;
    }

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

void draw( SDL_Renderer* render ) {
    SDL_RenderClear( render );

    r.w = UNIT;
    r.h = UNIT;

    for ( int i = 0; i < BOARD_SIZE; i++ ) {
        for ( int j = 0; j < BOARD_SIZE; j++ ) {
            r.x = i * UNIT;
            r.y = j * UNIT;

            if ( board[i][j].is_black )
                SDL_SetRenderDrawColor( render, 137, 80, 49, 255 );
            else
                SDL_SetRenderDrawColor( render, 248, 222, 157, 255 );

            if ( selected_x == i && selected_y == j )
                SDL_SetRenderDrawColor(render, 215, 0, 0, 255);

            if ( board[i][j].mouse_down )  
                SDL_SetRenderDrawColor(render, 60, 179, 113, 255);

            if ( board[i][j].target )
                SDL_SetRenderDrawColor(render, 215, 155, 0, 255);

            SDL_RenderFillRect( render, &r );

            int_to_sprite( board[i][j].value );
            if ( current_piece != NULL )
                SDL_RenderCopy( render, *current_piece, NULL, &r );
        }
    }

    SDL_SetRenderDrawColor( render, 0, 0, 0, 255 );
    SDL_RenderPresent( render );
}

void on_mouse_move( int mouse_pos_x, int mouse_pos_y ) {
    for ( int i = 0; i < BOARD_SIZE; i++ ) {
        for ( int j = 0; j < BOARD_SIZE; j++ ) {
            board[i][j].mouse_down = false;

            if (mouse_pos_x > i * UNIT &&
                mouse_pos_x < (i + 1) * UNIT &&
                mouse_pos_y > j * UNIT &&
                mouse_pos_y < (j + 1) * UNIT)
                    board[i][j].mouse_down = true;
        }
    }
}

void clear_targets() {
    for ( int i = 0; i < BOARD_SIZE; i++ ) {
        for ( int j = 0; j < BOARD_SIZE; j++ ) {
            board[i][j].target = false;
        }
    }
}

void on_click( int mouse_pos_x, int mouse_pos_y ) {
    for ( int i = 0; i < BOARD_SIZE; i++ ) {
        for ( int j = 0; j < BOARD_SIZE; j++ ) {
            if (mouse_pos_x > i * UNIT &&
                mouse_pos_x < (i + 1) * UNIT &&
                mouse_pos_y > j * UNIT &&
                mouse_pos_y < (j + 1) * UNIT) {

                if ( board[i][j].target ) {
                    board[i][j].value = board[selected_x][selected_y].value;
                    board[selected_x][selected_y].value = EMPTY;
                    black_turn = !black_turn;
                    clear_targets();
                    return;
                }

                clear_targets();
                calc_pos_fn fn = fn_piece( board[i][j].value );
                if ( fn ) fn( i, j );
                selected_x = i;
                selected_y = j;
                return;
            }
        }
    }
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

void int_to_sprite( int piece ) {
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
}

calc_pos_fn fn_piece(short int piece) {
    if (piece < 0 && black_turn) return NULL;
    if (piece > 0 && !black_turn) return NULL;

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
            return NULL;
    }
}

void move_pawn  ( int fx, int fy ) {
    if ( board[fx][fy].value < EMPTY ) {
        if ( board[fx][fy - 1].value == EMPTY )
            board[fx][fy - 1].target = true;

        if ( board[fx][fy - 1].value == EMPTY )
            board[fx][fy - 2].target = true;

        if ( board[fx + 1][fy - 1].value != EMPTY )
            board[fx + 1][fy - 1].target = true;

        if ( board[fx - 1][fy - 1].value != EMPTY )
            board[fx - 1][fy - 1].target = true;
    }

    else if ( board[fx][fy].value > EMPTY ) {
        if ( board[fx][fy + 1].value == EMPTY )
            board[fx][fy + 1].target = true;

        if ( board[fx][fy + 1].value == EMPTY )
            board[fx][fy + 2].target = true;

        if ( board[fx + 1][fy + 1].value != EMPTY )
            board[fx + 1][fy + 1].target = true;

        if ( board[fx - 1][fy + 1].value != EMPTY )
            board[fx - 1][fy + 1].target = true;
    }
}

void move_tower ( int fx, int fy ) {
    for ( int x = fx + 1; x < BOARD_SIZE; x++ ) {
        board[x][fy].target = true;
        if ( board[x][fy].value != EMPTY )
            break;
    }

    for ( int x = fx - 1; x >= 0; x-- ) {
        board[x][fy].target = true;
        if ( board[x][fy].value != EMPTY )
            break;
    }

    for ( int y = fy + 1; y < BOARD_SIZE; y++ ) {
        board[fx][y].target = true;
        if ( board[fx][y].value != EMPTY )
            break;
    }

    for ( int y = fy - 1; y >= 0; y-- ) {
        board[fx][y].target = true;
        if ( board[fx][y].value != EMPTY )
            break;
    }
}

void move_priest( int fx, int fy ) {
    int tx, ty;

    tx = fx, ty = fy;
    tx--; ty--;
    while( tx >= 0 && ty >= 0 ) {
        board[tx][ty].target = true;
        if ( board[tx][ty].value != EMPTY )
            break;
        tx--; ty--;
    }

    tx = fx, ty = fy;
    tx++; ty--;
    while( tx < BOARD_SIZE && ty >= 0 ) {
        board[tx][ty].target = true;
        if ( board[tx][ty].value != EMPTY )
            break;
        tx++; ty--;
    }

    tx = fx, ty = fy;
    tx++; ty++;
    while( tx < BOARD_SIZE && ty < BOARD_SIZE ) {
        board[tx][ty].target = true;
        if ( board[tx][ty].value != EMPTY )
            break;
        tx++; ty++;
    }

    tx = fx, ty = fy;
    tx--; ty++;
    while( tx >= 0 && ty < BOARD_SIZE ) {
        board[tx][ty].target = true;
        if ( board[tx][ty].value != EMPTY )
            break;
        tx--; ty++;
    }
}

void move_horse ( int fx, int fy ) {
    struct { int x, y; } coo[] = {
        { -1, -2 },
        { +1, -2 },
        { -1, +2 },
        { +1, +2 },
        { -2, -1 },
        { -2, +1 },
        { +2, -1 },
        { +2, +1 }
    };

    int tx, ty;
    for ( int i = 0; i < 8; i++ ) {
        tx = fx + coo[i].x;
        ty = fy + coo[i].y;
        int value = board[tx][ty].value;
        if (
            ((board[fx][fy].value < 0 && value >= 0) || (board[fx][fy].value > 0 && value <= 0)) &&
            (tx >= 0 && ty >= 0 && tx < BOARD_SIZE && ty < BOARD_SIZE)
        )
            board[tx][ty].target = true;
    }
}

void move_king  ( int fx, int fy ) {
    struct { int x, y; } coo[] = {
        { -1, -1 },
        { -1,  0 },
        { -1,  1 },
        {  0, -1 },
        {  0,  1 },
        {  1, -1 },
        {  1,  0 },
        {  1,  1 }
    };

    int tx, ty;
    for ( int i = 0; i < 8; i++ ) {
        tx = fx + coo[i].x;
        ty = fy + coo[i].y;
        int value = board[tx][ty].value;
        if (
            ((board[fx][fy].value < 0 && value >= 0) || (board[fx][fy].value > 0 && value <= 0)) &&
            (tx >= 0 && ty >= 0 && tx < BOARD_SIZE && ty < BOARD_SIZE)
        )
            board[tx][ty].target = true;
    }
}

void move_queen ( int fx, int fy ) {
    move_priest( fx, fy );
    move_tower( fx, fy );
}