#pragma once

#include "ofMain.h"

#include "ofxGCode.hpp"

#include "Box.hpp"

class Circle{
public:
    float x,y,s;
};

class ofApp : public ofBaseApp{

public:
    void setup();
    void make_circles();
    void split();
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
    
    bool show_boxes;
    vector<Box> boxes;
    
    vector<Circle> circs;
		
};
