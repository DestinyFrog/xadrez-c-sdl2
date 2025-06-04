#include <stdbool.h>

#include <SDL.h>

const int UNIT = 64, BORDER = 20;

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

void draw_piece(SDL_Renderer *render, SDL_Surface *sf, SDL_Rect r) {
    SDL_Texture *texture = SDL_CreateTextureFromSurface(render, sf);
    SDL_RenderCopy(render, texture, NULL, &r);
    SDL_DestroyTexture(texture);
    sf = NULL;
}

int main() {
    int width = UNIT * 8 + BORDER * 2;
    int height = UNIT * 8 + BORDER * 2;

    SDL_Init(SDL_INIT_EVERYTHING);

    SDL_Window *window = SDL_CreateWindow("CHESS", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_ALLOW_HIGHDPI);
    if (window == ((void *)0))
    {
        printf("Error creating window: %s", SDL_GetError());
        return 1;
    }

    SDL_Renderer *render = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (render == ((void *)0))
    {
        printf("Error creating renderer: %s", SDL_GetError());
        return 1;
    }

    SDL_Event window_event;

    SDL_Surface *BLACK_PAWN = SDL_LoadBMP("black_pawn.bmp");
    SDL_Surface *WHITE_PAWN = SDL_LoadBMP("white_pawn.bmp");
    SDL_Surface *BLACK_TOWER = SDL_LoadBMP("black_tower.bmp");
    SDL_Surface *WHITE_TOWER = SDL_LoadBMP("white_tower.bmp");
    SDL_Surface *BLACK_PRIEST = SDL_LoadBMP("black_priest.bmp");
    SDL_Surface *WHITE_PRIEST = SDL_LoadBMP("white_priest.bmp");
    SDL_Surface *BLACK_HORSE = SDL_LoadBMP("black_horse.bmp");
    SDL_Surface *WHITE_HORSE = SDL_LoadBMP("white_horse.bmp");
    SDL_Surface *BLACK_KING = SDL_LoadBMP("black_king.bmp");
    SDL_Surface *WHITE_KING = SDL_LoadBMP("white_king.bmp");
    SDL_Surface *BLACK_QUEEN = SDL_LoadBMP("black_queen.bmp");
    SDL_Surface *WHITE_QUEEN = SDL_LoadBMP("white_queen.bmp");
    SDL_Rect r;

    while (true) {
        SDL_WaitEvent(&window_event);
        if (SDL_QUIT == window_event.type) break;

        SDL_RenderClear( render );
        
        r.w = UNIT;
        r.h = UNIT;
        bool is_black = true;
        int piece;

        for (int i = 0; i < 8; i++) {
            is_black = !is_black;
            for (int j = 0; j < 8; j++) {
                r.x = i * UNIT + BORDER;
                r.y = j * UNIT + BORDER;
                
                if (is_black) SDL_SetRenderDrawColor(render, 137, 80, 49, 255);
                else          SDL_SetRenderDrawColor(render, 248, 222, 157, 255);
                SDL_RenderFillRect(render, &r);
                is_black = !is_black;
                
                piece = board[j][i];
                switch (piece) {
                    case -1: draw_piece( render, BLACK_TOWER, r ); break;
                    case  1: draw_piece( render, WHITE_TOWER, r ); break;
                    case -2: draw_piece( render, BLACK_PRIEST, r ); break;
                    case  2: draw_piece( render, WHITE_PRIEST, r ); break;
                    case -3: draw_piece( render, BLACK_HORSE, r ); break;
                    case  3: draw_piece( render, WHITE_HORSE, r ); break;
                    case -4: draw_piece( render, BLACK_KING, r ); break;
                    case  4: draw_piece( render, WHITE_KING, r ); break;
                    case -5: draw_piece( render, BLACK_QUEEN, r ); break;
                    case  5: draw_piece( render, WHITE_QUEEN, r ); break;
                    case -6: draw_piece( render, BLACK_PAWN, r ); break;
                    case  6: draw_piece( render, WHITE_PAWN, r ); break;
                }
            }
        }
        
        SDL_SetRenderDrawColor(render, 0, 0, 0, 255);
        SDL_RenderPresent(render);
    }

    SDL_FreeSurface(BLACK_PAWN);
    SDL_FreeSurface(WHITE_PAWN);
    SDL_FreeSurface(BLACK_TOWER);
    SDL_FreeSurface(WHITE_TOWER);
    SDL_FreeSurface(BLACK_PRIEST);
    SDL_FreeSurface(WHITE_PRIEST);
    SDL_FreeSurface(BLACK_HORSE);
    SDL_FreeSurface(WHITE_HORSE);
    SDL_FreeSurface(BLACK_KING);
    SDL_FreeSurface(WHITE_KING);
    SDL_FreeSurface(BLACK_QUEEN);
    SDL_FreeSurface(WHITE_QUEEN);

    SDL_DestroyRenderer(render);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return EXIT_SUCCESS;
}