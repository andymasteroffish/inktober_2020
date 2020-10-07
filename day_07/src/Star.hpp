//
//  Star.hpp
//  inktober_07
//
//  Created by Andrew Wallace on 10/6/20.
//

#ifndef Star_hpp
#define Star_hpp

#include "ofMain.h"
#include "ofxGCode.hpp"

class Star{
public:
    
    void setup(float center_x, float center_y, float base_dist, float spread=0.004);
    
    void draw();
    
    void make_gcode(ofxGCode * gcode);
    
    
    vector<GLine> lines;
    
    vector <ofVec2f> big_outline, small_outline;
    
};

#endif /* Star_hpp */
