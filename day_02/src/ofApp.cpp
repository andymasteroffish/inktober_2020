#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofBackground(250);
    gcode.setup(110);
    //gcode.show_path_with_color = false;
    
    show_boxes = true;
    
    Box b;
    b.setup(30, 30, ofGetWidth()-30, ofGetHeight()-30);
    boxes.push_back(b);
    
    make_circles();
    
}
//--------------------------------------------------------------
void ofApp::make_circles(){
    circs.clear();
    float padding = 250;
    for (int i=0; i<7; i++){
        Circle c;
        c.x = ofRandom(padding,ofGetWidth()-padding);
        c.y = ofRandom(padding,ofGetHeight()-padding);
        c.s = ofRandom(70,180);
        circs.push_back(c);
    }
}

//--------------------------------------------------------------
void ofApp::split(){
    
    Box * best_option = NULL;
    
    for (int i=0; i<5; i++){
        int index = ofRandom(boxes.size());
        if (i==0){
            best_option = &boxes[index];
        }
        else if (boxes[index].get_size() > best_option->get_size() ){
            best_option = &boxes[index];
        }
    }
    
    Box new_b = best_option->split();
    boxes.push_back(new_b);
    
}

//--------------------------------------------------------------
void ofApp::make_gcode(){
    gcode.clear();
    for (int i=0; i<boxes.size(); i++){
        gcode.rect(boxes[i].top_left.x, boxes[i].top_left.y, boxes[i].bot_right.x-boxes[i].top_left.x, boxes[i].bot_right.y-boxes[i].top_left.y);
    }
    
    gcode.sort();
    gcode.save("inktober_2.nc");
    
}

//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw(){
    gcode.draw();
    
    if (show_boxes){
        ofSetColor(0);
        ofNoFill();
        for (int i=0; i<boxes.size(); i++){
            //ofSetColor(i*25);
            boxes[i].draw();
        }
        
        for (int i=0; i<circs.size(); i++){
            ofDrawCircle(circs[i].x, circs[i].y, circs[i].s);
        }
    }
    //ofDrawCircle(ofGetWidth()/2, ofGetHeight()/2, 250);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    
    if (key == ' '){
        for (int i=0; i<500; i++){
            split();
        }
    }
    
    if (key == 'c'){
        boxes.clear();
        Box b;
        b.setup(10, 10, ofGetWidth()-10, ofGetHeight()-10);
        boxes.push_back(b);
    }
    
    if (key == 'm'){
        make_circles();
    }
    
    if (key == 't'){
        for (int i=boxes.size()-1; i>=0; i--){
            
            bool is_safe = false;
            
            for (int k=0; k<circs.size(); k++){
                if (ofDistSquared(boxes[i].top_left.x, boxes[i].top_left.y, circs[k].x, circs[k].y) < powf(circs[k].s,2)){
                    is_safe = true;
                }
            }
            
            if (!is_safe){
                boxes.erase(boxes.begin() + i);
            }
        }
    }
    
    if (key == 'p'){
        make_gcode();
    }
    
    if (key == 'h'){
        show_boxes = !show_boxes;
    }
    
//    int index = ofRandom(boxes.size());
//    Box new_b = boxes[index].split();
//    boxes.push_back(new_b);
    
//    for (int i=0; i<boxes.size(); i++){
//        cout<<"box "<<i<<endl;
//        cout<<"  "<<boxes[i].top_left<<endl;
//        cout<<"  "<<boxes[i].bot_right<<endl;
//    }
    
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
