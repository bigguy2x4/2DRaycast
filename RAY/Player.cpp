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

void Player::castRays1(int numRays, int** map, int tilesize, int sizeX, int sizeY, int screenwidth, int screenheight) {
	//initial position and interval distance between rays
	float stepTheta = (float) fov / (numRays-1);
	float rayDirX = (dirX * cos(fov / 2.0)) - (dirY * sin(fov / 2.0));
	float rayDirY = (dirX * sin(fov / 2.0)) + (dirY * cos(fov / 2.0));
	float fx = 0, fy = 0;
	float distance = 0;
	
	for (int i = 0; i < numRays; i++) {
		bool intersect = false;
		
		for (int a = 0; a < screenwidth; a++){
			fx = px + (a * rayDirX);
			fy = py + (a * rayDirY);

			int xInd = (int)floor(fx / tilesize);
			int yInd = (int)floor(fy / tilesize);

			if (xInd < 0) { xInd = 0; }
			if (xInd > sizeX - 1) { xInd = sizeX - 1; }
			if (yInd < 0) { yInd = 0; }
			if (yInd > sizeY - 1) { yInd = sizeY - 1; }

			if (map[yInd][xInd] == 1) {
				intersect = true;
				
				float cosAngleToDir = ((rayDirX * dirX) + (rayDirY * dirY)) / ((sqrt(pow(rayDirX, 2) + pow(rayDirY, 2)) * sqrt(pow(dirX, 2) + pow(dirY, 2))));
				
				distance = abs(sqrt(pow(fx - px, 2) + pow(fy - py, 2)) * cosAngleToDir);
			
				std::cout << "ray" << i << "intersected with: " << xInd << "," << yInd << "\n";
				std::cout << "ray" << i << "distance: " << distance << "\n";
				
				break;
			}
		}

		
		glLineWidth(1);
		glBegin(GL_LINES);
		glVertex2i(px, py);
		
		

		if (intersect) {
			glVertex2i(fx, fy);

			/*
			float rectWdith = screenwidth / (numRays - 1); 
			float rectHeight = (tilesize * 512) / distance; if (rectHeight > screenheight) { rectHeight = screenheight; }
			float yMargin = (screenheight - rectHeight) / 2;

			glBegin(GL_QUADS);
			glVertex2i(i * rectWdith, yMargin);
			glVertex2i((i*rectWdith) + rectWdith, yMargin);
			glVertex2i((i * rectWdith) + rectWdith, screenheight - yMargin);
			glVertex2i(i * rectWdith, screenheight - yMargin);
			glEnd();
			*/


		}
		
		
		//else{
			//glVertex2i(px + (rayDirX *500), py + (rayDirY*500));
		//}

		glEnd();
		

		
		
		//setting up next ray
		float rX = rayDirX; float rY = rayDirY;
		rayDirX = (rX * cos(-stepTheta)) - (rY * sin(-stepTheta));
		rayDirY = (rX * sin(-stepTheta)) + (rY * cos(-stepTheta));



	}
}

