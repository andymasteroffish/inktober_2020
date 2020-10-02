//
//  Box.hpp
//  inktober_02
//
//  Created by Andrew Wallace on 10/1/20.
//

#ifndef Box_hpp
#define Box_hpp

#include "ofMain.h"

class Box{
public:
    
    void setup(float x1, float y1, float x2, float y2);
    Box split();
    
    float get_size();
    
    void draw();
    
    ofVec2f top_left, bot_right;
    
    
};

#endif /* Box_hpp */
