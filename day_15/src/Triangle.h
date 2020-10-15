//
//  Triangle.h
//  inktober_15
//
//  Created by Andrew Wallace on 10/14/20.
//

#ifndef Triangle_h
#define Triangle_h

#include "ofMain.h"
#include "ofxGCode.hpp"
#include "Tentacle.h"

class Triangle{
public:
    
    vector<ofVec2f> border;
    vector<GLine> lines;
    
    void setup(float base_angle, float dist, float size){
        
        border.clear();
        lines.clear();
        
        ofVec2f center;
        center.x = ofGetWidth()/2 + cos(base_angle) * dist;
        center.y = ofGetHeight()/2 + sin(base_angle) * dist;
        
        //set the border
        float angle_step = TWO_PI/3.0;
        for (int i=0; i<3; i++){
            float angle = i * angle_step + base_angle + PI;
            ofVec2f pos;
            pos.x = center.x + cos(angle) * size;
            pos.y = center.y + sin(angle) * size;
            border.push_back(pos);
        }
        
        //define the lines
        
        for (int i=0; i<border.size(); i++){
            GLine line;
            line.set( border[i], border[(i+1)%border.size()] );
            vector<GLine> segments = line.get_segments(10);
            for (int k=0; k<segments.size(); k++){
                lines.push_back(segments[k]);
            }
        }
        
        //shading line
        float shade_prc = 0.9;
        ofVec2f a = shade_prc*border[1] + (1.0-shade_prc) * center;
        ofVec2f b = shade_prc*border[0] + (1.0-shade_prc) * center;
        GLine line;
        line.set(a,b);
        lines.push_back(line);
        
    }
    
    void trim(vector<Triangle> &others, vector<Tentacle> &tentacles){
        for (int i=0; i<lines.size(); i++){
            for (int o=0; o<others.size(); o++){
                lines[i].clip_outside_polygon(others[o].border);
            }
            for (int t=0; t<tentacles.size(); t++){
                lines[i].clip_outside_polygon(tentacles[t].border);
            }
            
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
    
    
    
    void test_draw(){
        if(false){
            ofSetColor(255,0,0);
            ofBeginShape();
            for (int i=0; i<border.size(); i++){
                ofVertex(border[i]);
            }
            ofEndShape(true);
        }
        
        ofSetColor(0);
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


#endif /* Triangle_h */
