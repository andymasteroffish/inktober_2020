#pragma once

#include "ofMain.h"
#include "ofxGCode.hpp"
#include "Wall.h"
#include "Tile.h"

class ofApp : public ofBaseApp{

public:
    void setup();
    void make_background(ofVec2f maze_offset);
    void reset();
    void update();
    void draw();
    
    void trace_path(vector<Tile *> path);
    vector<Tile *> find_path(Tile * start_tile, Tile * end);
    vector<Tile *> explore(Tile * start_tile, int max_length);
    bool contains(vector<Tile *> list, Tile * tile);
    
    int get_num_unexplored();
    Tile * get_explored_tile();
    void find_orphans();
    void open_closed_tiles();

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
    
    #define NUM_COLS 22
    #define NUM_ROWS 28
    
    Tile tiles[NUM_COLS][NUM_ROWS];
    Wall v_walls[NUM_COLS+1][NUM_ROWS+1];
    Wall h_walls[NUM_COLS+1][NUM_ROWS+1];
    
    int tile_size;
    
    ofxGCode gcode;
    
    
		
};
