//
//  Shape.hpp
//  inktober_03
//
//  Created by Andrew Wallace on 10/2/20.
//

#ifndef Shape_hpp
#define Shape_hpp

#include "ofMain.h"
#include "GLine.hpp"

class Shape{
public:
    
    void setup(float x, float y, float s);
    void shade(ofVec2f o0, ofVec2f o1, ofVec2f o2,ofVec2f o3);
    
    void draw();
    
    ofVec2f center;
    vector<ofVec2f> outline;
    
    vector<GLine> shading;
    
};

#endif /* Shape_hpp */
