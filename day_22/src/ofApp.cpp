#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofBackground(250);
    
    string names[NUM_COLS];
    
    for (int i=0; i<NUM_COLS; i++){
        gcode[i].setup(100);
        gcode[i].show_transit_lines = false;
        demo[i] = true;
    }
    gcode[0].demo_col = ofColor(0);
    names[0] = "black";
    gcode[1].demo_col = ofColor(255, 20, 20);
    names[1] = "red";
    gcode[2].demo_col = ofColor(242, 126, 205);
    names[2] = "purple";
    gcode[3].demo_col = ofColor(47, 245, 245);
    names[3] = "blue";
    gcode[4].demo_col = ofColor(242, 174, 63);
    names[4] = "orange";
    gcode[5].demo_col = ofColor(219, 213, 26);
    names[5] = "yellow";
    
    
    
    ofPushMatrix();
    //ofTranslate(-200,-200);
    
    for (int c=0; c<NUM_COLS; c++){
        string file_name = "grid_lines.txt";
        if (c>0)    file_name = "actor"+ofToString(c-1)+".txt";
        ofBuffer buffer = ofBufferFromFile(file_name);
        
        if(buffer.size()) {
            
            for (ofBuffer::Line it = buffer.getLines().begin(), end = buffer.getLines().end(); it != end; ++it) {
                
                string line = *it;
                
                int word_index = 0;
                string words[4];
                //string cur_word = "";
                
                for (int i=0; i<line.length(); i++){
                    if (line[i] == ','){
                        word_index++;
                    }else{
                        words[word_index] += line[i];
                    }
                }
                
                gcode[c].line( ofToFloat(words[0]),ofToFloat(words[1]), ofToFloat(words[2]),ofToFloat(words[3]) );
               
                
            }
            
        }
    }
    
    ofPopMatrix();
    
    for (int i=0; i<NUM_COLS; i++){
        
//        ofRectangle test_box;
//        test_box.set(0,0,ofGetWidth()/2, ofGetHeight()/2);
//        gcode[i].clip_outside(test_box);
        
        gcode[i].sort();
        gcode[i].save("inktober_22_"+names[i]+".nc");
    }
}

//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw(){
    for (int i=0; i<NUM_COLS; i++){
        if (demo[i]){
            gcode[i].draw();
        }
    }
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    if (key == '1') demo[1] = !demo[1];
    if (key == '2') demo[2] = !demo[2];
    if (key == '3') demo[3] = !demo[3];
    if (key == '4') demo[4] = !demo[4];
    if (key == '5') demo[5] = !demo[5];
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
