#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofBackground(250);
    
    ofSeedRandom(100);
    
    for (int i=0; i<NUM_COLS; i++){
        gcode[i].setup(100);
        gcode[i].demo_col = ofColor::fromHsb(i*70,255,150);
    }
    
    float spacing = 110;
    for (float x=0; x<ofGetWidth(); x+=spacing){
        for (float y=0; y<ofGetHeight(); y+=spacing){
            draw_triangle(x, y);
        }
    }
    
    //defining a large triangle to mask
    ofVec2f top_left = ofVec2f(ofGetWidth()*0.35, ofGetHeight()*0.1);
    ofVec2f top_right = ofVec2f(ofGetWidth()*0.65, top_left.y);
    ofVec2f left = ofVec2f(ofGetWidth()*0.1, ofGetHeight()*0.9);
    ofVec2f right = ofVec2f(ofGetWidth()-left.x, left.y);
    
    
    vector<ofVec2f> left_pnts;
    left_pnts.push_back(ofVec2f(-100,top_left.y));
    left_pnts.push_back(top_left);
    left_pnts.push_back(left);
    left_pnts.push_back(ofVec2f(-100,left.y+10));
    
    vector<ofVec2f> right_pnts;
    right_pnts.push_back(ofVec2f(ofGetWidth()+100,top_right.y));
    right_pnts.push_back(top_right);
    right_pnts.push_back(right);
    right_pnts.push_back(ofVec2f(ofGetWidth()+100,right.y+10));
    
    
    
    for (int i=0; i<NUM_COLS; i++){
        gcode[i].clip_inside(ofRectangle(0,0,ofGetWidth()+100,top_left.y));
        gcode[i].clip_inside(ofRectangle(0,left.y,ofGetWidth()+100,500));
        gcode[i].clip_inside(left_pnts);
        gcode[i].clip_inside(right_pnts);
        
        gcode[i].sort();
        gcode[i].save("inktober_25_"+ofToString(i)+".nc");
    }

}


//--------------------------------------------------------------
void ofApp::draw_triangle(float center_x, float center_y){
    //float center_x = ofRandom(ofGetWidth());
    //float center_y = ofRandom(ofGetHeight());
    
    float height = 150;
    float y_spacing = 4;
    float color_spacing = 1;
    
    float angle_range = PI/32;
    float offset_range = 0;//5;
    
    float base_angle = ofRandom(TWO_PI*0.3);
    
    for (int c=0; c<NUM_COLS; c++){
        ofPushMatrix();
        ofTranslate(center_x, center_y);
        ofRotateRad(ofRandom(-angle_range,angle_range) + base_angle);
        for (float i=0; i<=height; i+=y_spacing){
            float y = i-height/2 + color_spacing*c + 50 + ofRandom(-offset_range, offset_range);
            float w = i/2 + ofRandom(-offset_range, offset_range);
            //gcode[c].line(-w,y, w,y);
            gcode[c].thick_line(-w,y, w,y, 0.1, 2);
        }
        
        
        ofPopMatrix();
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
