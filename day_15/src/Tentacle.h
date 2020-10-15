//
//  Tentacle.h
//  inktober_15
//
//  Created by Andrew Wallace on 10/15/20.
//

#ifndef Tentacle_h
#define Tentacle_h

#include "ofMain.h"
#include "ofxGCode.hpp"
#include "Circle.h"

class Tentacle{
public:
    
    vector<ofVec2f> spine;
    vector<ofVec2f> side_a, side_b;
    vector<ofVec2f> border;
    
    vector<Circle> circles;
    
    
    void setup(float base_angle, float target_length){
        
        float total_dist = 0;
        
        float start_dist = 50;
        
        ofVec2f start_pos = ofVec2f(ofGetWidth()/2+cos(base_angle)*start_dist, ofGetHeight()/2+sin(base_angle)*start_dist);
        float angle = base_angle;
        
        ofVec2f pos = ofVec2f(start_pos);
        //ofVec2f prev_pos = ofVec2f(start_pos);
        
        float step_dist = 4; //this should maybe go down over time?
        
        float max_width = 40;
        
        float curve_range = 0.02;
        float curve = 0.01;//ofRandom(-curve_range,curve_range);
        int num_steps = 0;
        
        while (total_dist < target_length){
            num_steps++;
            
            pos.x += cos(angle) * step_dist;
            pos.y += sin(angle) * step_dist;
            total_dist += step_dist;
            
            angle += curve;
            //angle += (1-ofNoise(start_pos.x, start_pos.y, num_steps*0.01) *2) * 0.02;
            angle += (1-ofNoise(base_angle, num_steps*0.01) *2) * 0.017;
            angle += (1-ofNoise(total_dist*0.001) *2) * 0.08;
            
            spine.push_back(pos);
            
            //get the side positions
            float prc_dist = 1.0 - total_dist / target_length;
            prc_dist = powf(prc_dist, 0.7);
            float tan_dist = 1 + prc_dist * max_width;
            float tan_angle = angle + PI/2;
            
            ofVec2f side_a_pos = ofVec2f(pos);
            side_a_pos.x += cos(tan_angle) * tan_dist;
            side_a_pos.y += sin(tan_angle) * tan_dist;
            side_a.push_back(side_a_pos);
            
            ofVec2f side_b_pos = ofVec2f(pos);
            side_b_pos.x -= cos(tan_angle) * tan_dist;
            side_b_pos.y -= sin(tan_angle) * tan_dist;
            side_b.push_back(side_b_pos);
            
            //prev_pos.set(pos);
        }
        
        //combine the sides to make a border
        for (int i=0; i<side_a.size(); i++){
            border.push_back(side_a[i]);
        }
        for (int i=side_b.size()-1; i>=0; i--){
            border.push_back(side_b[i]);
        }
        
        //drop a bunch of circles
        
        
        bool shade_side_a = side_a[0].y > side_b[0].y;
        
        int layer_num = 0;
        for (int i=0; i<spine.size(); i+=2){
            layer_num++;
            
            float spine_prc = 1.0 - (float)i / (float)spine.size();
            
            float circ_size = 8+spine_prc*8;
            float circ_spacing = circ_size;
            
            //get the a nd b positions
            ofVec2f pos_a = side_a[i];
            ofVec2f pos_b = side_b[i];
            
            //get the dist
            float dist = ofDist(pos_a.x, pos_a.y, pos_b.x, pos_b.y);
            int num_circles = (dist / circ_size) + 1;
            float start_val = layer_num%2==0 ? 0: 0.5;
            for (float k= start_val; k<num_circles; k++){
                float prc = k / (float)num_circles;
                ofVec2f mid = (1.0-prc)*pos_a + prc * pos_b;
                
                Circle circ;
                circ.setup(mid, circ_size);
                
                circ.trim_inside(border);
                
                for (int c=0; c<circles.size(); c++){
                    circ.trim_outside(circles[c].border);
                }
                
                //extra lines for shading
                float shade_prc = 0.3;
                if (shade_side_a && prc < shade_prc){
                    circ.add_extra(2);
                }
                if (!shade_side_a && prc > 1.0-shade_prc){
                    circ.add_extra(2);
                }
                circles.push_back(circ);
            }
        }
        
        
    }
    
    void draw(){
        for (int i=0; i<spine.size()-1; i++){
            ofDrawLine(spine[i], spine[i+1]);
        }
        
        for (int i=0; i<border.size()-1; i++){
             ofDrawLine(border[i], border[i+1]);
        }
        
        for(int i=0; i<circles.size(); i++){
            circles[i].draw();
        }
//        for (int i=0; i<side_a.size()-1; i++){
//            ofDrawLine(side_a[i], side_a[i+1]);
//        }
//        for (int i=0; i<side_b.size()-1; i++){
//            ofDrawLine(side_b[i], side_b[i+1]);
//        }
    }
    
    void make_gcode(ofxGCode * gcode_border, ofxGCode * gcode_circles){
        for (int i=0; i<border.size()-1; i++){
            gcode_border->line(border[i], border[i+1]);
        }
        for(int i=0; i<circles.size(); i++){
            circles[i].make_gcode(gcode_circles);
        }
    }
    
};


#endif /* Tentacle_h */
