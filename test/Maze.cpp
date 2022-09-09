#include "Maze.h";
#include <iostream>

Maze::Maze(int _mapSize, int _shuffle, int _cellSize, SDL_Renderer* _renderer) {
    
    mapSize = _mapSize;
    shuffle = _shuffle;
    cellSize = _cellSize;

    renderer = _renderer;

    rect = new SDL_Rect();
    rect->w = cellSize * 0.8;
    rect->h = cellSize * 0.8;

	CellArray = new Cell[mapSize * mapSize];
}

void Maze::Render(bool* shouldRender) {
    if (shuffle != 0 && ++stepCount % (shuffle + 1) == 0) {
        int number = floor(rand() % stack.size());
        Cell* it = stack[number];
        stack.erase(stack.begin() + number);
        stack.insert(stack.begin(), it);

        currentCell = stack.front();

        return;
    }

    percentage = ((stepCount / 2) * 100) / (mapSize * mapSize);

    if (percentage != prevPercentage) {
        printf("\rGenerating map... %s ", spinner[percentage % 4]);

        prevPercentage = percentage;
    }

    newCurrentCell = getNearby(currentCell);

    //printf(" currentcell: %d:%d ", currentCell->x, currentCell->y);

    if (newCurrentCell == nullptr) {
        stack.erase(stack.begin());
        currentCell = stack.front();

        /*
        printf(" stack: ");

        for (int i = 0; i < stack.size(); i++) {
            printf("%d:%d, ", stack[i]->x, stack[i]->y);
        }

        printf("\n");
        */

        if (stack.size() == 0) {
            printf("\rGenerating map... Done!\n");
            printf("Finding best spot to place goal... ");

            int bestGoalDistance = 0;
            int currentDistance = 0;
            Cell* walker;

            for (int y = 0; y < mapSize; y++)
            {
                for (int x = 0; x < mapSize; x++)
                {
                    walker = &CellArray[x + y * mapSize];

                    if (walker->next != nullptr) continue;

                    currentDistance = 0;

                    while (walker->prev != nullptr) {
                        currentDistance++;
                        if (currentDistance > bestGoalDistance) {
                            GoalCell = CellArray[x + y * mapSize];
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

            *shouldRender = false;
        }

        return;
    }

    SDL_SetRenderDrawColor(renderer, 64, 64, 64, 255);

    newCurrentCell->visited = true;
    UpdateCell(newCurrentCell);
    Bridge(currentCell, newCurrentCell);

    currentCell->next = newCurrentCell;
    newCurrentCell->prev = currentCell;

    currentCell = newCurrentCell;

    SDL_RenderPresent(renderer);

    stack.insert(stack.begin(), currentCell);

    return;
}

void Maze::Reset(bool* shouldRender) {
    stack.clear();
    delete[] CellArray;
    CellArray = new Cell[mapSize * mapSize];

    rect->w = cellSize * 0.8;
    rect->h = cellSize * 0.8;

    stepCount = 0;

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    SDL_SetRenderDrawColor(renderer, 64, 64, 64, 255);

    for (uint16_t y = 0; y < mapSize; y++)
    {
        for (uint16_t x = 0; x < mapSize; x++)
        {
            CellArray[x + y * mapSize].ResetCell(x, y);
        }
    }

    currentCell = &CellArray[rand() % mapSize + rand() % mapSize * mapSize];
    currentCell->visited = true;
    stack.push_back(currentCell);

    StartCell = *currentCell;

    SDL_RenderPresent(renderer);

    *shouldRender = true;
}

void Maze::UpdateMaze(int _mapSize, int _shuffle, int _cellSize) {
    mapSize = _mapSize;
    shuffle = _shuffle;
    cellSize = _cellSize;
}

void Maze::Solve() {

    Cell* walker = GoalCell.prev;

    SDL_SetRenderDrawColor(renderer, 16, 128, 16, 255);

    while (walker->prev != nullptr) {
        UpdateCell(walker);

        walker = walker->prev;
    }

    SDL_RenderPresent(renderer);
}

void Maze::UpdateCell(Cell* cell) {
    rect->x = cell->x * cellSize + cellSize * 0.1;
    rect->y = cell->y * cellSize + cellSize * 0.1;
    SDL_RenderFillRect(renderer, rect);
}

void Maze::Bridge(Cell* cell, Cell* cell2) {

    rect->x = ((cell->x * cellSize) + (cell2->x * cellSize)) / 2 + cellSize * 0.1;
    rect->y = ((cell->y * cellSize) + (cell2->y * cellSize)) / 2 + cellSize * 0.1;

    SDL_RenderFillRect(renderer, rect);
}


Cell* Maze::getNearby(Cell* cell) {
    uint8_t num = 0;

    if (cell->x > 0) {
        if (!CellArray[(cell->x - 1) + cell->y * mapSize].visited) {
            nearby[num] = (cell->x - 1) + cell->y * mapSize;
            num++;
        }
    }
    if (cell->y > 0) {
        if (!CellArray[cell->x + (cell->y - 1) * mapSize].visited) {
            nearby[num] = cell->x + (cell->y - 1) * mapSize;
            num++;
        }
    }
    if (cell->x < (mapSize - 1)) {
        if (!CellArray[(cell->x + 1) + cell->y * mapSize].visited) {
            nearby[num] = (cell->x + 1) + cell->y * mapSize;
            num++;
        }
    }
    if (cell->y < (mapSize - 1)) {
        if (!CellArray[cell->x + (cell->y + 1) * mapSize].visited) {
            nearby[num] = cell->x + (cell->y + 1) * mapSize;
            num++;
        }
    }

    if (num == 0) return nullptr;

    return &CellArray[nearby[(int)floor(rand() % num)]];
}