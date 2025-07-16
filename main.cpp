#include <iostream>
#include "define.hpp"
#include "types.hpp"
#include <string.h>
#include <unistd.h>
#include "../../../../usr/include/x86_64-linux-gnu/sys/time.h"
#include <SDL2/SDL.h>

void update_board(unsigned int count[WIDTH][HEIGHT], state board[WIDTH][HEIGHT])
{
    for (int i = 1; i < HEIGHT-1; i++)
    {
        for (int j = 1; j < WIDTH-1; j++)
        {
            if (board[j][i] == dead && count[j][i] == 3)
            {
                board[j][i] = alive;
            }
            else if (board[j][i] == alive)
            {
                if (count[j][i] > 3)
                {
                    board[j][i] = dead;
                }
                else if (count[j][i] < 2)
                {
                    board[j][i] = dead;
                }
            }
        }
    }
}

void process_neibourgh(state board[WIDTH][HEIGHT], unsigned int count[WIDTH][HEIGHT])
{

    // on cols
    for (int i = 1; i < WIDTH - 1; i++)
    {

        for (int j = 1; j < HEIGHT - 1; j++)
        {
            count[i][j] =
                (unsigned int)board[i][j + 1] + (unsigned int)board[i][j - 1] + (unsigned int)board[i - 1][j] + (unsigned int)board[i + 1][j] + (unsigned int)board[i + 1][j - 1] + (unsigned int)board[i + 1][j + 1] + (unsigned int)board[i - 1][j - 1] + (unsigned int)board[i - 1][j + 1];
        }
    }
}

void disp_array(char *name, unsigned int array[WIDTH][HEIGHT])
{
    printf("%s : \n", name);
    for (int i = 0; i < HEIGHT; i++)
    {
        printf("|");

        for (int j = 0; j < WIDTH; j++)
        {
            printf(" %d |", array[j][i]);
        }
        printf("\n");
    }
    printf("\n");
}

void disp_board(char *name, state array[WIDTH][HEIGHT])
{
    printf("%s : \n", name);
    for (int i = 0; i < HEIGHT; i++)
    {
        printf("|");

        for (int j = 0; j < WIDTH; j++)
        {
            printf(" %d |", array[j][i]);
        }
        printf("\n");
    }
    printf("\n");
}




int
SDL_RenderFillCircle(SDL_Renderer * renderer, int x, int y, int radius)
{
    int offsetx, offsety, d;
    int status;

    offsetx = 0;
    offsety = radius;
    d = radius -1;
    status = 0;

    while (offsety >= offsetx) {

        status += SDL_RenderDrawLine(renderer, x - offsety, y + offsetx,
                                     x + offsety, y + offsetx);
        status += SDL_RenderDrawLine(renderer, x - offsetx, y + offsety,
                                     x + offsetx, y + offsety);
        status += SDL_RenderDrawLine(renderer, x - offsetx, y - offsety,
                                     x + offsetx, y - offsety);
        status += SDL_RenderDrawLine(renderer, x - offsety, y - offsetx,
                                     x + offsety, y - offsetx);

        if (status < 0) {
            status = -1;
            break;
        }

        if (d >= 2*offsetx) {
            d -= 2*offsetx + 1;
            offsetx +=1;
        }
        else if (d < 2 * (radius - offsety)) {
            d += 2 * offsety - 1;
            offsety -= 1;
        }
        else {
            d += 2 * (offsety - offsetx - 1);
            offsety -= 1;
            offsetx += 1;
        }
    }

    return status;
}

void draw_board(state board[WIDTH][HEIGHT], unsigned int count[WIDTH][HEIGHT], SDL_Renderer *renderer)
{


    for (int i = 0; i < HEIGHT; i++)
    {

        for (int j = 0; j < WIDTH; j++)
        {
            if (board[j][i] == alive)
            {
                switch (count[j][i])
                {
                case 2:
                    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
                    break;
                case 3:
                    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
                    break;
                default:
                    SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
                    break;
                }

                SDL_RenderFillCircle(renderer,j*RATIO+RATIO/2,i*RATIO+RATIO/2,RATIO/2-1);
            
            }
        }
    }
}

int main(int, char **)
{
    std::cout << "Hello, from gameoflife!\n";

    SDL_Init(SDL_INIT_EVERYTHING);
    SDL_Window *win;
    SDL_Renderer *renderer;

    win = SDL_CreateWindow("Game of Life", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WIDTH * RATIO, HEIGHT * RATIO, SDL_WINDOW_SHOWN);
    if (!win)
    {
        std::cout << "Error creating window: " << SDL_GetError() << std::endl;
        system("pause");
        return false;
    }

    renderer = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer)
    {
        std::cout << "Error creating renderer: " << SDL_GetError() << std::endl;
        return false;
    }



    state board[WIDTH][HEIGHT];
    unsigned int count[WIDTH][HEIGHT];

    memset(board, 0, sizeof(board));
    memset(count, 0, sizeof(count));
    timeval tv;
    gettimeofday(&tv, NULL);
    srand(tv.tv_sec);
    for (int i = 1; i < HEIGHT - 1; i++)
    {

        for (int j = 1; j < WIDTH - 1; j++)
        {
            board[j][i] = (state)(rand() % 2);
        }
    }

    //disp_board((char *)"BOARD INIT", board);

    bool cont = true;
    while (cont)
    {
        // do drawing
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        process_neibourgh(board, count);
        //disp_array((char *)"COUNT", count);
        update_board(count, board);
        //disp_board((char *)"BOARD", board);
        draw_board(board, count, renderer);
        SDL_RenderPresent(renderer);

        SDL_Delay(100);

        // Event loop
        SDL_Event ev;
        while (SDL_PollEvent(&ev) != 0)
        {
            // check event type
            switch (ev.type)
            {
            case SDL_QUIT:
                cont = false;
                break;
            case SDL_KEYDOWN:
                // test keycode
                switch (ev.key.keysym.sym)
                {
                case SDLK_ESCAPE:
                    cont = false;
                    break;
                }
                break;
            }
        }
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(win);

    renderer = NULL;
    win = NULL;
}
