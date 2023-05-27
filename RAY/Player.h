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
	void castRaysOld(int numRays, int** map, int tilesize, int sizeX, int sizeY, int screenwidth, int screenheight);
	void castRays(int numRays, int** map, int tilesize, int sizeX, int sizeY, int screenwidth, int screenheight);
	bool checkCollision(int** map, int tilesize);
};
