//
//  Box.h
//  inktober_extra_27
//
//  Created by Andrew Wallace on 10/26/20.
//

#ifndef Box_h
#define Box_h

#include "ofMain.h"
#include "ofxGCode.hpp"

class Box{
public:
    
    vector<ofVec2f> bounds;
    vector<GLine> lines;
    
    ofVec2f center;
    int col;
    
    int index;
    int spiral_id;
    
    void setup(ofVec2f a, ofVec2f b, int _col){
        col = _col;
        float w = ofDist(a.x, a.y, b.x, b.y);
        float h = w*ofRandom(0.65,0.85);//0.75;
        float angle = atan2(b.y-a.y, b.x-a.x);
        float tan_angle = angle - PI/2;
        
        //define bounds
        bounds.push_back(a);
        bounds.push_back(b);
        bounds.push_back(ofVec2f(b.x+cos(tan_angle)*h, b.y+sin(tan_angle)*h ));
        bounds.push_back(ofVec2f(a.x+cos(tan_angle)*h, a.y+sin(tan_angle)*h ));
        
        //define lines
        for (int i=0; i<bounds.size(); i++){
            GLine line;
            line.set(bounds[i], bounds[(i+1)%bounds.size()]);
            vector<GLine> segments = line.get_segments(10);
            for (int k=0; k<segments.size(); k++){
                lines.push_back(segments[k]);
            }
        }
        
        //some vertical lines for style
        int num_spaces = w/3;
        for (int i=1; i<num_spaces; i++){
            float prc = (float)i/(float)num_spaces;
            
            ofVec2f start = bounds[0]*(1.0-prc) + bounds[1]*prc;
            
            ofVec2f end = bounds[1]*(1.0-prc) + bounds[2]*prc;
            float end_length = ofRandom(h*0.3, h*0.75);
            end.x += cos(tan_angle) * end_length;
            end.y += sin(tan_angle) * end_length;
            GLine line;
            line.set(start,end);
            if (ofRandomuf() < 0.6){
                lines.push_back(line);
            }
        }
        
        num_spaces = h/5;
        for (int i=1; i<num_spaces; i++){
            float prc = (float)i/(float)num_spaces;
            
            ofVec2f start = bounds[3]*(1.0-prc) + bounds[0]*prc;
            
            ofVec2f end = bounds[3]*(1.0-prc) + bounds[0]*prc;
            float end_length = ofRandom(h*0.3, h*0.7);
            end.x += cos(angle) * end_length;
            end.y += sin(angle) * end_length;
            GLine line;
            line.set(start,end);
            if (ofRandomuf() < 0.8){
                lines.push_back(line);
            }
        }
//        num_spaces = h/5;
//        for (int i=1; i<num_spaces; i++){
//            float prc = (float)i/(float)num_spaces;
//
//            ofVec2f start = bounds[2]*(1.0-prc) + bounds[3]*prc;
//
//            ofVec2f end = bounds[2]*(1.0-prc) + bounds[3]*prc;
//            float end_length = ofRandom(h*0.3, h*0.7);
//            end.x += cos(tan_angle) * end_length;
//            end.y += sin(tan_angle) * end_length;
//            GLine line;
//            line.set(start,end);
//            if (ofRandomuf() < 0.8){
//                lines.push_back(line);
//            }
//        }
        
        //get the center
        center = ofVec2f(0,0);
        for (int i=0; i<bounds.size(); i++){
            center += bounds[i];
        }
        center /= (float)bounds.size();
    }
    
    void trim(vector<Box> others){
        float min_dist_to_care = 100;
        for (int b=0; b<others.size(); b++){
            
            if (ofDistSquared(center.x,center.y, others[b].center.x,others[b].center.y) < powf(min_dist_to_care, 2)){
            
                for (int i=0; i<lines.size(); i++){
                    lines[i].clip_outside_polygon(others[b].bounds);
                }
            }
        }
    }
    
    void trim(Box other){
        float min_dist_to_care = 100;
            
            if (ofDistSquared(center.x,center.y, other.center.x,other.center.y) < powf(min_dist_to_care, 2)){
                
                for (int i=0; i<lines.size(); i++){
                    lines[i].clip_outside_polygon(other.bounds);
                }
            }
    }
    
    void trim(vector<ofVec2f> bounds){
        float min_dist_to_care = 100;
        for (int i=0; i<lines.size(); i++){
            lines[i].clip_outside_polygon(bounds);
        }
    }
    
    void border_cut(){
        
        float border_dist = 150;
        ofRectangle border = ofRectangle(border_dist,border_dist, ofGetWidth()-border_dist*2, ofGetHeight()-border_dist*2);
        
        int num_inside = 0;
        for (int i=0; i<bounds.size(); i++){
            if (border.inside(bounds[i])){
                num_inside++;
            }
        }
        
        float chance_to_trim = 1;
        
        if(num_inside > 1 && num_inside <4){
            chance_to_trim = 0.75;
        }
        
        
        if (ofRandomuf() < chance_to_trim){
           for (int i=0; i<lines.size(); i++){
                 lines[i].clip_inside_rect(border);
            }
        }
    }
    
    void make_gcode(ofxGCode * gcode){
        for (int i=0; i<lines.size(); i++){
            gcode->line(lines[i]);
        }
    }
    
    void draw(){
        ofBeginShape();
        for (int i=0; i<bounds.size(); i++){
            ofVertex(bounds[i]);
        }
        ofEndShape(true);
    }
    
    
};

#endif /* Box_h */
