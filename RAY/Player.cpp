#include "Player.h"
#include <GL/glut.h>
#include <cmath>
#include <iostream>

using namespace std;

Player::Player(float x, float y, float fov) {
	this->px = x;
	this->py = y;
	this->dirX = 1; this->dirY = 0;
	this->fov = fov;
}

Player::~Player() {}
float Player::getX() {return px;}
float Player::getY() { return py; }
void Player::setX(float x) { this->px = x; }
void Player::setY(float y) { this->py = y; }

float Player::getDirX() { return dirX; }
float Player::getDirY() { return dirY; }
void Player::setDirX(float x) { this->dirX = x; }
void Player::setDirY(float y) { this->dirY = y; }

void Player::rotate(float theta) {
	
	
	float dx = dirX; float dy = dirY;
	
	this->dirX = (dx * cos(theta)) - (dy * sin(theta));
	this->dirY = (dx * sin(theta)) + (dy * cos(theta));
	
}

void Player::draw() {
	//player
	glColor3f(1, 0, 1);
	glPointSize(8);
	glBegin(GL_POINTS);
	glVertex2i(px, py);
	glEnd();

	//direction line
	glLineWidth(1);
	glBegin(GL_LINES);
	glVertex2i(px, py);
	glVertex2i(px+dirX * 50, py+dirY * 50);
	glEnd();

}


//need to move this method out of player class!! totally unnecessary to have this many arguments
void Player::castRays(int numRays, int** map, int tilesize, int sizeX, int sizeY, int screenwidth, int screenheight) {
	// Initial position and interval distance between rays
	float stepTheta = static_cast<float>(fov) / (numRays - 1);
	float rayDirX = (dirX * cos(fov / 2.0)) - (dirY * sin(fov / 2.0));
	float rayDirY = (dirX * sin(fov / 2.0)) + (dirY * cos(fov / 2.0));
	float angleToH = atan(rayDirY / rayDirX);

	// Final distance
	float distanceA = 0, distanceB = 0, distancef = 0;

	for (int i = 0; i < numRays; i++) {
		float Ax = 0, Ay = 0;
		//if the y compoment of the ray is 'pointing up' on the screen, finds the distance the player position is from the tile above
		if (rayDirY < 0) {
			Ay = floor(py / tilesize) * tilesize - 0.01;
		}
		//similarily checks the y distance from player position to the tile underneat
		if (rayDirY > 0) {
			Ay = (floor(py / tilesize) * tilesize) + tilesize;
		}
		//calculates the x component of the line relative to the determined y component
		Ax = px + copysignf((Ay - py) / tan(angleToH), rayDirX);
		bool intersectA = false;

		//does the same, but this time we find the x distance to the nearest tile left and right, and then calculate the Y relative to the X travelled
		float Bx = 0, By = 0;
		if (rayDirX < 0) {
			Bx = floor(px / tilesize) * tilesize - 0.01;
		}
		if (rayDirX > 0) {
			Bx = floor(px / tilesize) * tilesize + tilesize;
		}
		By = py + copysignf((Bx - px) * tan(angleToH), rayDirY);
		bool intersectB = false;

		//at most we must travel a number of tiles equal to the larger dimension
		for (int j = 0; j < std::max(sizeX, sizeY); j++) {
			//we check what tile the point we calculated on the ray belongs to
			int xInd = static_cast<int>(floor(Ax / tilesize));
			int yInd = static_cast<int>(floor(Ay / tilesize));

			//handling for out of bounds
			if (xInd < 0) {
				xInd = 0;
			}
			if (xInd > sizeX - 1) {
				xInd = sizeX - 1;
			}
			if (yInd < 0) {
				yInd = 0;
			}
			if (yInd > sizeY - 1) {
				yInd = sizeY - 1;
			}
			//if the point on the ray belongs to a tile with a '1' value in the map then a collision has been found 
			if (map[yInd][xInd] == 1) {
				intersectA = true;
				distanceA = std::abs(sqrt(pow(Ax - px, 2) + pow(Ay - py, 2)));
			}
			//if no itersection we travel a whole tilesize on the y and compute the x component relative to
			if (!intersectA) {
				Ay = Ay + copysignf(tilesize, rayDirY);
				Ax = Ax + copysignf((tilesize / tan(angleToH)), rayDirX);
			}
			//we do the same for the version iterating horizontally
			xInd = static_cast<int>(floor(Bx / tilesize));
			yInd = static_cast<int>(floor(By / tilesize));

			if (xInd < 0) {
				xInd = 0;
			}
			if (xInd > sizeX - 1) {
				xInd = sizeX - 1;
			}
			if (yInd < 0) {
				yInd = 0;
			}
			if (yInd > sizeY - 1) {
				yInd = sizeY - 1;
			}

			if (map[yInd][xInd] == 1) {
				intersectB = true;
				distanceB = std::abs(sqrt(pow(Bx - px, 2) + pow(By - py, 2)));
			}
			if (!intersectB) {
				Bx = Bx + copysignf(tilesize, rayDirX);
				By = By + copysignf(tilesize * tan(angleToH), rayDirY);
			}
		}
		//if an intersection is found either iterating horizontally or vertically we take the one with the shorter distance
		if (intersectA || intersectB) {
			if (intersectA && intersectB) {
				if (distanceA < distanceB) {
					distancef = distanceA;
				}
				else {
					distancef = distanceB;
				}
			}
			if (intersectA && !intersectB) {
				distancef = distanceA;
			}
			if (intersectB && !intersectA) {
				distancef = distanceB;
			}
			//for 'fish eye' compensation, calculating the angle between the ray and the direction vector
			float cosAngleToDir = ((rayDirX * dirX) + (rayDirY * dirY)) /
				((sqrt(pow(rayDirX, 2) + pow(rayDirY, 2)) * sqrt(pow(dirX, 2) + pow(dirY, 2))));
			distancef = std::abs(distancef * cosAngleToDir);

			//setting width and heigh of each rectangle, and sets their height proportional to distance
			float rectWdith = screenwidth / (numRays - 1);
			float rectHeight = (tilesize * 512) / distancef;
			if (rectHeight > screenheight) {
				rectHeight = screenheight;
			}
			float yMargin = (screenheight - rectHeight) / 2;
			//drawing the rectangle
			glBegin(GL_QUADS);
			glVertex2i(i * rectWdith, yMargin);
			glVertex2i((i * rectWdith) + rectWdith, yMargin);
			glVertex2i((i * rectWdith) + rectWdith, screenheight - yMargin);
			glVertex2i(i * rectWdith, screenheight - yMargin);
			glEnd();
		}

		// Setting up the next ray
		float rX = rayDirX;
		float rY = rayDirY;
		rayDirX = (rX * cos(-stepTheta)) - (rY * sin(-stepTheta));
		rayDirY = (rX * sin(-stepTheta)) + (rY * cos(-stepTheta));
		angleToH = atan(rayDirY / rayDirX);
	}

}

bool Player::checkCollision(int** map, int tilesize) {
	float fx = (px + (dirX* 2));
	float fy = (py + (dirY * 2));

	int xIndex = (int)floor(fx / tilesize);
	int yIndex = (int)floor(fy / tilesize);

	if (map[yIndex][xIndex] == 1) {
		return true;
	}
	return false;
}


	





