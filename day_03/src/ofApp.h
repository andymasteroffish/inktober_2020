#pragma once

#include "ofMain.h"

#include "ofxGCode.hpp"

class ofApp : public ofBaseApp{

public:
    void setup();
    void draw_cone(ofVec2f a, ofVec2f b, ofVec2f c);
    void crackle(float startX, float startY);
    void orbit();
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
    
    vector <ofVec2f> cone1, cone2;
    
    vector <GLine> orbit_lines;
    vector <GLine> crackle_lines;
};
