#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofBackground(250);
    for (int i=0; i<NUM_COLS; i++){
        gcode[i].setup(100);
        gcode[i].demo_col = ofColor::fromHsb( (255/NUM_COLS) * i, 230, 255);
    }
    
    ofSeedRandom(0);
    
    float spacing = 100;
    float size = 40;
    
    int num_cols = ofGetWidth()/spacing - 1;
    int num_rows = (ofGetHeight()/spacing) * 2 - 2;
    
    int num_depth = 10;
    
    float grid_speed = 0.33;
    float noise_speed = 0.1;
    
    float min_ctrl_noise_power = 0.1;
    float max_ctrl_noise_power = 0.5;
    
    float min_push_noise_power = 0.3;
    float max_push_noise_power = 2;
    
    float max_dist = 1335;
    
    for (int c=1; c<num_cols; c++){
        for (int r=1; r<num_rows; r++){
            
            ofPushMatrix();
            float this_x = c*spacing + (r%2)*spacing*0.5 + spacing*0.25 ;
            float this_y = r*spacing * 0.5 + spacing * 0.5 ;
            ofTranslate(this_x, this_y);
            
            float dist_prc = ofDist(this_x, this_y, 0, 0) / max_dist;
            if (r%2==0) dist_prc = ofDist(this_x, this_y, ofGetWidth(), ofGetHeight()) / max_dist;
            
            dist_prc = powf(dist_prc, 2.5);
            
            float ctrl_noise_power = (1.0-dist_prc) * min_ctrl_noise_power + dist_prc * max_ctrl_noise_power;
            float push_noise_power = (1.0-dist_prc) * min_push_noise_power + dist_prc * max_push_noise_power;
            
            vector<int> palette;
            
            if (r%2==0){
                for (int i=0; i<5; i++){
                    palette.push_back( 1+(int)ofRandom(NUM_COLS-1));
                }
                palette[4] = 0;
                palette[2] = 0;
                //palette[4] = 0;
            }else{
                for (int i=0; i<5; i++){
                    palette.push_back((int)ofRandom(NUM_COLS-1));
                }
                palette[4] = NUM_COLS-1;
                palette[2] = NUM_COLS-1;
                //palette[4] = NUM_COLS-1;
            }
            
            ofVec2f center = ofVec2f(0,0);
            ofVec2f outter_bounds[4];
            outter_bounds[0] = ofVec2f(0,-size);
            outter_bounds[1] = ofVec2f(size,0);
            outter_bounds[2] = ofVec2f(0,size);
            outter_bounds[3] = ofVec2f(-size,0);
            
            
            for (int d=0; d<num_depth; d++){
                float d_prc = (d+1)/(float)num_depth;
                for (int i=0; i<4; i++){
                    ofVec2f a = d_prc * outter_bounds[i] + (1.0-d_prc) * center;
                    ofVec2f b = d_prc * outter_bounds[(i+1)%4]  + (1.0-d_prc) * center;
                    
                    
                    float ctrl_prc1 = 0.5 + (1-ofNoise(c*grid_speed, r*grid_speed, d*noise_speed, 0)*2) * ctrl_noise_power;
                    float ctrl_prc2 = 0.5 + (1-ofNoise(c*grid_speed, r*grid_speed, d*noise_speed, 99)*2) * ctrl_noise_power;
                    ofVec2f c1 = a * (1.0-ctrl_prc1) + b * ctrl_prc1;
                    ofVec2f c2 = a * (1.0-ctrl_prc2) + b * ctrl_prc2;
                    //push it a bit
                    float push_prc = 1.2 +(1-ofNoise(c*grid_speed, r*grid_speed, d*noise_speed, 5)*2) * push_noise_power;
                    c1 = c1 * push_prc;
                    push_prc = 1.2 +(1-ofNoise(c*grid_speed, r*grid_speed, d*noise_speed, 10)*2) * push_noise_power;
                    c2 = c2 * push_prc;
                    
                    int col = palette[d%palette.size()];
                    gcode[col].bezier(a, c1, c2, b);
                    
                    //gcode[1].line(a,b);
                    
                    //gcode[d%NUM_COLS].circle(c1.x, c1.y, 1);
                    //gcode[d%NUM_COLS].circle(c2.x, c2.y, 1);
                }
            }
            
            
            //testing
//            gcode[0].begin_shape();
//            for (int i=0; i<4; i++){
//                gcode[0].vertex(outter_bounds[i]);
//            }
//            gcode[0].end_shape(true);
            
            ofPopMatrix();
            
        }
    }
    
    for (int i=0; i<NUM_COLS; i++){
        gcode[i].sort();
        gcode[i].save("inktober_"+ofToString(i)+".nc");
    }

    //cout<<"max dist: "<<max_dist;
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
