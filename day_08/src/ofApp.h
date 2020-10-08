#pragma once

#include "ofMain.h"
#include "ofxGcode.hpp"

class ofApp : public ofBaseApp{

public:
    void setup();
    void orb(float start_x, float start_y);
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
    
    vector<ofVec2f> bottom_pnts;
		
};
