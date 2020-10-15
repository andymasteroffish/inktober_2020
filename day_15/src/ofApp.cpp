#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofBackground(250);
    
    for (int i=0; i<NUM_COLS; i++){
        gcode[i].setup(100);
        gcode[i].show_transit_lines = false;
        gcode[i].show_do_not_reverse = true;
    }
    gcode[PURPLE].demo_col = ofColor::purple;
    gcode[LIGHT_PURPLE].demo_col = ofColor::magenta;
    gcode[RED].demo_col = ofColor::red;
    gcode[BLACK].demo_col = ofColor::black;
    
    float outter_circle_size = 300;
    int ring_pnts = 100;
    
    //tentacles
    int num_tentacles = 5;
    for (int i=0; i<num_tentacles; i++){
        float angle = 0.5+(TWO_PI/(float)num_tentacles) * i;
        Tentacle tent;
        tent.setup(angle, 600);
        tentacles.push_back(tent);
        
        tent.make_gcode(&gcode[PURPLE], &gcode[LIGHT_PURPLE]);
    }
    
    //rings
    Circle outter_circle;
    outter_circle.setup(ofVec2f(ofGetWidth()/2, ofGetHeight()/2), outter_circle_size, ring_pnts, 0);
    
    
    Circle inner_circle;
    inner_circle.setup(ofVec2f(ofGetWidth()/2, ofGetHeight()/2), 70);
    
    
    //teeth
    int num_triangles = 500;
    for (float i=0; i<num_triangles; i++){
        //float prc = 1.0 - i/(float)num_triangles;
        float angle = 0.47 + i*0.2;//  prc * TWO_PI * 3;
        float dist = outter_circle_size + 30 - i*0.5;//100 + 200 * prc;
        float size = 60 - i*0.08;//20 + 25 * prc;
        Triangle tri;
        tri.setup(angle, dist, size);

        //chop 'em
        tri.trim(triangles, tentacles);
        tri.trim_inside(outter_circle.border);
        tri.trim_outside(inner_circle.border);

        tri.make_gcode(&gcode[RED]);

        triangles.push_back(tri);
    }
    
    //outter ring
    Circle outter_circle2;
    outter_circle2.setup(ofVec2f(ofGetWidth()/2, ofGetHeight()/2), outter_circle_size+30, ring_pnts, PI/16);
    
    
    for (int i=0; i<ring_pnts; i++){
        ofVec2f a = outter_circle.border[i];
        ofVec2f b = outter_circle2.border[i];
        GLine line;
        line.set(a,b);
        for (int t=0; t<tentacles.size(); t++){
            line.clip_outside_polygon(tentacles[t].border);
        }
        gcode[PURPLE].line(line);
    }
    
    for (int t=0; t<tentacles.size(); t++){
        outter_circle.trim_outside(tentacles[t].border);
        outter_circle2.trim_outside(tentacles[t].border);
    }
    
    outter_circle.make_gcode(&gcode[PURPLE]);
    outter_circle2.make_gcode(&gcode[PURPLE]);
    
    //hexagon mouth
    float angle_step = TWO_PI/5;
    for(float d = 5; d<69; d+=4){
        gcode[BLACK].begin_shape();
        for (int i=0; i<5; i++){
            float angle = 0.3 + angle_step*i + sin(d*0.01)*TWO_PI;
            gcode[BLACK].vertex(ofGetWidth()/2+cos(angle)*d, ofGetHeight()/2+sin(angle)*d);
        }
        gcode[BLACK].end_shape(true);
    }
    
    
    //shoot some lines
    int num_noise_lines = 40;
    angle_step = TWO_PI/num_noise_lines;
    
    for (int k=0; k<num_noise_lines; k++){
        float angle = angle_step * k;
        
        float start_dist = outter_circle_size+50;
        if (k%2==0){
            start_dist += 100;
        }
        
        ofVec2f start_pos = ofVec2f(ofGetWidth()/2+cos(angle)*start_dist, ofGetHeight()/2+sin(angle)*start_dist);
        
        ofVec2f pos = ofVec2f(start_pos);
        ofVec2f prev_pos;
        
        int num_steps = 300;//ofRandom(100, 400);
        float stepDist = 2;
        
        float curve_range = 0.02;
        float curve = 0.005;//ofRandom(-curve_range,curve_range);
        
        vector<GLine> lines;
        
        for (int i=0; i<num_steps; i++){
            pos.x += cos(angle) * stepDist;
            pos.y += sin(angle) * stepDist;
            
            angle += curve;
            //angle += (1-ofNoise(start_pos.x, start_pos.y, i*0.005) *2) * 0.02;
            
            if (ofDistSquared(pos.x, pos.y, ofGetWidth()/2, ofGetHeight()/2) < powf(start_dist,2)){
                break;
            }
            
            if (i!=0){
                //if (ofRandomuf() < 0.5){
                    GLine line;
                    line.set(pos, prev_pos);
                    for (int t=0; t<tentacles.size(); t++){
                        line.clip_outside_polygon(tentacles[t].border);
                    }
                    
                    lines.push_back(line);
                    //gcode.line(line);
                //}
            }
            prev_pos.set(pos);
            
            if (pos.x < -10 || pos.x >ofGetWidth()+10 || pos.y < -10 || pos.y >= ofGetHeight()+10){
                break;
            }
        }
        
        //draw it
        bool is_on = ofRandomuf() < 0.5;
        int counter = 0;
        for (int i=0; i<lines.size(); i++){
            counter--;
            if(counter <= 0){
                is_on = !is_on;
                
                if (is_on){
                    counter = ofRandom(5,12);
                }else{
                    counter = ofRandom(4,10);
                }
                
            }
            
            if (is_on){
                gcode[BLACK].line(lines[i]);
            }
        }
        
    }
    
    for (int i=0; i<NUM_COLS; i++){
        ofRectangle safe_zone;
        safe_zone.set(20, 20, ofGetWidth()-20, ofGetHeight()-20);
        gcode[i].set_outwards_only_bounds(safe_zone);
        gcode[i].sort();
        gcode[i].save("inktober_15_"+ofToString(i)+".nc");
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
    
//    ofSetColor(0,100,0);
//    ofNoFill();
//    for (int i=0; i<tentacles.size(); i++){
//        tentacles[i].draw();
//    }
//
//
//    ofSetColor(255,0,0);
//    ofNoFill();
//    for (int i=0; i<triangles.size(); i++){
//        triangles[i].test_draw();
//    }
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
