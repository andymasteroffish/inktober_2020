//
//  Wall.h
//  mazes
//
//  Created by Andrew Wallace on 10/14/20.
//

#ifndef Wall_h
#define Wall_h

#include "ofMain.h"
#include "ofxGCode.hpp"

#define DIR_UP 0
#define DIR_RIGHT 1
#define DIR_DOWN 2
#define DIR_LEFT 3

class Wall{
public:
    
    bool is_on;
    bool locked;
    bool has_been_drawn;
    
    GLine pos;
    
    Wall(){
        reset();
    }
    
    void reset(){
        is_on = false;
        locked = false;
        has_been_drawn = false;
    }
    
    void set_and_lock(bool value){
        is_on = value;
        locked = true;
    }
    
    void set(bool value){
        if (!locked){
            is_on = value;
        }
    }
    
    void make_free_wall_gcode(ofxGCode * gcode, bool is_horz, float tile_size){
        int num_steps = tile_size * 0.4;//20
        for (int i=0; i<=num_steps; i++){
            float prc = (float)i/(float)num_steps;
            float angle = sin(prc*TWO_PI);
            if (!is_horz){
                angle += PI/2;
            }
            float dist = 5;
            ofVec2f center = (1.0-prc) * pos.a + prc * pos.b;

            ofVec2f a,b;
            a.x = center.x + cos(angle) * dist;
            a.y = center.y + sin(angle) * dist;
            b.x = center.x - cos(angle) * dist;
            b.y = center.y - sin(angle) * dist;

            gcode->line(a, b);
        }
    }
    
    void draw(){
        if (is_on){
            ofSetColor(0);
        }else{
            ofSetColor(220);
        }
        
        pos.draw();
    }
};

#endif /* Wall_h */
