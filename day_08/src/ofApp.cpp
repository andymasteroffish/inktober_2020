#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofBackground(250);
    gcode.setup(100);
    gcode.show_transit_lines = false;
    gcode.show_path_with_color = false;
    
    orb(ofGetWidth()/2, ofGetHeight()*0.4);
    
    for (int i=0; i<100; i++){
        int rand = ofRandom(bottom_pnts.size());
        ofVec2f start = bottom_pnts[rand];
        bottom_pnts.erase(bottom_pnts.begin()+rand);
        gcode.line(start.x, start.y, start.x, start.y + ofRandom(25, 300));
    }
    
    
    
}

//--------------------------------------------------------------
void ofApp::orb(float start_x, float start_y){
    
    int num_steps = 250;
    float step_dist = 2;
    
    for (float a=0; a<TWO_PI; a+=0.025){
        float base_angle = a;
        ofVec2f prev_pos = ofVec2f(start_x, start_y);
        ofVec2f prev_base_pos = ofVec2f(start_x, start_y);
        for (int i=0; i<num_steps; i++){
            float prc = (float)i / (float)num_steps;
            float angle_mod = prc * (PI);
            float angle = base_angle + angle_mod;
            
            float tan_angle = angle + PI/2;
            float noise_a = sin(a);
            float tan_dist = 2*(ofNoise(noise_a*1.5,i*0.03)-0.5) * 20;
            
            float amp = sin(prc*PI);
            tan_dist *= amp;
            
            
            ofVec2f base_pos;
            base_pos.x = prev_base_pos.x + cos(angle) * step_dist;
            base_pos.y = prev_base_pos.y + sin(angle) * step_dist;
            
            ofVec2f pos;
            pos.x = base_pos.x + cos(tan_angle) * tan_dist;
            pos.y = base_pos.y + sin(tan_angle) * tan_dist;
            
            gcode.line(prev_pos, pos);
            
            prev_pos.set(pos);
            prev_base_pos.set(base_pos);
            
            if (i==num_steps-1 && pos.y > ofGetHeight()*0.4){
                bottom_pnts.push_back(pos);
            }
        }
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
    if (key == 's'){
        gcode.sort();
        gcode.save("inktober_08.nc");
    }
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
