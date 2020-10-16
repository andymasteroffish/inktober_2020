//
//  Tile.h
//  mazes
//
//  Created by Andrew Wallace on 10/14/20.
//

#ifndef Tile_h
#define Tile_h

#include "ofMain.h"
#include "ofxGCode.hpp"
#include "Wall.h"

class Tile{
public:
    
    Wall * walls[4];
    
    bool critical_path;
    bool explored;
    
    int c,r;
    
    ofColor demo_col;
    
    Tile(){
        reset();
    }
    
    void reset(){
        critical_path = false;
        explored = false;
        
        demo_col.set(200);
    }
    
    void make_gcode(ofxGCode * gcode, float tile_size){
        
        //corners
        for (int w=0; w<4; w++){
            Wall * wall_a = walls[w];
            Wall * wall_b = walls[(w+1)%4];
            
            if (wall_a->is_on && wall_b->is_on){
                int num_steps = tile_size * 0.3333;
                if (w%2==0)    num_steps = tile_size * 0.4;
                for (int i=0; i<=num_steps; i++){
                    float prc = (float)i/(float)num_steps;
                    ofVec2f pos_a = (1.0-prc) * wall_a->pos.a + prc * wall_a->pos.b;
                    ofVec2f pos_b = (1.0-prc) * wall_b->pos.b + prc * wall_b->pos.a;
                    gcode->line(pos_a, pos_b);
                }
                wall_a->has_been_drawn = true;
                wall_b->has_been_drawn = true;
            }
        }
        
        
        
    }
    
    void draw(){
        
        //get the average position
        ofVec2f pos;
        pos += walls[0]->pos.a + walls[1]->pos.b;
        pos /= 2.0;

        ofSetColor(demo_col);
        ofDrawCircle(pos.x, pos.y, 10);
    }
    
    bool test_click(int x, int y){
        ofVec2f pos;
        pos += walls[0]->pos.a + walls[1]->pos.b;
        pos /= 2.0;
        if (ofDist(x,y, pos.x, pos.y) < 15){
            return true;
        }
        return false;
    }
    
    
    
};

#endif /* Tile_h */
