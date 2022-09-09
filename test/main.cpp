#include <SDL.h>
#include <cstdlib>
#include <iostream>
#include <vector>

#include <iostream>
#include <Windows.h>
#include <cstdio>

#include "main.h"
#include "Maze.h"

int main(int argc, char* args[])
{
    switch (argc) {
        case 1:
            break;
        case 4:
            WINDOW_WIDTH = atoi(args[3]);
        case 3:
            SHUFFLE = atoi(args[2]);
        case 2:
            MAPSIZE = atoi(args[1]);
            break;
    }

    CELLSIZE = floor(WINDOW_WIDTH / MAPSIZE);

    SDL_Init(SDL_INIT_VIDEO);
    SDL_CreateWindowAndRenderer(CELLSIZE * MAPSIZE, CELLSIZE * MAPSIZE, 0, &window, &renderer);

    SDL_SetWindowTitle(window, "majs generat");

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    SDL_RenderPresent(renderer);

    srand(clock());

    printf("velkom to ris profferisonal console majs genreator!\n\n");
    
    printf("butons thet ar god to knwos\n");
    printf("(you pressi pressi in windowso niet konesl)\n\n");

    printf("r: generate\n");
    printf("spesi: save generation\n");
    printf("q: abort\n\n");

    printf("wants more specliaes? use lunch options! example:\n");
    printf("majs.exe <mapsize> <shuffle> <window size>\n\n");

    Maze maze(MAPSIZE, SHUFFLE, CELLSIZE, renderer);

    //Uint32 last = SDL_GetTicks();

    bool shouldExit = false;
    bool shouldRender = false;
    while (!shouldExit)
    {
        if (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                break;
            }

            if (event.type == SDL_KEYDOWN) {
                switch (event.key.keysym.sym) {
                    case SDLK_r:
                        maze.Reset(&shouldRender);
                        break;
                    case SDLK_q:
                        shouldExit = true;
                        break;
                    case SDLK_c:
                        //update config

                        if (shouldRender) continue;

                        printf("\n- - Entered configuration mode - -\n");

                        printf("new mapsize: ");
                        std::cin >> MAPSIZE;

                        printf("new shuffle: ");
                        std::cin >> SHUFFLE;

                        CELLSIZE = floor(WINDOW_WIDTH / MAPSIZE);

                        maze.UpdateMaze(MAPSIZE, SHUFFLE, CELLSIZE);

                        break;

                    case SDLK_s:
                        //update config

                        if(!shouldRender) maze.Solve();

                        break;
                }
            }
        }

        //if (SDL_GetTicks() < last + 1) continue;
        if (!shouldRender) continue;

        maze.Render(&shouldRender);
        //last = SDL_GetTicks();
    }

    SDL_DestroyWindow(window);
    SDL_Quit();

    return EXIT_SUCCESS;
}