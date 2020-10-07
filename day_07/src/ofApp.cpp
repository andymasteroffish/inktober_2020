#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    
    ofBackground(250);
    
    gcode.setup(100);
    gcode.show_transit_lines = false;
    gcode.show_path_with_color = false;
    
    for (int i=0; i<4; i++){
        Star star;
        float spread = 0.004;
        if (i==2)   spread = 0.01;
        if (i==3)   spread = 0.1;
        star.setup(ofGetWidth()/2, ofGetHeight()/2, 50+i*90, spread);
        stars.push_back(star);
    }
    
    //define a square
    float rect_w = ofGetWidth()*0.9;
    ofVec2f top_left;
    top_left.x = ofGetWidth()/2 - rect_w/2;
    top_left.y = ofGetHeight()/2 - rect_w/2;
    ofVec2f bot_right;
    bot_right.x = ofGetWidth()/2 + rect_w/2;
    bot_right.y = ofGetHeight()/2 + rect_w/2;
    
    
    //shoot some lines
    for (int start_x=top_left.x-rect_w; start_x<bot_right.x; start_x+=40){
        int offset_count = ofMap( abs(start_x-top_left.x), 0, rect_w, 2, 6);
        for (int o=0; o<offset_count; o++){
            int line_step_dist = 4;
            int line_steps_per_dir = 5;
            int line_counter = 0;
            ofVec2f pos = ofVec2f(start_x+o,top_left.y+o);
            ofVec2f prev_pos = ofVec2f(pos);
            bool go_horz = true;
            while(pos.x < bot_right.x){
                line_counter ++;
                if (line_counter % (line_steps_per_dir*2) < line_steps_per_dir){
                    pos.x += line_step_dist;
                }
                else{
                    pos.y += line_step_dist;
                }
                
                bool can_draw = false;
                
                //make sure it's i the box
                if (pos.x > top_left.x && pos.y > top_left.y && pos.x < bot_right.x && pos.y < bot_right.y){
                    can_draw = true;
                }
                
                //then make sure it clears the shapes
                if (can_draw){
                    GLine line;
                    line.set(prev_pos, pos);
                   
                    for (int s=0; s<stars.size(); s++){
                        if(gcode.checkInPolygon(stars[s].big_outline, prev_pos.x, prev_pos.y) && !gcode.checkInPolygon(stars[s].small_outline, prev_pos.x, prev_pos.y)){
                            can_draw = false;
                        }
                    }
                }
                
                if (can_draw){
                    gcode.line(prev_pos, pos);
                }
                prev_pos.set(pos);
            }
        }
    }
    
    //save the lines
    gcode.sort();
    gcode.save("inktober_07_lines.nc");
    
    gcode.clear();
    //Make some GCode for the stars
    for (int i=0; i<stars.size(); i++){
        stars[i].make_gcode(&gcode);
    }
    
    gcode.sort();
    gcode.save("inktober_07_stars.nc");
}

//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw(){
    
    gcode.draw();
    
    //float rect_w = ofGetWidth()*0.9;
    //ofDrawRectangle(ofGetWidth()/2-rect_w/2, ofGetHeight()/2-rect_w/2, rect_w, rect_w);
    
//    ofSetColor(0);
//    for (int i=0; i<stars.size(); i++){
//        stars[i].draw();
//    }

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
