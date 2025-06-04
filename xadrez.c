#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include <SDL.h>

int board[8][8] = {
     1, 2, 3, 5, 4, 3, 2, 1,
     6, 6, 6, 6, 6, 6, 6, 6,
     0, 0, 0, 0, 0, 0, 0, 0,
     0, 0, 0, 0, 0, 0, 0, 0,
     0, 0, 0, 0, 0, 0, 0, 0,
     0, 0, 0, 0, 0, 0, 0, 0,
    -6,-6,-6,-6,-6,-6,-6,-6,
    -1,-2,-3,-4,-5,-3,-2,-1   
};

int main() {
    const int UNIT = 32, BORDER = 20,
        width = UNIT * 8 + BORDER * 2,
        height = UNIT * 8 + BORDER * 2;
        
    SDL_Event window_event;
    SDL_Rect r;
    SDL_Window *window;
    SDL_Renderer *render;
        
    bool running = true,
        clicked = false,
        selected = false;

    int window_mouse_pos_x,
        window_mouse_pos_y,
        window_pos_x,
        window_pos_y,
        mouse_pos_x,
        mouse_pos_y,
        selected_x,
        selected_y;

    SDL_Init(SDL_INIT_EVERYTHING);

    window = SDL_CreateWindow("CHESS", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_ALLOW_HIGHDPI);
    if (window == ((void *)0))
    {
        printf("Error creating window: %s", SDL_GetError());
        return 1;
    }

    render = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (render == ((void *)0))
    {
        printf("Error creating renderer: %s", SDL_GetError());
        return 1;
    }

    SDL_Surface *S_BLACK_PAWN     = SDL_LoadBMP("black_pawn.bmp");     
    SDL_Surface *S_WHITE_PAWN     = SDL_LoadBMP("white_pawn.bmp");
    SDL_Surface *S_BLACK_TOWER    = SDL_LoadBMP("black_tower.bmp");
    SDL_Surface *S_WHITE_TOWER    = SDL_LoadBMP("white_tower.bmp");
    SDL_Surface *S_BLACK_PRIEST   = SDL_LoadBMP("black_priest.bmp");
    SDL_Surface *S_WHITE_PRIEST   = SDL_LoadBMP("white_priest.bmp");
    SDL_Surface *S_BLACK_HORSE    = SDL_LoadBMP("black_horse.bmp");
    SDL_Surface *S_WHITE_HORSE    = SDL_LoadBMP("white_horse.bmp");
    SDL_Surface *S_BLACK_KING     = SDL_LoadBMP("black_king.bmp");
    SDL_Surface *S_WHITE_KING     = SDL_LoadBMP("white_king.bmp");
    SDL_Surface *S_BLACK_QUEEN    = SDL_LoadBMP("black_queen.bmp");
    SDL_Surface *S_WHITE_QUEEN    = SDL_LoadBMP("white_queen.bmp");

    SDL_Texture *BLACK_PAWN     = SDL_CreateTextureFromSurface(render, S_BLACK_PAWN);     
    SDL_Texture *WHITE_PAWN     = SDL_CreateTextureFromSurface(render, S_WHITE_PAWN);
    SDL_Texture *BLACK_TOWER    = SDL_CreateTextureFromSurface(render, S_BLACK_TOWER);
    SDL_Texture *WHITE_TOWER    = SDL_CreateTextureFromSurface(render, S_WHITE_TOWER);
    SDL_Texture *BLACK_PRIEST   = SDL_CreateTextureFromSurface(render, S_BLACK_PRIEST);
    SDL_Texture *WHITE_PRIEST   = SDL_CreateTextureFromSurface(render, S_WHITE_PRIEST);
    SDL_Texture *BLACK_HORSE    = SDL_CreateTextureFromSurface(render, S_BLACK_HORSE);
    SDL_Texture *WHITE_HORSE    = SDL_CreateTextureFromSurface(render, S_WHITE_HORSE);
    SDL_Texture *BLACK_KING     = SDL_CreateTextureFromSurface(render, S_BLACK_KING);
    SDL_Texture *WHITE_KING     = SDL_CreateTextureFromSurface(render, S_WHITE_KING);
    SDL_Texture *BLACK_QUEEN    = SDL_CreateTextureFromSurface(render, S_BLACK_QUEEN);
    SDL_Texture *WHITE_QUEEN    = SDL_CreateTextureFromSurface(render, S_WHITE_QUEEN);
        
    SDL_FreeSurface(S_BLACK_PAWN);
    SDL_FreeSurface(S_WHITE_PAWN);
    SDL_FreeSurface(S_BLACK_TOWER);
    SDL_FreeSurface(S_WHITE_TOWER);
    SDL_FreeSurface(S_BLACK_PRIEST);
    SDL_FreeSurface(S_WHITE_PRIEST);
    SDL_FreeSurface(S_BLACK_HORSE);
    SDL_FreeSurface(S_WHITE_HORSE);
    SDL_FreeSurface(S_BLACK_KING);
    SDL_FreeSurface(S_WHITE_KING);
    SDL_FreeSurface(S_BLACK_QUEEN);
    SDL_FreeSurface(S_WHITE_QUEEN);

    while (running) {
        clicked = false;

        while (SDL_PollEvent(&window_event))
            switch (window_event.type) {
                case SDL_QUIT: running = false; break;
                case SDL_MOUSEBUTTONDOWN: clicked = true; break;
            }

        SDL_GetGlobalMouseState(&window_mouse_pos_x, &window_mouse_pos_y);
        SDL_GetWindowPosition(window, &window_pos_x, &window_pos_y);
        mouse_pos_x = window_mouse_pos_x - window_pos_x;
        mouse_pos_y = window_mouse_pos_y - window_pos_y;

        SDL_RenderClear( render );
        
        r.w = UNIT;
        r.h = UNIT;
        bool is_black = true;
        int piece;
        SDL_Texture** current_piece;

        for (int i = 0; i < 8; i++) {
            is_black = !is_black;
            for (int j = 0; j < 8; j++) {

                piece = board[j][i];
                r.x = i * UNIT + BORDER;
                r.y = j * UNIT + BORDER;
                 
                if ( is_black )  SDL_SetRenderDrawColor(render, 137, 80, 49, 255);
                else                SDL_SetRenderDrawColor(render, 248, 222, 157, 255);
                
                if ( selected ) {
                    if ( j == selected_x && i == selected_y ) 
                        SDL_SetRenderDrawColor(render, 215, 0, 0, 255);
                        
                    
                }

                if (mouse_pos_x > r.x &&
                    mouse_pos_x < r.x + r.w &&
                    mouse_pos_y > r.y &&
                    mouse_pos_y < r.y + r.h) {
                        SDL_SetRenderDrawColor(render, 60, 179, 113, 255);
                        
                        if (clicked) {
                            if (selected) {

                                    selected = false;
                                    board[j][i] = board[selected_x][selected_y];
                                    board[selected_x][selected_y] = 0;
                            }
                            else if (piece != 0) {
                                selected = true;
                                selected_x = j;
                                selected_y = i;
                            }
                        }
                }

                SDL_RenderFillRect(render, &r);
                is_black = !is_black;
                
                switch (piece) {
                    case -1: current_piece = &BLACK_TOWER;   break;  
                    case  1: current_piece = &WHITE_TOWER;   break;
                    case -2: current_piece = &BLACK_PRIEST;  break;
                    case  2: current_piece = &WHITE_PRIEST;  break;
                    case -3: current_piece = &BLACK_HORSE;   break;
                    case  3: current_piece = &WHITE_HORSE;   break;
                    case -4: current_piece = &BLACK_KING;    break;
                    case  4: current_piece = &WHITE_KING;    break;
                    case -5: current_piece = &BLACK_QUEEN;   break;
                    case  5: current_piece = &WHITE_QUEEN;   break;
                    case -6: current_piece = &BLACK_PAWN;    break;
                    case  6: current_piece = &WHITE_PAWN;    break;
                    default: current_piece = NULL;
                }

                if ( current_piece != NULL )
                    SDL_RenderCopy(render, *current_piece, NULL, &r);
            }
        }
        
        SDL_SetRenderDrawColor(render, 0, 0, 0, 255);
        SDL_RenderPresent(render);
    }
        
    SDL_DestroyTexture(BLACK_PAWN);
    SDL_DestroyTexture(WHITE_PAWN);
    SDL_DestroyTexture(BLACK_TOWER);
    SDL_DestroyTexture(WHITE_TOWER);
    SDL_DestroyTexture(BLACK_PRIEST);
    SDL_DestroyTexture(WHITE_PRIEST);
    SDL_DestroyTexture(BLACK_HORSE);
    SDL_DestroyTexture(WHITE_HORSE);
    SDL_DestroyTexture(BLACK_KING);
    SDL_DestroyTexture(WHITE_KING);
    SDL_DestroyTexture(BLACK_QUEEN);
    SDL_DestroyTexture(WHITE_QUEEN);

    SDL_DestroyRenderer(render);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return EXIT_SUCCESS;
}