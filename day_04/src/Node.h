//
//  Node.h
//  inktober_04
//
//  Created by Andrew Wallace on 10/4/20.
//

#ifndef Node_h
#define Node_h

#include "ofMain.h"
#include "ofxGCode.hpp"

#define OUT_DIR 0
#define CLOCK_DIR 1
#define IN_DIR 2
#define COUNTER_CLOCK_DIR 3


class Node {
public:
    

    
    float x,y;
    bool used;
    
    bool draw_circle;
    
    Node * neighbors[4];
    bool connections[4];
    
    void setup(float angle, float dist){
        x = ofGetWidth()/2 + cos(angle) * dist;
        y = ofGetHeight()/2 + sin(angle) * dist;
        
        used = false;
        draw_circle = false;
        
        for (int i=0; i<4; i++){
            neighbors[i] = NULL;
            connections[i] = false;
        }
    }
    
    void draw(){
        for (int i=0; i<4; i++){
            if (connections[i]){
                ofDrawLine(x,y, neighbors[i]->x, neighbors[i]->y);
                
            }
        }
    }
    
    void make_gcode(ofxGCode * gcode){
        for (int i=0; i<4; i++){
            if (connections[i]){
                gcode->line(x,y, neighbors[i]->x, neighbors[i]->y);
            }
        }
        
        if (draw_circle){
            float size = ofDist(x,y, ofGetWidth()/2, ofGetHeight()/2) * 0.01;
            gcode->circle(x,y, size);
        }

    }
};


#endif /* Node_h */
