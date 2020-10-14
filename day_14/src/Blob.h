//
//  Blob.h
//  inktober_12
//
//  Created by Andrew Wallace on 10/13/20.
//

#ifndef Blob_h
#define Blob_h

#include "ofMain.h"
#include "ofxGCode.hpp"

class Blob{
public:
    
    vector<ofVec2f> pnts;
    
    void reset(){
        pnts.clear();
    }
    
    void make_gcode(ofxGCode * gcode){
        
        vector<ofVec2f> horz_starts;
        vector<GLine> bound_lines;
        float max_x = 0;
        
        gcode->begin_shape();
        for (int i=0; i<pnts.size(); i++){
            
            if (pnts[i].x > max_x){
                max_x = pnts[i].x;
            }
            
            if (i>0 && i<pnts.size()-1 && i%5==0){
                if (pnts[i].y < pnts[i-1].y && pnts[i].y > pnts[i+1].y){
                    horz_starts.push_back(pnts[i]);
                }
            }
            
            if (i>0){
                GLine line;
                line.set(pnts[i-1], pnts[i]);
                bound_lines.push_back(line);
            }
            
            gcode->vertex(pnts[i]);
        }
        gcode->end_shape(false);
        
        
        //the horizontla lines
        for (int h=0; h<horz_starts.size(); h++){
            GLine line;
            line.set(horz_starts[h], horz_starts[h]);
            line.a.x += 1;
            line.b.x = max_x;
            
            for (int i=0; i<bound_lines.size(); i++){
                line.clip_to_other_line(bound_lines[i]);
            }
            
            gcode->line(line);
        }
        
    }
    
    
    
};

#endif /* Blob_h */
