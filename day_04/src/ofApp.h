#pragma once

#include "ofMain.h"

#include "ofxGCode.hpp"
#include "Node.h"

class Ticket{
public:
    int dir;
    float chance;
};

class ofApp : public ofBaseApp{

public:
    void setup();
    void block(int block_start_d,int block_start_a, int size);
    void explore_node(Node * node);
    
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
    
#define NUM_OUTWARD 30
#define NUM_AROUND 120
    Node nodes[NUM_OUTWARD][NUM_AROUND];
    
    
};
