#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    
    ofBackground(250);
    
    ofRectangle trim = ofRectangle(0,0, 522-138, 543-421);
    
    ofPushMatrix();
    //ofTranslate(-138, -428);
    for (int i=0; i<NUM_COLS; i++){
        gcode[i].setup(100);
        gcode[i].demo_col = ofColor::fromHsb(i*70,200,255);
    }

    int num_trees = NUM_COLS * 2;
    float angle_step = TWO_PI/num_trees;
    for (int i=0; i<num_trees; i++){
        
        float angle =  angle_step*i;
        
        //float spread = ofMap( sin(angle), -1, 1, PI/16, PI/4);
        float spread = ofMap( angle, 0, TWO_PI, PI/16, PI/4);
        
        float dist = 40;
        float x = ofGetWidth()/2 + cos(angle)*dist;
        float y = ofGetHeight()/2 + sin(angle)*dist;
        
        tree(x, y, angle, spread, 0, i%NUM_COLS);
    }
    
    ofPopMatrix();
    
    for (int i=0; i<NUM_COLS; i++){
        //gcode[i].clip_outside(trim);
        gcode[i].sort();
        gcode[i].save("inktober_"+ofToString(i)+".nc");
    }
}

//--------------------------------------------------------------
void ofApp::tree(float x, float y, float angle, float spread, int depth, int col){
    
    int num_leaves = 2;
    
    float dist = 40;
    
    for (int i=0; i<num_leaves; i++){
        float new_angle = i==0 ? angle-spread : angle+spread;
        
        float x2 = x + cos(new_angle) * dist;
        float y2 = y + sin(new_angle) * dist;
        
        gcode[col].line(x,y, x2,y2);
        
        if (depth < 8){
            tree(x2,y2, new_angle, spread, depth+1, col);
        }else{
            float circ_dist = ofRandom(dist * 1.5, dist * 2);
            float wiggle_range = 4;
            float circ_x = x2+cos(new_angle)*dist + ofRandom(-wiggle_range,wiggle_range);
            float circ_y = y2+sin(new_angle)*dist + ofRandom(-wiggle_range,wiggle_range);
            if (ofDistSquared(ofGetWidth()/2, ofGetHeight()/2, circ_x, circ_y) > powf(340,2)){
                gcode[(int)ofRandom(NUM_COLS)].circle(circ_x, circ_y, 4);
            }
        }
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
    cout<<"mouse "<<x<<" , "<<y<<endl;
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
