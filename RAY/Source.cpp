#include <GL/glut.h>
#include <cmath>
#include "Player.h"
#include "Map.h"
#include <iostream>
using namespace std;


Player player(75, 75, 1.0);
Map map(16, 8, 64);
int SCREENWIDTH = map.getSizeX() * map.getTileSize();
int SCREENHEIGHT = map.getSizeY() * map.getTileSize();

void drawMap() {
   
    int x, y, xi, yi;
    
    for (y = 0; y < map.getSizeY(); y++) {
        for (x = 0; x < map.getSizeX(); x++) {
            if (map.getMap()[y][x] == 1) {
                glColor3f(1, 1, 1);
            }
            else {
                glColor3f(0, 0, 0);
            }
            xi = x * map.getTileSize();
            yi = y * map.getTileSize();

            glBegin(GL_QUADS);
            glVertex2i(xi + 1, yi + 1);
            glVertex2i(xi + 1, yi + map.getTileSize()-1);
            glVertex2i(xi + map.getTileSize()-1, yi + map.getTileSize()-1);
            glVertex2i(xi + map.getTileSize()-1, yi+1);
            glEnd();
        }

    }
}


// Display
void display()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    //drawMap();
   // player.draw();
    player.castRays3(512, map.getMap(), map.getTileSize(), map.getSizeX(), map.getSizeY(), SCREENWIDTH, SCREENHEIGHT);
    glutSwapBuffers();
 
    

}

void buttons(unsigned char key, int x, int y) {
    if (key == 'w') { 
        float xNew = player.getX() + (2*player.getDirX());
        float yNew = player.getY() + (2*player.getDirY());
        player.setX(xNew); player.setY(yNew);
    }
    if(key == 's') {
        float xNew = player.getX() - (2*player.getDirX());
        float yNew = player.getY() - (2*player.getDirY());
        player.setX(xNew); player.setY(yNew);
    }
    if (key == 'a') { player.rotate(0.1); }
    if (key == 'd') { player.rotate(-0.1); }
    glutPostRedisplay();

}

// Initialization
void init()
{
    glClearColor(0.3,0.3,0.3,0);
    gluOrtho2D(0, SCREENWIDTH, SCREENHEIGHT, 0);
    
    

}

// Main
int main(int argc, char* argv[])
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutCreateWindow("RAYCAST");
    init();
    glutInitWindowSize(SCREENWIDTH, SCREENHEIGHT);
    glutKeyboardFunc(buttons);
    glutDisplayFunc(display);
    glutMainLoop();


}

// End