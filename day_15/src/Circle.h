//
//  Circle.h
//  inktober_15
//
//  Created by Andrew Wallace on 10/15/20.
//

#ifndef Circle_h
#define Circle_h

#include "ofMain.h"
#include "ofxGCode.hpp"

class Circle{
public:
    
    vector<GLine> lines;
    vector<ofVec2f> border;
    
    
    void setup(ofVec2f center, float size, int num_pnts=30, float angle_offset = 0){
        
        float angle_step =  TWO_PI / (float)num_pnts;
        ofVec2f prev_pos;
        for (int i=0; i<=num_pnts; i++){
            float angle = angle_offset + i*angle_step;
            ofVec2f pos = ofVec2f(center);
            pos.x += cos(angle) * size;
            pos.y += sin(angle) * size;
            
            border.push_back(pos);
            
            if (i>0){
                GLine line;
                line.set(prev_pos, pos);
                lines.push_back(line);
            }
            
            prev_pos.set(pos);
        }
        
    }
    
    void trim_inside(vector<ofVec2f> border){
        for (int i=0; i<lines.size(); i++){
            lines[i].clip_inside_polygon(border);
        }
    }
    
    void trim_outside(vector<ofVec2f> border){
        for (int i=0; i<lines.size(); i++){
            lines[i].clip_outside_polygon(border);
        }
    }
    
    void add_extra(int num){
        float spacing = 1;
        vector<GLine> extra;
        for (int k=0; k<num; k++){
            for (int i=0; i<lines.size(); i++){
                if (!lines[i].skip_me){
                    GLine line;
                    line.a = ofVec2f(lines[i].a);
                    line.b = ofVec2f(lines[i].b);
                    
                    line.a.y += spacing;
                    line.b.y += spacing;
                    extra.push_back(line);
                }
            }
        }
        
        //add 'em
        for (int i=0; i<extra.size(); i++){
            lines.push_back(extra[i]);
        }
    }
    
    void draw(){
        for (int i=0; i<lines.size(); i++){
            if (!lines[i].skip_me){
                lines[i].draw();
            }
        }
        
    }
    
    void make_gcode(ofxGCode * gcode){
        for (int i=0; i<lines.size(); i++){
            if (!lines[i].skip_me){
                gcode->line(lines[i]);
            }
        }
    }
    
};


#endif /* Circle_h */
