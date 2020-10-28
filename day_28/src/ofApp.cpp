#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofBackground(250);
    
    ofSeedRandom(10);
    
    spiral_count = 0;
    
    
    
    ofPushMatrix();
    
    for (int i=0; i<NUM_COLS; i++){
        gcode[i].setup(100);
        gcode[i].show_do_not_reverse = true;
    }
    gcode[0].demo_col = ofColor::red;
    gcode[1].demo_col = ofColor::blue;
    gcode[2].demo_col = ofColor::green;
       
    
    float noise = 100;
    for (float x=-noise; x<ofGetWidth()+noise; x += ofGetWidth()/4){
        for (float y=-noise; y<ofGetHeight()+noise; y += ofGetHeight()/4){
            
            
            float this_x = x + ofRandom(-noise, noise);
            float this_y = y + ofRandom(-noise, noise);
            
            float angle_step = ofRandom(0.15,0.25);// 0.2 ;
            if (ofRandomf() < 0.5)  angle_step *= -1;
            
            spiral(this_x, this_y, 300, 0, angle_step);
        }
    }
    
    for (int i=0; i<boxes.size(); i++){
        cout<<"chekcing box "<<i<<" of "<<boxes.size()<<endl;;
        for (int k=0; k<boxes.size(); k++){
            if (boxes[k].index < boxes[i].index){
                boxes[i].trim(boxes[k]);
            }
        }
    }
    
    
    
    
    
    for (int i=0; i<boxes.size(); i++){
        boxes[i].make_gcode(&gcode[boxes[i].col]);
    }
    
    ofRectangle safe;
    safe.set(10,10,ofGetWidth()-20, ofGetHeight()-20);
    
    for (int i=0; i<NUM_COLS; i++){
        gcode[i].set_outwards_only_bounds(safe);
        gcode[i].sort();
        gcode[i].save("inktober_"+ofToString(i)+".nc");
    }
    
    ofPopMatrix();

}

//--------------------------------------------------------------
void ofApp::spiral(float center_x, float center_y, float size, float start_angle, float angle_step){
    
    
    ofVec2f center = ofVec2f(center_x, center_y);
    
    //define the spiral
    vector<ofVec2f> spiral_pnts_raw;
    vector<ofVec2f> spiral_pnts;
    
    ofVec2f cur_pos = ofVec2f(center);
    float cur_dist = 0;
    float cur_angle = start_angle;
    
    
    
    //float step_dist = 5;
    
    //define the spiral
    float max_dist = size;
    while (cur_pos.x > center.x-max_dist && cur_pos.x < center.x+max_dist && cur_pos.y > center.y-max_dist && cur_pos.y < center.y+max_dist){
        spiral_pnts_raw.push_back(cur_pos);
        
        cur_pos.x += cos(cur_angle) * cur_dist;
        cur_pos.y += sin(cur_angle) * cur_dist;
        
        cur_dist += 0.25;
        cur_angle += angle_step;
    }
    
    cout<<"spiral_pnts_raw "<<spiral_pnts_raw.size()<<endl;
    
    //evenly divide
    float section_dist = 3;
    cur_dist = 0;
    int num_steps = 40;
    for (int k=0; k<spiral_pnts_raw.size()-1; k++){
        ofVec2f a = spiral_pnts_raw[k];
        ofVec2f b = spiral_pnts_raw[k+1];
        float dist_to_next = ofDist(a.x,a.y, b.x,b.y);
        float dist_slice = dist_to_next / (float) num_steps;
        for (int i=0; i<num_steps; i++){
            cur_dist += dist_slice;
            if (cur_dist >= section_dist){
                float prc = (float)i / (float)num_steps;
                ofVec2f pos = (1.0-prc)*a + prc * b;
                spiral_pnts.push_back(pos);
                cur_dist -= section_dist;
            }
        }
    }
    
    //build rects
    int cur_index = 0;
    while (cur_index <  spiral_pnts.size()-1){
        cout<<"spiral "<<spiral_count<<" index is "<<cur_index<<" out of "<<spiral_pnts.size()<<endl;
        int step_dist = ofRandom (10,35);
        int next_step = cur_index + step_dist;
        next_step = MIN(next_step, spiral_pnts.size()-1);
        
        ofVec2f a = spiral_pnts[cur_index];
        ofVec2f b = spiral_pnts[next_step];
        
        Box box;
        
        int col = spiral_count;
        if (ofRandomf() < 0.3) col++;
        col = col%NUM_COLS;
        //if (spiral_count % 2 == 0)  col = (col+1) % NUM_COLS;
        
        box.setup(a,b, col);
        box.index = cur_index;
        box.spiral_id = spiral_count;
        
        //if (ofRandomf()<0.85){
            box.border_cut();
        //}
        
        //trim it
//        for (int i=0; i<boxes.size(); i++){
//            box.trim(boxes);
//        }
        
        boxes.push_back(box);
        
        
        cur_index += ofRandom(step_dist/2, step_dist);
    }
    
    spiral_count++;
}

//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw(){
    for (int i=0; i<NUM_COLS; i++){
        gcode[i].draw();
    }
    
    ofSetColor(255,0,0);
    ofNoFill();
    for (int i=0; i<boxes.size(); i++){
       // boxes[i].draw();
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
