#pragma once

#include <SDL.h>
#include <cstdint>

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

	void UpdateData(int xPos, int yPos) {
		x = xPos;
		y = yPos;

		visited = false;
	}
};

bool Reset();
bool Render();
void Bridge(Cell* cell, Cell* cell2);
void UpdateCell(Cell* cell);
Cell* getNearby(Cell* cell);