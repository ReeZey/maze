#pragma once

#include <SDL.h>
#include <vector>

struct Cell {
	uint16_t x, y;
	bool visited;
	Cell* prev;
	Cell* next;

	Cell() {
		x = 0;
		y = 0;
		visited = false;
		prev = nullptr;
		next = nullptr;
	}

	void ResetCell(int xPos, int yPos) {
		x = xPos;
		y = yPos;
		visited = false;
		prev = nullptr;
		next = nullptr;
	}
};

class Maze {
public:
	Maze(int mapSize, int shuffle, int cellSize, SDL_Renderer* renderer);
	void Render(bool* shouldRender);
	void Reset(bool* shouldRender);
	void UpdateMaze(int mapSize, int shuffle, int cellSize);
	void Solve();
private:
	int shuffle;
	int mapSize;
	int cellSize;

	std::vector<Cell*> stack;

	Cell* CellArray;
	Cell* currentCell;
	Cell* newCurrentCell;
	Cell StartCell;
	Cell GoalCell;
	int nearby[4];
	int stepCount = 0;
	uint8_t percentage = 0;
	uint8_t prevPercentage = 0;

	const char* spinner[4] = { "/", "|", "\\", "-" };

	SDL_Renderer* renderer;
	SDL_Rect* rect;
	void UpdateCell(Cell* cell);
	void Bridge(Cell* cell, Cell* cell2);
	Cell* getNearby(Cell* cell);
};

