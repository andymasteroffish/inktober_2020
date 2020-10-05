#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofBackground(250);
    gcode.setup(100);
    gcode.show_path_with_color = false;
    gcode.show_transit_lines = false;
    
    //TAKE 1
    
    if (true){
        float x_padding = 100;
        
        for(float a=-3; a<20; a+=0.15){
            ofVec2f prev_pos;
            bool first_loop = true;
            for (float p=0; p<1; p+=0.01){
                float prc = p;
                ofVec2f pos;
                pos.x = (1.0-prc) * x_padding + prc * (ofGetWidth()-x_padding);
                float angle_max1 = 1 + a * 0.5;
                float angle_max2 = PI*5;
                float offset_y = sin(prc * angle_max1) * (300) + cos(prc * angle_max2) * (80+a*2);
                
                float scaling = sin(prc * PI);
                
                pos.y = ofGetHeight()/2 + offset_y * scaling;
                
                if (first_loop){
                    first_loop = false;
                }
                else{
                    gcode.line(prev_pos,pos);
                }
                prev_pos.set(pos);
            }
        }
    }
    
    gcode.sort();
    gcode.save("inktober_05.nc");
    
    //TAKE 2!
    
    if (false){
        for (int k=0; k<6; k++){
            ofPushMatrix();
            ofTranslate(ofGetWidth()/2, ofGetHeight()/2);

            ofRotateRad( k * (PI/2) );
            ofScale( ofMap(k,0,5,1,0.3));

            ofVec2f prev_pos;
            for (int i=0; i<2001; i++){

                float angle = (float)i * 0.05;

                ofVec2f pos;
                pos.x = sin(angle) * 400;
                pos.y = cos(angle*1.01) * 400;

                if (i > 0){
                    gcode.line(prev_pos, pos);
                }

                prev_pos.set(pos);
            }

            ofPopMatrix();
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
