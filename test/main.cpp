#include <SDL.h>
#include <cstdlib>
#include <iostream>
#include <vector>

#include "main.h"

int MAPSIZE = 200;
int SHUFFLE = 20;
int WINDOW_WIDTH = 800;

int cellSize;

SDL_Rect* rect;
SDL_Renderer* renderer;

std::vector<Cell*> stack;

Cell* CellArray;
Cell* currentCell;
Cell* newCurrentCell;

Cell StartCell;
Cell GoalCell;

int nearby[4];

int stepCount = 0;

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

    SDL_Event event;
    SDL_Window* window;
    SDL_Surface* s;

    cellSize = floor(WINDOW_WIDTH / MAPSIZE);

    SDL_Init(SDL_INIT_VIDEO);
    SDL_CreateWindowAndRenderer(cellSize * MAPSIZE, cellSize * MAPSIZE, 0, &window, &renderer);

    SDL_SetWindowTitle(window, "majs generat");

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    SDL_RenderPresent(renderer);

    

    rect = new SDL_Rect();
    rect->w = cellSize * 0.8;
    rect->h = cellSize * 0.8;

    srand(clock());

    printf("velkom to ris profferisonal console majs genreator!\n\n");
    
    printf("butons thet ar god to knwos\n");
    printf("(you pressi pressi in windowso niet konesl)\n\n");

    printf("r: generate\n");
    printf("spesi: save generation\n");
    printf("q: abort\n\n");

    printf("wants more specliaes? use lunch options! example:\n");
    printf("majs.exe <mapsize> <shuffle> <window size>\n\n");


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
                        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
                        SDL_RenderClear(renderer);

                        shouldRender = Reset();
                        break;
                    case SDLK_q:
                        shouldExit = true;
                        break;
                    case SDLK_SPACE:
                        SDL_Surface* surface = SDL_CreateRGBSurface(0, WINDOW_WIDTH, WINDOW_WIDTH, 32, 0, 0, 0, 0);
                        SDL_RenderReadPixels(renderer, NULL, surface->format->format, surface->pixels, surface->pitch);

                        SDL_SaveBMP(surface, "test.bmp");
                        break;
                }
            }
        }

        if (shouldRender) {
            shouldRender = Render();
        }
    }

    SDL_DestroyWindow(window);
    SDL_Quit();

    return EXIT_SUCCESS;
}

bool Render() {
    if (stack.size() <= 0) {
        printf("\rGenerating map... Done!\n");
        printf("Finding best spot to place goal... ");

        int bestGoalDistance = 0;
        int currentDistance = 0;
        Cell* walker;

        for (int y = 0; y < MAPSIZE; y++)
        {
            for (int x = 0; x < MAPSIZE; x++)
            {
                walker = &CellArray[x + y * MAPSIZE];

                if (walker->next != nullptr) continue;

                currentDistance = 0;

                while (walker->prev != nullptr) {
                    currentDistance++;
                    if (currentDistance > bestGoalDistance) {
                        GoalCell = CellArray[x + y * MAPSIZE];
                        bestGoalDistance = currentDistance;
                    }

                    walker = walker->prev;
                }
            }
        }

        SDL_SetRenderDrawColor(renderer, 0, 255, 64, 255);
        UpdateCell(&StartCell);
        SDL_SetRenderDrawColor(renderer, 255, 0, 64, 255);
        UpdateCell(&GoalCell);

        SDL_RenderPresent(renderer);

        printf("Done!\n");

        printf("Everything done!\n");
        return false;
    }

    if (SHUFFLE != 0 && ++stepCount % SHUFFLE == 0) {
        int number = floor(rand() % stack.size());
        Cell* it = stack[number];
        stack.erase(stack.begin() + number);
        stack.insert(stack.begin(), it);

        return true;
    }

    printf("\rGenerating map... %d%%", ((stepCount / 2) * 100) / (MAPSIZE * MAPSIZE));

    newCurrentCell = getNearby(currentCell);

    if (newCurrentCell == nullptr) {
        stack.erase(stack.begin());
        currentCell = stack.front();
        return true;
    }

    SDL_SetRenderDrawColor(renderer, 64, 64, 64, 255);

    newCurrentCell->visited = true;
    UpdateCell(newCurrentCell);
    Bridge(currentCell, newCurrentCell);

    currentCell->next = newCurrentCell;
    newCurrentCell->prev = currentCell;

    currentCell = newCurrentCell;

    stack.insert(stack.begin(), currentCell);
    SDL_RenderPresent(renderer);

    return true;
}

bool Reset() {
    delete[] CellArray;
    CellArray = new Cell[MAPSIZE * MAPSIZE];
    stack.clear();
    stepCount = 0;

    SDL_SetRenderDrawColor(renderer, 64, 64, 64, 255);
    for (int y = 0; y < MAPSIZE; y++)
    {
        for (int x = 0; x < MAPSIZE; x++)
        {
            currentCell = &CellArray[x + y * MAPSIZE];
            currentCell->UpdateData(x, y);
            UpdateCell(currentCell);
        }
    }

    currentCell = &CellArray[rand() % MAPSIZE + rand() % MAPSIZE * MAPSIZE];
    currentCell->visited = true;
    StartCell = *currentCell;
    stack.push_back(currentCell);

    SDL_RenderPresent(renderer);

    return true;
}

void UpdateCell(Cell* cell) {
    rect->x = cell->x * cellSize + cellSize * 0.2;
    rect->y = cell->y * cellSize + cellSize * 0.2;
    SDL_RenderFillRect(renderer, rect);
}

void Bridge(Cell* cell, Cell* cell2) {

    rect->x = ((cell->x * cellSize) + (cell2->x * cellSize)) / 2 + cellSize * 0.2;
    rect->y = ((cell->y * cellSize) + (cell2->y * cellSize)) / 2 + cellSize * 0.2;

    SDL_RenderFillRect(renderer, rect);
}


Cell* getNearby(Cell* cell) {
    uint8_t num = 0;

    if (cell->x > 0) {
        if (!CellArray[(cell->x - 1) + cell->y * MAPSIZE].visited) {
            nearby[num] = (cell->x - 1) + cell->y * MAPSIZE;
            num++;
        }
    }
    if (cell->y > 0) {
        if (!CellArray[cell->x + (cell->y - 1) * MAPSIZE].visited) {
            nearby[num] = cell->x + (cell->y - 1) * MAPSIZE;
            num++;
        }
    }
    if (cell->x < (MAPSIZE - 1)) {
        if (!CellArray[(cell->x + 1) + cell->y * MAPSIZE].visited) {
            nearby[num] = (cell->x + 1) + cell->y * MAPSIZE;
            num++;
        }
    }
    if (cell->y < (MAPSIZE - 1)) {
        if (!CellArray[cell->x + (cell->y + 1) * MAPSIZE].visited) {
            nearby[num] = cell->x + (cell->y + 1) * MAPSIZE;
            num++;
        }
    }

    if (num == 0) return nullptr;

    return &CellArray[nearby[(int) floor(rand() % num)]];
}