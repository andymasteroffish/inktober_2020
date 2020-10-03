#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){

    ofBackground(250);
    gcode.setup(100);
    gcode.show_path_with_color = false;
    gcode.show_transit_lines = false;
    
    //cones
    float cone_x_spread = 150;
    float cone_y_dist_from_top = 40;
    float cone_y_dist_from_center = 100;
    
    //top cone
    cone1.push_back(ofVec2f(ofGetWidth()/2-cone_x_spread, cone_y_dist_from_top));
    cone1.push_back(ofVec2f(ofGetWidth()/2+cone_x_spread, cone_y_dist_from_top));
    cone1.push_back(ofVec2f(ofGetWidth()/2, ofGetHeight()/2-cone_y_dist_from_center));
    draw_cone(cone1[0],cone1[1],cone1[2]);
    
    //bottom cone
    cone2.push_back(ofVec2f(ofGetWidth()/2-cone_x_spread, ofGetHeight()-cone_y_dist_from_top));
    cone2.push_back(ofVec2f(ofGetWidth()/2+cone_x_spread,  ofGetHeight()-cone_y_dist_from_top));
    cone2.push_back(ofVec2f(ofGetWidth()/2, ofGetHeight()/2+cone_y_dist_from_center));
    draw_cone(cone2[0], cone2[1], cone2[2]);
    
    
    gcode.sort();
    gcode.save("inktober_03_cones.nc");
    gcode.clear();
    
    //orbit
    orbit();
    
    //actually add the orbit lines to gcode
    for (int i=0; i<orbit_lines.size(); i++){
        gcode.line(orbit_lines[i]);
    }
    
    gcode.sort();
    gcode.save("inktober_03_orbit.nc");
    gcode.clear();
    
    //crackles
    for (int d=0; d<6; d++){
        for (int i=0; i<300+d*200; i++){
            float dist = ofRandom(30+d*30,d*50);
            float angle = ofRandom(TWO_PI);

            crackle(ofGetWidth()/2+cos(angle)*dist, ofGetHeight()/2+sin(angle)*dist);
        }
    }

    //actually add the crackle lines to gcode
    for (int i=0; i<crackle_lines.size(); i++){
        gcode.line(crackle_lines[i]);
    }
    
    gcode.sort();
    gcode.save("inktober_03_crackle.nc");
    gcode.clear();
    
    
    
}

//--------------------------------------------------------------
void ofApp::orbit(){
    float resolution = 100;
    float angle_step = TWO_PI/resolution;
    
    bool is_on = false;
    int switch_counter = 0;
    
    int stroke = 1;
    float stroke_dist = 2;
    
    float angle = 0;
    ofVec2f prev_pos;
    bool first_draw = true;
    for( float y_prc=-1; y_prc<1; y_prc += 0.0003){
        
        float prc_angle = y_prc*PI;
        
        float y_center = ofGetHeight()/2 + y_prc * 300;
        float x_dist = 100+abs(y_prc) * 300;
        float y_dist = 50;
        
        angle += angle_step;
        
        ofVec2f pos;
        pos.x = ofGetWidth()/2 + cos(angle) * x_dist;
        pos.y = y_center + sin(angle) * y_dist;
        
        if (first_draw){
            first_draw = false;
        }
        else{
            bool can_draw = true;
            
            if (!is_on){
                can_draw = false;
            }
            if (abs(y_prc) < 0.2){
                can_draw = false;
            }
            if (sin(angle) < 0 ){
                if(gcode.checkInPolygon(cone1, pos.x, pos.y) || gcode.checkInPolygon(cone2, pos.x, pos.y)){
                    can_draw = false;
                }
                if(gcode.checkInPolygon(cone1, prev_pos.x, prev_pos.y) || gcode.checkInPolygon(cone2, prev_pos.x, prev_pos.y)){
                    can_draw = false;
                }
            }
            
            if (can_draw){
                for (int s=0; s<stroke; s++){
                    GLine line;
                    line.set(prev_pos.x, prev_pos.y+s*stroke_dist, pos.x, pos.y+s*stroke_dist);
                    //gcode.line(line);
                    orbit_lines.push_back(line);
                }
            }
        }
        
        prev_pos.set(pos);
        
        switch_counter --;
        if (switch_counter < 0){
            is_on = !is_on;

            if (is_on)  switch_counter = ofRandom(3,5);
            else        switch_counter = ofRandom(5,10);
            
            stroke = ofRandom(1,4);
        }
    }
    
}

//--------------------------------------------------------------
void ofApp::draw_cone(ofVec2f a, ofVec2f b, ofVec2f c){
    int num_lines = 40;
    for (int i=0; i<num_lines; i++){
        float prc = (float)i / (float)num_lines;
        prc = powf(prc,2);
        ofVec2f start = (1-prc) * a + prc * b;
        gcode.line(start, c);
        
        if (i%2==0){
            ofVec2f start2 = (1-prc) * b + prc * c;
            gcode.line(start2, a);
        }
    }
    
    gcode.line(a, b);
    gcode.line(b, c);
    gcode.line(c, a);
    
}

//--------------------------------------------------------------
void ofApp::crackle(float startX, float startY){
    int num_connections = 5;
    float dist = 5;
    
    ofVec2f prev_pos = ofVec2f(startX, startY);
    //make sure it is not in the cones
    if (gcode.checkInPolygon(cone1, prev_pos.x, prev_pos.y) || gcode.checkInPolygon(cone2, prev_pos.x, prev_pos.y)){
        return;
    }
    
    float angle = ofRandom(TWO_PI);
    
    // float angle_range = PI/3;
    float angle_step = PI/4;
    
    for (int i=0; i<num_connections; i++){
        
        int steps = ofRandom(-3,4);
        angle += steps * angle_step;// ofRandom(-angle_range, angle_range);
        
        ofVec2f pos;
        pos.x = prev_pos.x + cos(angle) * dist;
        pos.y = prev_pos.y + sin(angle) * dist;
        
        GLine line;
        line.set(prev_pos, pos);
        
        //make sure it is not in the cones
        if (gcode.checkInPolygon(cone1, pos.x, pos.y) || gcode.checkInPolygon(cone2, pos.x, pos.y)){
            return;
        }
        
        //make sure it does not intersect other lines
        for (int k=0; k<crackle_lines.size(); k++){
            if (line.intersects(crackle_lines[k])){
                return;
            }
        }
        
        //make sure it is not intersecting orbit lines
        for (int k=0; k<orbit_lines.size(); k++){
            if (line.intersects(orbit_lines[k])){
                return;
            }
        }
        
        
        crackle_lines.push_back(line);
        
        prev_pos.set(pos);
        
    }
}

//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw(){
    gcode.draw();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
