#pragma once

#include "ofMain.h"

#include "ofxGCode.hpp"

class Circ{
public:
    float x;
    float y;
    float s;
};

class ofApp : public ofBaseApp{

	public:
    void setup();
    void make_gcode();
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
    vector<Circ> circles;
    
    int selection;
    
    bool show_circles;
    
    float loop_end_mod;
    
    float bend;
		
};
