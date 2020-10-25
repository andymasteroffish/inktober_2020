#pragma once

#include "ofMain.h"
#include "ofxGCode.hpp"

class ofApp : public ofBaseApp{

public:
    void setup();
    void draw_triangle(float center_x, float center_y);
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
    
#define NUM_COLS 3
    ofxGCode gcode[NUM_COLS];
		
};
