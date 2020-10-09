//
//  Shape.cpp
//  inktober_03
//
//  Created by Andrew Wallace on 10/2/20.
//

#include "Shape.hpp"

void Shape::setup(float x, float y, float s){
    float padding = 200;
    center.x = x;
    center.y = y;
    
    //define the shape
    int num_pnts = 6;
    float dist = s;// ofRandom(50,80);
    float start_angle = ofRandom(TWO_PI);
    for (int i=0; i<num_pnts; i++){
        float angle = start_angle + (TWO_PI/num_pnts) * i;
        ofVec2f pnt;
        pnt.x = center.x + cos(angle) * dist;
        pnt.y = center.y + sin(angle) * dist;
        outline.push_back(pnt);
    }
    
    shade(outline[0],outline[1],outline[2], center);
    shade(outline[2],outline[3],outline[4], center);
    shade(outline[4],outline[5],outline[0], center);
    
    
}

void Shape::shade(ofVec2f o0, ofVec2f o1, ofVec2f o2,ofVec2f o3){
    //shade a segment
    vector<ofVec2f> border;
    border.push_back(o0);
    border.push_back(o1);
    border.push_back(o2);
    border.push_back(o3);
    
    float angle = atan2(o3.y-o1.y, o3.x-o1.x) ;
    
    float spacing = 10 + sin(angle) * 7;
    
    
    int num_steps = ofDist(border[0].x, border[0].y, border[3].x, border[3].y) / spacing;
    for (int i=0; i<num_steps; i++){
        float prc = (float)i / (float) num_steps;
        ofVec2f a = prc *border[0] + (1.0-prc) * border[1];
        ofVec2f b = prc *border[3] + (1.0-prc) * border[2];
        GLine line;
        line.set(a, b);
        shading.push_back(line);
    }
}

void Shape::draw(){
    ofBeginShape();
    for (int i=0; i<outline.size(); i++){
        ofVertex(outline[i]);
    }
    ofEndShape(true);
    
    for (int i=0; i<outline.size(); i+=2){
        ofDrawLine(outline[i], center);
    }
    
    for (int i=0; i<shading.size(); i++){
        shading[i].draw();
    }
}
