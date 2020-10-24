#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofBackground(250);
    
    gcode.setup(100);
    gcode.show_do_not_reverse = true;
    gcode_alt.setup(100);
    gcode_alt.demo_col = ofColor::red;
    
    ofPushMatrix();
    ofTranslate(0, -10);
    
    float bottom_spacing = 3.5;
    float vertical_spacing = 2.5;
    
    float bottom_y = ofGetHeight()*0.75;
    float vert_y_bottom_base = ofGetHeight() * 0.65;
    float vert_y_bottom_spacing = 20;
    
    float base_height = 500;
    
    int num_bands = 5;
    int lines_per_band = 10;
    
    float bottom_band_spacing = (lines_per_band*bottom_spacing) +  10;
    float vert_in_between_spacing = 8;
    float vertical_band_spacing = (lines_per_band*vertical_spacing) +  vert_in_between_spacing;
    float vert_edge_dist = vertical_band_spacing * num_bands;
    
    ofVec2f bottom_pivot_pnt = ofVec2f(0, ofGetHeight()*0.5);
    float bottom_pivot_angle = PI/4;
    
    //coming in from the sides
    for (int b=0; b<num_bands; b++){
        for (int i=0; i<lines_per_band; i++){
            //bottom line
            float start_y = bottom_y + b*bottom_band_spacing + i*bottom_spacing;
            float y_dist = start_y - bottom_pivot_pnt.y;
            float x_end = bottom_pivot_pnt.x + cos(bottom_pivot_angle) * y_dist;
            gcode.line(0,start_y, x_end, start_y);
            gcode.line(ofGetWidth(),start_y, ofGetWidth()-x_end, start_y);
            
            //vertical
            float vert_y_bottom = vert_y_bottom_base - (num_bands-b-1) * vert_y_bottom_spacing;
            float vert_x = ofGetWidth()/2 - vert_edge_dist + b*vertical_band_spacing + i*vertical_spacing;
            gcode.line(x_end, start_y, vert_x, vert_y_bottom);
            gcode.line(ofGetWidth()-x_end, start_y, ofGetWidth()-vert_x, vert_y_bottom);
            
            float vert_h = base_height - (num_bands-b-1) * vert_y_bottom_spacing;
            float vert_y_top = vert_y_bottom_base-vert_h;
            gcode.line(vert_x, vert_y_bottom, vert_x, vert_y_top);
            gcode.line(ofGetWidth()-vert_x, vert_y_bottom, ofGetWidth()-vert_x, vert_y_top);
            
            //curve
            float dist = vert_in_between_spacing + b*vertical_band_spacing + i*vertical_spacing;
            //left
            ofVec2f pnt1 = ofVec2f(vert_x, vert_y_top);
            ofVec2f pnt2 = ofVec2f(vert_x-dist*2, vert_y_top);
            ofVec2f ctrl1 = ofVec2f(vert_x, vert_y_top-dist*1.25);
            ofVec2f ctrl2 = ofVec2f(pnt2.x, vert_y_top-dist*1.25);
            gcode.bezier(pnt1, ctrl1, ctrl2, pnt2);
            //right
            ofVec2f pnt1r = ofVec2f(ofGetWidth()-vert_x, vert_y_top);
            ofVec2f pnt2r = ofVec2f(ofGetWidth()-(vert_x-dist*2), vert_y_top);
            ofVec2f ctrl1r = ofVec2f(ofGetWidth()-vert_x, vert_y_top-dist*1.25);
            ofVec2f ctrl2r = ofVec2f(pnt2r.x, vert_y_top-dist*1.25);
            gcode.bezier(pnt1r, ctrl1r, ctrl2r, pnt2r);
            
            //vertical 2
            vert_y_bottom = vert_y_bottom_base - num_bands * vert_y_bottom_spacing - b*vert_y_bottom_spacing ;
            gcode.line(pnt2.x, pnt2.y, pnt2.x, vert_y_bottom);
            gcode.line(ofGetWidth()-pnt2.x, pnt2.y, ofGetWidth()-pnt2.x, vert_y_bottom);
            
            
            //top line
            float top_y = bottom_y  - num_bands*bottom_band_spacing;
            start_y = top_y + (num_bands-b-1)*bottom_band_spacing + (lines_per_band-i-1)*bottom_spacing;
            y_dist = start_y - bottom_pivot_pnt.y;
            x_end = bottom_pivot_pnt.x + cos(bottom_pivot_angle) * y_dist;
            gcode.line(0,start_y, x_end, start_y);
            gcode.line(ofGetWidth(),start_y, ofGetWidth()-x_end, start_y);
            
            //connect 'em
            gcode.line(x_end, start_y, pnt2.x, vert_y_bottom);
            gcode.line(ofGetWidth()-x_end, start_y, ofGetWidth()-pnt2.x, vert_y_bottom);

            
        }
    }
    
    for (float s=vert_in_between_spacing ; s<vertical_band_spacing*num_bands+vert_in_between_spacing*1.5; s+=vertical_spacing*2){
        gcode_alt.circle(ofGetWidth()/2,vert_y_bottom_base-base_height/2, s);
    }
    
    //gcode.circle(bottom_pivot_pnt.x, bottom_pivot_pnt.y, 5);
    ofPopMatrix();
    
    gcode_alt.sort();
    gcode_alt.save("inktober_24_red.nc");
    
    float safe_border = 10;
    ofRectangle safe_zone;
    safe_zone.set(safe_border, safe_border, ofGetWidth()-safe_border*2, ofGetHeight()-safe_border*2);
    
    gcode.set_outwards_only_bounds(safe_zone);
    gcode.sort();
    gcode.save("inktober_24_black.nc");

}

//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw(){
    
    gcode.draw();
    gcode_alt.draw();
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
