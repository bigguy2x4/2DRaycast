#include "Map.h"
#include <cstddef>

Map::Map(int x, int y, int size) {
	generateMap(x, y, size);
}
Map::~Map() {}
//not sure if this works!!
void Map::generateMap(int x, int y, int size) {
	this->xDim = x;
	this->yDim = y;
	this->TILESIZE = size;
	
	//setting size
	if (map != NULL) {
		cleanMap();
	}
	
	map = new int* [y];
	for (int i = 0; i < y; ++i) {
		map[i] = new int[x];
	}
	//setting index values
	for (int i = 0; i < y; i++) {
		for (int j = 0; j < x; j++) {
			if (i == 0 || j == 0) {
				map[i][j] = 1;
			}
			else if (i == y - 1 || j == x - 1) {
				map[i][j] = 1;
			}
			else {
				map[i][j] = 0;
			}
			
			
			
		}
	}
	map[3][3] = 1;
	


}
//!!
void Map::cleanMap() {
	for (int i = 0; i < yDim; ++i) {
		delete[] map[i];
	}
	delete[] map;
}
//unsure
int** Map::getMap() {
	return map;
}
int Map::getTileSize() { return TILESIZE; }
void Map::setTileSize(int tilesize) { this->TILESIZE = tilesize; }
int Map::getSizeX() { return this->xDim; }
int Map::getSizeY() { return this->yDim; }