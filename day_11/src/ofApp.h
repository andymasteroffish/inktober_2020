#pragma once

#include "ofMain.h"
#include "ofxGCode.hpp"
#include "Circle.h"

class ofApp : public ofBaseApp{

public:
    void setup();
    Circle * generate_circle();
    void circle_fill(Circle * circ, float angle);
    void line_shoot(ofVec2f origin, float angle, Circle * circ);
    bool inside_other(ofVec2f pos, vector<Circle *> others);
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
    
    Circle * root;
    
    //Circle circ_a, circ_b;
   // vector<Circle> circles;
    
//    ofVec2f center_a, center_b;
//    float size_a, size_b;
    
    float line_spacing;
    float line_step_dist;
    float min_circle_size;
    
    float grow_step;
    float grow_padding;
    
    vector<GLine> lines;
		
};
