#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofBackground(250);
    
    for (int i=0; i<NUM_COLS; i++){
        gcode[i].setup(100);
        gcode[i].show_transit_lines = false;
        gcode[i].show_path_with_color = false;
    }
    gcode[0].demo_col.set(ofColor::red);
    gcode[1].demo_col.set(ofColor::green);
    gcode[2].demo_col.set(ofColor::blue);
    
    ofPushMatrix();
    ofTranslate(ofGetWidth()/2, ofGetHeight()/2);
    
    int num_rings = 12;
    float ring_dist = (ofGetHeight()/2 - 150) / num_rings;
    
    for (int d=0; d<num_rings; d++){
        float prc = (float)d / (float)num_rings;
        
        for (int i=0; i<3; i++){
            for (float k=0; k<3; k++){
            
                float offset_dist = 1.5 + (1.0-prc) * ring_dist * 0.4 + k * 0.5;
                float angle = (TWO_PI/3) * i + d *0.1 + (prc) * k * 0.75;
                
                float x = cos(angle) * offset_dist;
                float y = sin(angle) * offset_dist;
                
                gcode[i].circle(x, y, (d+1)*ring_dist);
            }
        }
    }
    
    for (int i=0; i<6; i++){
        float topper_dist = ring_dist * ( (float)num_rings + 1.5);
        float anchor_dist = topper_dist + 50 + (i%2) * 30;
        float angle_range = PI/6 - (i%2) * (PI/16);
        float angle_step = PI/128;
        
        float angle_offset = i* (TWO_PI/6) ;
        
        ofVec2f anchor;
        anchor.x = cos(angle_offset) * anchor_dist;
        anchor.y = sin(angle_offset) * anchor_dist;
        
        float angle_start = angle_offset -angle_range/2;
        ofVec2f prev_pos = ofVec2f(anchor);
        int count = 0;
        for (float a=angle_start; a<=angle_start+angle_range; a+= angle_step){
            
            //arc line
            ofVec2f pos;
            pos.x = cos(a) * topper_dist;
            pos.y = sin(a) * topper_dist;
            
            //gcode[i%NUM_COLS].line(prev_pos, pos);
            prev_pos.set(pos);
            
            //connector line
            gcode[ (i+count)%NUM_COLS ].line(pos, anchor);
            count++;
            
            
        }
        gcode[i%NUM_COLS].line(prev_pos, anchor);
        
    }
    
    
    ofPopMatrix();
    
    
    for (int i=0; i<NUM_COLS; i++){
        gcode[i].sort();
        gcode[i].save("inktober_12_"+ofToString(i)+".nc");
    }
    
    
}

//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw(){
    for (int i=0; i<NUM_COLS; i++){
        gcode[i].draw();
    }
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
