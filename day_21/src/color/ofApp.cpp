#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofBackground(250);
    
    for (int i=0; i<NUM_COLS; i++){
        gcode[i].setup(100);
        gcode[i].show_transit_lines = false;
    }
    gcode[0].demo_col = ofColor::green;
    gcode[1].demo_col = ofColor::brown;
    
    int angle_steps = 13;//15;
    float angle_step = TWO_PI/angle_steps;
    
    for (int i=0; i<angle_steps; i++){
        for (int k=0; k<angle_steps; k++){
        
            ofVec2f p1, p2, c1, c2;
            float angle1 = i*angle_step + (angle_step/angle_step) * k;
            float angle2 = k*angle_step;
            
            float offset_dist = 10;
            
            p1.x = ofGetWidth()*0.5 + cos(angle1) * offset_dist;
            p1.y = ofGetHeight()*0.25 + sin(angle1) * offset_dist;
            
            p2.x = ofGetWidth()*0.5 + cos(angle2) * offset_dist;
            p2.y = ofGetHeight()*0.75 + sin(angle2) * offset_dist;
            
            
            
            
            
            float dist1 = 400;
            c1.x = p1.x + cos(angle1) * dist1;
            c1.y = p1.y + sin(angle1) * dist1;
            
            c2.x = p2.x + cos(angle2) * dist1;
            c2.y = p2.y + sin(angle2) * dist1;
            
            gcode[k%NUM_COLS].bezier(p1, c1, c2, p2);
        }
    }
    
    for (int c=0; c<2; c++){
        for (int r=0; r<2; r++){
            ofVec2f center;
            center.x = ofGetWidth() * (c==0 ? 0.15 : 0.85);
            center.y = ofGetHeight() * (r==0 ? 0.15 : 0.85);
            int circle_pnts = 20;
            float dist = 40;
            angle_step = TWO_PI/circle_pnts;
            vector<ofVec2f> circ_pnts;
            float angle_offset = PI/4*c + PI/4+r;
            for (int k=0; k<1; k++){
                for (int i=0; i<circle_pnts; i++) {
                    float angle = i * angle_step + k*0.1 +angle_offset;
                    float angle2 = i * angle_step + (3*PI)/4 +angle_offset;
                    
                    float angle3 = i * angle_step * (k+1) + PI;// (3*PI)/4 powf(i * angle_step, 1+k);
                    
                    
                    ofVec2f a;
                    a.x = center.x + cos(angle) * dist;
                    a.y = center.y + sin(angle) * dist;
                    
                    ofVec2f b;
                    b.x = center.x + cos(angle2) * dist;
                    b.y = center.y + sin(angle2) * dist;
                    
                    ofVec2f c1 = center;
                    
                    ofVec2f c2;
                    float c_dist = 100 + k*50;
                    c2.x = center.x + cos(angle3) * c_dist;
                    c2.y = center.y + sin(angle3) * c_dist;
                    
                    gcode[ c!=r?0:1 ].bezier(a, c1, c2, b);
                }
            }
        }
    }
    
    
    for (int i=0; i<NUM_COLS; i++){
        gcode[i].sort();
        gcode[i].save("inktober_21_"+ofToString(i)+".nc");
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
