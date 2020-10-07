//
//  Star.cpp
//  inktober_07
//
//  Created by Andrew Wallace on 10/6/20.
//

#include "Star.hpp"

void Star::setup(float center_x, float center_y, float base_dist,float spread){
    
    big_outline.clear();
    small_outline.clear();
    
    float dist_range = 30;
    float wave_speed = 5;
    
    float outline_padding = 10;
    
    int steps = 100;
    float angle_step = TWO_PI / (float)steps;
    
    ofVec2f prev_pos;
    int ring_count = 10;
    int line_spread = 2;
    for (int d=0; d<ring_count; d++){
        for (int a=0; a<=steps; a++){
            float base_angle = a * angle_step ;
            
            float dist = base_dist + sin(base_angle*wave_speed) * dist_range;
            dist += d*line_spread;
            
            //0.1 spread looks nice here too
            float draw_angle = base_angle + d*spread;
            ofVec2f pos;
            pos.x = center_x + cos(draw_angle) * dist;
            pos.y = center_y + sin(draw_angle) * dist;
            
            if (a > 0){
                GLine line;
                line.set(prev_pos, pos);
                lines.push_back(line);
            }
            
            //should we save the outine?
            if (d==0){
                float outline_dist = dist - outline_padding;
                if (spread > 0.02){
                    outline_dist = base_dist - dist_range + d*line_spread - 7;
                }
                ofVec2f outline_pos;
                outline_pos.x = center_x + cos(draw_angle) * outline_dist;
                outline_pos.y = center_y + sin(draw_angle) * outline_dist;
                small_outline.push_back(outline_pos);
            }
            if (d==ring_count-1){
                float outline_dist = dist + outline_padding;
                if (spread > 0.02){
                    outline_dist = base_dist + dist_range + d*line_spread + 7;
                }
                ofVec2f outline_pos;
                outline_pos.x = center_x + cos(draw_angle) * outline_dist;
                outline_pos.y = center_y + sin(draw_angle) * outline_dist;
                big_outline.push_back(outline_pos);
            }
            
            //store prev pos
            prev_pos.set(pos);
        }
    }
    
}

void Star::draw(){
//    for (int i=0; i<lines.size(); i++){
//        lines[i].draw();
//    }
    ofSetColor(255,0,0);
    for (int i=0; i<small_outline.size()-1; i++){
        ofDrawLine(small_outline[i],small_outline[i+1]);
    }
    for (int i=0; i<big_outline.size()-1; i++){
        ofDrawLine(big_outline[i],big_outline[i+1]);
    }
}

void Star::make_gcode(ofxGCode * gcode){
    for (int i=0; i<lines.size(); i++){
        gcode->line(lines[i]);
    }
}
