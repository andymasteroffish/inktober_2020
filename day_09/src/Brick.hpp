//
//  Brick.hpp
//  inktober_03
//
//  Created by Andrew Wallace on 10/8/20.
//

#ifndef Brick_hpp
#define Brick_hpp

#include "ofMain.h"
#include "ofxGCode.hpp"

class Brick{
public:
    
    ofVec2f pnts[4];
    ofVec2f center;
    int row;
    
    void set_center(){
        center.set(0,0);
        for (int i=0; i<4; i++){
            center += pnts[i];
        }
        center /= 4.0;
    }
    
    void shift(float x, float y){
        for (int i=0; i<4; i++){
            pnts[i].x += x;
            pnts[i].y += y;
        }
        
        set_center();
    }
    
    void draw(){
        ofBeginShape();
        for (int i=0; i<4; i++){
            ofVertex(pnts[i]);
        }
        ofEndShape(true);
    }
    
    void make_gcode(ofxGCode * g){
        for (int i=0; i<4; i++){
            g->line(pnts[i], pnts[(i+1)%4]);
        }
    }
    
};


#endif /* Brick_hpp */
