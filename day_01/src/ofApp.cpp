#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofBackground(250);
    gcode.setup(90);
    gcode.show_path_with_color = false;
    
    show_circles = false;
    
    loop_end_mod = 1.36;
    
    bend = -1.12;
    
    for(int i=1; i<6; i++){
        Circ circ;
        circ.s = 10+45*i;
        
        float a = PI*0.75 + i * 0.7;
        float d = 70 + (6-i)*45;
        
        circ.x = ofGetWidth()/2 + cos(a)*d;
        circ.y = ofGetHeight()/2 + sin(a)*d + 90;
        circles.push_back(circ);
    }
    
    make_gcode();
    
    gcode.generate_gcode();
    gcode.save("inktober_1.nc");
    
}

//--------------------------------------------------------------
void ofApp::make_gcode(){
    gcode.clear();
    
    float y_spacing = 10;
    float step_dist = .5;
    
    float loop_end = ofGetHeight()* loop_end_mod;
    
    for (float start_y=0; start_y<loop_end; start_y+=y_spacing){
        ofVec2f pos = ofVec2f(0,start_y);
        float prev_y_slope = -step_dist;
        ofVec2f prev_pos = ofVec2f(pos);
        
        while(pos.x < ofGetWidth()){
            float y_slope = -step_dist;
            for (int i=0; i<circles.size(); i++){
                if (ofDistSquared(pos.x,pos.y, circles[i].x, circles[i].y) < powf(circles[i].s,2)){
                    y_slope += step_dist*bend;
                }
            }
            if(y_slope != prev_y_slope){
                prev_y_slope = y_slope;
                gcode.line(prev_pos, pos);
                prev_pos = ofVec2f(pos);
            }
            pos.x += step_dist;
            pos.y += y_slope;
        }
        gcode.line(prev_pos, pos);
    }
    
    for (float start_y=ofGetHeight(); start_y>ofGetHeight()-loop_end; start_y-=y_spacing){
        ofVec2f pos = ofVec2f(ofGetWidth(),start_y);
        float prev_y_slope = -step_dist;
        ofVec2f prev_pos = ofVec2f(pos);

        while(pos.x > 0){
            float y_slope = -step_dist;
            for (int i=0; i<circles.size(); i++){
                if (ofDistSquared(pos.x,pos.y, circles[i].x, circles[i].y) < powf(circles[i].s,2)){
                    y_slope += step_dist*bend;
                }
            }
            if(y_slope != prev_y_slope){
                prev_y_slope = y_slope;
                gcode.line(prev_pos, pos);
                prev_pos = ofVec2f(pos);
            }
            pos.x -= step_dist;
            pos.y -= y_slope;
        }
        gcode.line(prev_pos, pos);
    }
    
    gcode.sort();
}

//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw(){
    gcode.draw();
    
    if (show_circles){
        ofNoFill();
        for (int i=0; i<circles.size(); i++){
            ofDrawCircle(circles[i].x, circles[i].y, circles[i].s);
        }
    }
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    if (key == 'h'){
        show_circles = !show_circles;
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
//    if(selection < circles.size()){
//        circles[selection].x = x;
//        circles[selection].y = y;
//        cout<<"pos: "<<x<<" , "<<y<<endl;
//    }
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
    selection = 99;
    for (int i=0; i<circles.size(); i++){
        if (ofDist(x,y, circles[i].x, circles[i].y) < circles[i].s){
            selection = i;
        }
    }
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
    loop_end_mod = ofMap(x,0,ofGetWidth(), 1.25,1.5);
    cout<<"loop end mod: "<<loop_end_mod<<endl;
    
//    bend = ofMap(x,0,ofGetWidth(), -2,2);
//    cout<<"bend: "<<bend<<endl;
    make_gcode();
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