void Player::castRays2(int numRays, int** map, int tilesize, int sizeX, int sizeY, int screenwidth, int screenheight) {
	//initial position and interval distance between rays
	float stepTheta = (float)fov / (numRays - 1);
	float rayDirX = (dirX * cos(fov / 2.0)) - (dirY * sin(fov / 2.0));
	float rayDirY = (dirX * sin(fov / 2.0)) + (dirY * cos(fov / 2.0));
	float angleToH = atan(rayDirY / rayDirX);
	
	//final distance
	float distanceA = 0, distanceB = 0, distancef = 0;

	for (int i = 0; i < numRays; i++) {


		//first Horizontal line intersect point
		//going up
		float Ax = 0, Ay = 0;
		if (rayDirY < 0) {Ay = floor(py / tilesize) * tilesize - 0.01;}
		if (rayDirY > 0) { Ay = (floor(py / tilesize) * tilesize) + tilesize; }
		Ax = px + copysignf((Ay - py) / tan(angleToH), rayDirX);
		bool intersectA = false;

		float Bx = 0, By = 0;
		if (rayDirX < 0) { Bx = floor(px / tilesize) * tilesize - 0.01; }
		if (rayDirX > 0) { Bx = floor(px / tilesize) * tilesize + tilesize; }
		By = py + copysignf((Bx - px) * tan(angleToH), rayDirY);
		bool intersectB = false;

		for (int i = 0; i < max(sizeX,sizeY); i++) {
			int xInd = (int)floor(Ax / tilesize);
			int yInd = (int)floor(Ay / tilesize);

			if (xInd < 0) { xInd = 0; }
			if (xInd > sizeX - 1) { xInd = sizeX - 1; }
			if (yInd < 0) { yInd = 0; }
			if (yInd > sizeY - 1) { yInd = sizeY - 1; }

			if (map[yInd][xInd] == 1) {
				intersectA = true;
				distanceA = abs(sqrt(pow(Ax - px, 2) + pow(Ay - py, 2)));
			}

			if (!intersectA) {
				Ay = Ay + copysignf(tilesize, rayDirY);
				Ax = Ax + copysignf((tilesize / tan(angleToH)), rayDirX);
			}

			xInd = (int)floor(Bx / tilesize);
			yInd = (int)floor(By / tilesize);

			if (xInd < 0) { xInd = 0; }
			if (xInd > sizeX - 1) { xInd = sizeX - 1; }
			if (yInd < 0) { yInd = 0; }
			if (yInd > sizeY - 1) { yInd = sizeY - 1; }

			if (map[yInd][xInd] == 1) {
				intersectB = true;
				distanceB = abs(sqrt(pow(Bx - px, 2) + pow(By - py, 2)));
			}if (!intersectB) {
				Bx = Bx + copysignf(tilesize, rayDirX);
				By = By + copysignf(tilesize * tan(angleToH), rayDirY);

			}


		}
		
	
	


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
			if (intersectB & !intersectA) {
				distancef = distanceB;
			}
			
			float cosAngleToDir = ((rayDirX * dirX) + (rayDirY * dirY)) / ((sqrt(pow(rayDirX, 2) + pow(rayDirY, 2)) * sqrt(pow(dirX, 2) + pow(dirY, 2))));
			distancef = abs(distancef * cosAngleToDir);

			float rectWdith = screenwidth / (numRays - 1);
			float rectHeight = (tilesize * 512) / distancef; if (rectHeight > screenheight) { rectHeight = screenheight; }
			float yMargin = (screenheight - rectHeight) / 2;

			glBegin(GL_QUADS);
			glVertex2i(i * rectWdith, yMargin);
			glVertex2i((i * rectWdith) + rectWdith, yMargin);
			glVertex2i((i * rectWdith) + rectWdith, screenheight - yMargin);
			glVertex2i(i * rectWdith, screenheight - yMargin);
			glEnd();

		}
		
	
		
		//setting up next ray
		float rX = rayDirX; float rY = rayDirY;
		rayDirX = (rX * cos(-stepTheta)) - (rY * sin(-stepTheta));
		rayDirY = (rX * sin(-stepTheta)) + (rY * cos(-stepTheta));
		angleToH = atan(rayDirY / rayDirX);
	}


	

}



void Player::castRays3(int numRays, int** map, int tilesize, int sizeX, int sizeY, int screenwidth, int screenheight) {
	// Initial position and interval distance between rays
	float stepTheta = static_cast<float>(fov) / (numRays - 1);
	float rayDirX = (dirX * cos(fov / 2.0)) - (dirY * sin(fov / 2.0));
	float rayDirY = (dirX * sin(fov / 2.0)) + (dirY * cos(fov / 2.0));
	float angleToH = atan(rayDirY / rayDirX);

	// Final distance
	float distanceA = 0, distanceB = 0, distancef = 0;

	for (int i = 0; i < numRays; i++) {
		// First horizontal line intersect point (going up)
		float Ax = 0, Ay = 0;
		if (rayDirY < 0) {
			Ay = floor(py / tilesize) * tilesize - 0.01;
		}
		if (rayDirY > 0) {
			Ay = (floor(py / tilesize) * tilesize) + tilesize;
		}
		Ax = px + copysignf((Ay - py) / tan(angleToH), rayDirX);
		bool intersectA = false;

		// First vertical line intersect point
		float Bx = 0, By = 0;
		if (rayDirX < 0) {
			Bx = floor(px / tilesize) * tilesize - 0.01;
		}
		if (rayDirX > 0) {
			Bx = floor(px / tilesize) * tilesize + tilesize;
		}
		By = py + copysignf((Bx - px) * tan(angleToH), rayDirY);
		bool intersectB = false;

		for (int j = 0; j < std::max(sizeX, sizeY); j++) {
			int xInd = static_cast<int>(floor(Ax / tilesize));
			int yInd = static_cast<int>(floor(Ay / tilesize));

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
				intersectA = true;
				distanceA = std::abs(sqrt(pow(Ax - px, 2) + pow(Ay - py, 2)));
			}

			if (!intersectA) {
				Ay = Ay + copysignf(tilesize, rayDirY);
				Ax = Ax + copysignf((tilesize / tan(angleToH)), rayDirX);
			}

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
			if (intersectB & !intersectA) {
				distancef = distanceB;
			}

			float cosAngleToDir = ((rayDirX * dirX) + (rayDirY * dirY)) /
				((sqrt(pow(rayDirX, 2) + pow(rayDirY, 2)) * sqrt(pow(dirX, 2) + pow(dirY, 2))));
			distancef = std::abs(distancef * cosAngleToDir);

			float rectWdith = screenwidth / (numRays - 1);
			float rectHeight = (tilesize * 512) / distancef;
			if (rectHeight > screenheight) {
				rectHeight = screenheight;
			}
			float yMargin = (screenheight - rectHeight) / 2;

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


	





