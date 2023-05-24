#pragma once

class Map {
private:
	int TILESIZE;
	int xDim, yDim;
	int** map;

	void cleanMap();
	

public:
	Map(int x, int y, int size);
	~Map();
	void generateMap(int x, int y, int size);
	void customMap(int map[]);
	int **getMap();
	int getTileSize();
	void setTileSize(int tilesize);
	int getSizeX();
	int getSizeY();
	
	

	
};
