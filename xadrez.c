#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include <SDL.h>

#define BOARD_SIZE 10

typedef struct {
    int x, y;
} Vector2;

short int board[BOARD_SIZE][BOARD_SIZE] = {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 1, 2, 3, 5, 4, 3, 2, 1, 0,
    0, 6, 6, 6, 6, 6, 6, 6, 6, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0,-1, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0,-6,-6,-6,-6,-6,-6,-6,-6, 0,
    0,-1,-2,-3,-4,-5,-3,-2,-1, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0
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

bool is_destiny( Vector2*, int, int);
void start(SDL_Renderer*);
void destroy();
void draw(SDL_Renderer*);

const int   UNIT = 56,
            BORDER = UNIT;

bool running = true,
    clicked = false,
    selected = false,
    black_turn = true;

Vector2 window_mouse_pos = { 0, 0 },
    window_pos = { 0, 0 },
    mouse_pos = { 0, 0 },
    selected_pos = { 0, 0 };

int main() {
    int width = UNIT * 8 + BORDER * 2,
        height = UNIT * 8 + BORDER * 2;
        
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

short int piece;
bool is_black = true;
SDL_Texture** current_piece;
SDL_Rect r;

void draw(SDL_Renderer* render) {
    SDL_RenderClear( render );

    r.w = UNIT;
    r.h = UNIT;

    for ( int i = 0; i < BOARD_SIZE; i++ ) {
        is_black = !is_black;
        for ( int j = 0; j < BOARD_SIZE; j++ ) {

            piece = board[j][i];
            r.x = i * UNIT;
            r.y = j * UNIT;

            if ( j == 0 || i == 0 || j == BOARD_SIZE-1 || i == BOARD_SIZE-1 )
                if (black_turn) SDL_SetRenderDrawColor( render, 0, 0, 0, 255 );
                else            SDL_SetRenderDrawColor( render, 255, 255, 255, 255 );
            else if ( is_black ) SDL_SetRenderDrawColor( render, 137, 80, 49, 255 );
            else                 SDL_SetRenderDrawColor( render, 248, 222, 157, 255 );
            
            if ( selected ) {
                if ( is_destiny( &selected_pos, j, i ) && (board[selected_pos.x][selected_pos.y] < 0) == black_turn )
                    SDL_SetRenderDrawColor(render, 215, 155, 0, 255);

                if ( j == selected_pos.x && i == selected_pos.y )
                    SDL_SetRenderDrawColor(render, 215, 0, 0, 255);
            }

            if (mouse_pos.x > r.x &&
                mouse_pos.x < r.x + r.w &&
                mouse_pos.y > r.y &&
                mouse_pos.y < r.y + r.h) {
                    SDL_SetRenderDrawColor(render, 60, 179, 113, 255);
                    
                    if (clicked)
                        if (selected && is_destiny( &selected_pos, j, i ) && (board[selected_pos.x][selected_pos.y] < 0) == black_turn) {
                            SDL_SetRenderDrawColor(render, 215, 155, 0, 255);
                            
                            selected = false;
                            black_turn = !black_turn;

                            board[j][i] = board[selected_pos.x][selected_pos.y];
                            board[selected_pos.x][selected_pos.y] = EMPTY;
                        }
                        else {
                            selected = true;
                            selected_pos.x = j;
                            selected_pos.y = i;
                        }
            }

            SDL_RenderFillRect(render, &r);
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
    }

    SDL_SetRenderDrawColor( render, 0, 0, 0, 255 );
    SDL_RenderPresent( render );
}

bool is_destiny( Vector2* selected_pos, int x, int y ) {
    if ( x == 0 || y == 0 || x == BOARD_SIZE-1 || y == BOARD_SIZE-1 )
        return false;
        
    int piece = board[selected_pos->x][selected_pos->y];
    int distance_from_piece_x = x - selected_pos->x;

    switch( piece ) {
        case WHITE_TOWER:
        case BLACK_TOWER:
            bool next = false;
            if ( x == selected_pos->x ) {
                if ( selected_pos->y < y ) {
                    for ( int i = selected_pos->y; i > y; i-- ) {
                        if ( board[x][i] > EMPTY ) next = true;
                    }
                    return next;
                }
                else if ( selected_pos->y > y ) {
                    for ( int i = selected_pos->y; i < y; i++ ) {
                        if ( board[x][i] > EMPTY ) next = true;
                    }
                    return next;
                }
            }
            else if ( y == selected_pos->y ) {
                if ( selected_pos->x < x ) {
                    for ( int i = selected_pos->x; i > x; i-- ) {
                        if ( board[i][y] > EMPTY ) next = true;
                    }
                    return next;
                }
                else if ( selected_pos->x > x ) {
                    for ( int i = selected_pos->x; i < x; i++ ) {
                        if ( board[i][y] > EMPTY ) next = true;
                    }
                    return next;
                }
            }
            return false;

        case WHITE_PRIEST:
        case BLACK_PRIEST: return
                abs(x - selected_pos->x) == abs(y - selected_pos->y);

        case WHITE_HORSE:
        case BLACK_HORSE: return
                (y == selected_pos->y - 1 || y == selected_pos->y + 1) && (x == selected_pos->x + 2 || x == selected_pos->x - 2) ||
                (y == selected_pos->y - 2 || y == selected_pos->y + 2) && (x == selected_pos->x + 1 || x == selected_pos->x - 1);
                
        case WHITE_KING: return
                abs(y - selected_pos->y) <= 1 &&
                abs(x - selected_pos->x) <= 1 &&
                board[x][y] < 0;

        case BLACK_KING: return
                abs(y - selected_pos->y) <= 1 &&
                abs(x - selected_pos->x) <= 1 &&
                board[x][y] > 0;

        case WHITE_QUEEN: return
                abs(x - selected_pos->x) == abs(y - selected_pos->y) ||
                y == selected_pos->y || x == selected_pos->x &&
                board[x][y] < 0;

        case BLACK_QUEEN: return
                abs(x - selected_pos->x) == abs(y - selected_pos->y) ||
                y == selected_pos->y || x == selected_pos->x &&
                board[x][y] > 0;

        case WHITE_PAWN: return
            (y == selected_pos->y && board[x][y] == EMPTY && distance_from_piece_x <= 2 && distance_from_piece_x > 0) ||
            (y == selected_pos->y -1 || y == selected_pos->y +1)
                && x == selected_pos->x + 1
                && board[x][y] < 0;
                
        case BLACK_PAWN: return
            (y == selected_pos->y && board[x][y] == EMPTY && distance_from_piece_x >= -2 && distance_from_piece_x < 0) ||
            (y == selected_pos->y -1 || y == selected_pos->y +1)
                && x == selected_pos->x - 1
                && board[x][y] > 0;

        case EMPTY: return false;
    }
}
