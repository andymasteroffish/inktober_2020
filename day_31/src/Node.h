//
//  Node.h
//  inktober_extra_30
//
//  Created by Andrew Wallace on 10/30/20.
//

#ifndef Node_h
#define Node_h

#include "ofMain.h"
#include "ofxGCode.hpp"

class AnglePoint{
public:
    ofVec2f pos;
    float angle;
};



class Node{
public:
    
    float x, y;
    
    
    vector<ofVec2f> outline;
    vector<GLine> lines;
    
    static bool pointSort(  AnglePoint * a, AnglePoint * b ) {
        return a->angle > b->angle;
    }
    
    Node(){
        setup(0,0);
    }
    
    Node(float _x, float _y){
        setup(_x,_y);
    }
    
    void setup(float _x, float _y){
        x=_x;
        y=_y;
    }
    
    void draw(bool show_pnts){
        ofFill();
        //ofSetColor(255,0,0);
        ofDrawCircle(x,y,3);
        
        ofNoFill();
        if (show_pnts){
            ofBeginShape();
            for (int i=0; i<outline.size(); i++){
                ofVertex(outline[i]);
            }
            ofEndShape(true);
        }
        
        for (int i=0; i<lines.size(); i++){
            if (!lines[i].skip_me){
                lines[i].draw();
            }
        }
    }
    
    void sort_pnts(vector<ofVec2f> unordered){
        vector<AnglePoint *> pnts;
        for (int i=0; i<unordered.size(); i++){
            AnglePoint * pnt = new AnglePoint();
            pnt->pos = unordered[i];
            pnt->angle = atan2(y - pnt->pos.y, x - pnt->pos.x);
            pnts.push_back(pnt);
        }
        
        ofSort( pnts, pointSort );
        
        outline.clear();
        for (int i=0; i<pnts.size(); i++){
            outline.push_back(pnts[i]->pos);
        }
    }
    
    void fill(){
        
//        //find the point furthest from the node
//        ofVec2f start_pnt = outline[0];
//        int start_pnt_id = 0;
//        float far_dist = 0;
//        for (int i=0; i<outline.size(); i++){
//            float dist_sq = ofDistSquared(x,y, outline[i].x, outline[i].y);
//            if (dist_sq > far_dist){
//                far_dist = dist_sq;
//                start_pnt = outline[i];
//                start_pnt_id = i;
//            }
//        }
//
//
//        //find the point furhtest from that
//        ofVec2f end_pnt = outline[ (start_pnt_id+outline.size()/2) % outline.size()];
//
//
//        //get the angle of that line
//        float angle = atan2(end_pnt.y-start_pnt.y, end_pnt.x-start_pnt.x);
//        float tan_angle = angle + PI/2;
//
//        float total_dist = start_pnt.distance(end_pnt);
//        float cur_dist = -100;
//
//        while (cur_dist < total_dist + 100){
//
//            float step_dist = ofMap(cur_dist, 0, total_dist, 1.5, 15, true);
//
//            ofVec2f center = ofVec2f(start_pnt);
//            center.x += cos(angle) * cur_dist;
//            center.y += sin(angle) * cur_dist;
//
//            //draw a big long line
//            float line_full_dist = total_dist;
//            ofVec2f a,b;
//            a.x = center.x + cos(tan_angle) * line_full_dist;
//            a.y = center.y + sin(tan_angle) * line_full_dist;
//            b.x = center.x - cos(tan_angle) * line_full_dist;
//            b.y = center.y - sin(tan_angle) * line_full_dist;
//
//            GLine full = GLine(a,b);
//            vector<GLine> segs = full.get_segments(40);
//
//            for (int i=0; i<segs.size(); i++){
//                lines.push_back(segs[i]);
//            }
//
//            cur_dist += step_dist;
//        }
//
//
//        //clip the whole thing
//        for (int i=0; i<lines.size(); i++){
//            lines[i].clip_inside_polygon(outline);
//        }
    }
    
    void make_gcode(ofxGCode * gcode){
        
        //find the point furthest from the node
        ofVec2f start_pnt = outline[0];
        int start_pnt_id = 0;
        float far_dist = 0;
        for (int i=0; i<outline.size(); i++){
            float dist_sq = ofDistSquared(x,y, outline[i].x, outline[i].y);
            if (dist_sq > far_dist){
                far_dist = dist_sq;
                start_pnt = outline[i];
                start_pnt_id = i;
            }
        }
        
        
        //find the point furhtest from that
        ofVec2f end_pnt = outline[ (start_pnt_id+outline.size()/2) % outline.size()];
        
        
        //get the angle of that line
        float angle = atan2(end_pnt.y-start_pnt.y, end_pnt.x-start_pnt.x);
        float tan_angle = angle + PI/2;
        
        float total_dist = start_pnt.distance(end_pnt);
        float cur_dist = -100;
        
        int num_sections = 4;
        float section_size = total_dist / (float)num_sections;
        
        while (cur_dist < total_dist + 100){
            
            int section = CLAMP(cur_dist / section_size, 0, num_sections-1);
            
            float section_dist = cur_dist - section_size*section;
            float step_dist = ofMap(section_dist, 0, section_size, 2, 6, true);
            float thick_spacing = ofMap(section_dist, 0, section_size, 0.7, 0.2, true);
            int thick_layers = num_sections-section;
            
            ofVec2f center = ofVec2f(start_pnt);
            center.x += cos(angle) * cur_dist;
            center.y += sin(angle) * cur_dist;
            
            //draw a big long line
            float line_full_dist = total_dist;
            ofVec2f a,b;
            a.x = center.x + cos(tan_angle) * line_full_dist;
            a.y = center.y + sin(tan_angle) * line_full_dist;
            b.x = center.x - cos(tan_angle) * line_full_dist;
            b.y = center.y - sin(tan_angle) * line_full_dist;
            
            GLine full = GLine(a,b);
            vector<GLine> segs = full.get_segments(40);
            
            //clip the whole thing
            for (int i=0; i<segs.size(); i++){
                segs[i].clip_inside_polygon(outline);
            }
            
            //draw it
            for (int i=0; i<segs.size(); i++){
                if (!segs[i].skip_me){
                    gcode->thick_line(segs[i].a, segs[i].b, thick_spacing, thick_layers);
                }
                //gcode->line(lines[i]);
            }
            
            cur_dist += step_dist;
        }
        
        
        
        
        
        
       
    }
    
};


#endif /* Node_h */
