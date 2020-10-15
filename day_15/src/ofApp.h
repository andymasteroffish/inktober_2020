#pragma once

#include "ofMain.h"

#include "ofxGCode.hpp"
#include "Triangle.h"
#include "Tentacle.h"

class ofApp : public ofBaseApp{

public:
    void setup();
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
    
    #define NUM_COLS 4
    #define PURPLE 0
    #define LIGHT_PURPLE 1
    #define RED 2
    #define BLACK 3
    
    ofxGCode gcode[NUM_COLS];
    
    vector<Triangle> triangles;
    vector<Tentacle> tentacles;
		
};
