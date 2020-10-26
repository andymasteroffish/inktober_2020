#pragma once

#include "ofMain.h"
#include "ofxGCode.hpp"

class ofApp : public ofBaseApp{

public:
    void setup();
    void refresh();
    void update();
    void draw();
    
    void load();

    void keyPressed(int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y );
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void mouseEntered(int x, int y);
    void mouseExited(int x, int y);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);
    
    
    ofxGCode gcode[2];
    
    float tile_size;
    ofVec2f grid_offset;
    
#define GRID_W 14
#define GRID_H 14
    int grid[GRID_W][GRID_H];
    bool snake_a[GRID_W][GRID_H];

#define NONE -1
#define TOP_LEFT 0
#define TOP_RIGHT 1
#define BOTTOM_RIGHT 2
#define BOTTOM_LEFT 3
#define VERT 4
#define HORZ 5
		
};
