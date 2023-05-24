#pragma once

class Player {
private:
	float px, py;
	float dirX, dirY;
	float fov;

public:
	Player(float x, float y, float fov);
	~Player();
	float getX();
	float getY();
	void setX(float x);
	void setY(float y);

	float getDirX();
	float getDirY();
	void setDirX(float x);
	void setDirY(float y);
	void rotate(float theta);

	float getFOV();
	
	void draw();
	void castRays1(int numRays, int** map, int tilesize, int sizeX, int sizeY, int screenwidth, int screenheight);
	void castRays2(int numRays, int** map, int tilesize, int sizeX, int sizeY, int screenwidth, int screenheight);
	void castRays3(int numRays, int** map, int tilesize, int sizeX, int sizeY, int screenwidth, int screenheight);
};
