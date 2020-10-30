#pragma once

#include "ofMain.h"
#include "ofxGCode.hpp"
#include "Node.h"


class ofApp : public ofBaseApp{

public:
    void setup();
    void refresh();
    void populate(int target_num);
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
    ofxGCode gcode_red;
    
    vector<Node> pnts;
    int cur_dragging;
    float max_dist;
    
    bool show_pnts;
    bool show_max_dist;
    bool show_field, show_grid;
    
    int seed;
		
};
