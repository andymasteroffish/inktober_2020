#pragma once

#include "ofMain.h"
#include "ofxGCode.hpp"

class ofApp : public ofBaseApp{

public:
    void setup();

    void draw_polar(ofRectangle border, int num_layers);
    void draw_dial(ofRectangle border);
    void draw_bar(ofRectangle border);
    void draw_pie(ofRectangle border, int num_rings);
    void draw_line(ofRectangle border, int num_lines);
    
    void draw_frame(ofRectangle rect);
    
    void draw_thick_line(ofVec2f base_a, ofVec2f base_b);
    void fill_pattern(vector<ofVec2f> bounds);
    
    int get_next_pattern();
    int get_next_color();

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
    
    int stroke_col;
    
    int thick_line_layers;
    float thick_line_offset;
    
#define PATTERN_CIRC 0
#define PATTERN_SQUARE 1
#define PATTERN_PLUS 2
#define PATTERN_STRIPE_HORZ 3
#define PATTERN_STEP 4
#define PATTERN_X 5
    
#define NUM_PATTERNS 6
    
    vector<int> available_patterns;
    vector<int> available_colors;
    
#define PATTERN_STRIPE_SLANT 3
    
    vector<ofRectangle> clip_boxes;
		
};
