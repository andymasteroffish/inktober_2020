#pragma once

#include "ofMain.h"

#include "ofxGCode.hpp"
#include "Node.h"

class ofApp : public ofBaseApp{

public:
    void setup();
    ofVec2f get_free_spot();
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
    
    vector<Node> nodes;
    
#define GRID_W 600
#define GRID_H 450
    int grid[GRID_W][GRID_H];
    
    float padding;
    int tile_size;
		
};
