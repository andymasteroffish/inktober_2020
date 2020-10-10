#pragma once

#include "ofMain.h"
#include "ofxGCode.hpp"

class Fray{
public:
    int color;
    ofVec2f pos;
    float angle;
};

class ofApp : public ofBaseApp{

public:
    void setup();
    void curve_shoot(Fray fray);
    void line_shoot(int c, int r, float grid_size, float start_x, float start_y, float dist, int angle_dir, bool clockwise, ofVec2f pivot, int col);
    void update();
    void draw();

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
    
    ofxGCode gcode;
    
#define NUM_COLS 4
    vector<GLine> lines[NUM_COLS];
    
    float main_circle_dist;
		
};
