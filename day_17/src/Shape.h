//
//  Shape.h
//  inktober_17
//
//  Created by Andrew Wallace on 10/16/20.
//

#ifndef Shape_h
#define Shape_h

#include "ofMain.h"
#include "ofxGCode.hpp"

class Shape{
public:
    
    vector<ofVec2f> pnts;
    vector<GLine> lines;
    ofVec2f center;
    
    void setup(float center_x, float center_y, float size, int num_pnts){
        //define bounds
        center.set(center_x, center_y);
        float angle_step = TWO_PI / (float)num_pnts;
        for (int i=0; i<num_pnts; i++){
            float angle = angle_step * i;
            ofVec2f pos = ofVec2f(center);
            pos.x += sin(angle) * size;
            pos.y += cos(angle) * size;
            pnts.push_back(pos);
        }
        
        float min_segment_length = 5;
        
        //make lines
        for (int i=0; i<pnts.size(); i++){
            GLine base;
            base.set(pnts[i], pnts[(i+1)%num_pnts]);
            
            int segment_count = MAX(1, ofDist(base.a.x, base.a.y, base.b.x, base.b.y) / min_segment_length);
            vector<GLine> segments = base.get_segments(segment_count);
            for (int k=0; k<segments.size(); k++){
                lines.push_back(segments[k]);
            }
            
        }
        
        
    }
    
    void clip_inside(vector<ofVec2f> bounds){
        for (int i=0; i<lines.size(); i++){
            lines[i].clip_inside_polygon(bounds);
        }
    }
    
    void clip_outside(vector<ofVec2f> bounds){
        for (int i=0; i<lines.size(); i++){
            lines[i].clip_outside_polygon(bounds);
        }
    }
    
    bool check_is_visible(){
        for (int i=0; i<lines.size(); i++){
            if(!lines[i].skip_me){
                return true;
            }
        }
        
        return false;
    }
    
    void make_gcode(ofxGCode * gcode){
        for (int i=0; i<lines.size(); i++){
            gcode->line(lines[i]);
        }
    }
    
};


#endif /* Shape_h */
