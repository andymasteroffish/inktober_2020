#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    
    ofBackground(250);
    
    for (int i=0; i<NUM_GCODES; i++){
        gcode[i].setup(100);
        gcode[i].show_path_with_color = false;
        gcode[i].show_transit_lines = false;
    }
    
    vector<vector<ofVec2f> > outlines;
    
    ofPushMatrix();
    ofTranslate(50, 100);
    ofScale(0.9,0.9);
    
    //cones
    vector<ofVec2f> tops;
    tops.push_back(ofVec2f(183, 354));
    tops.push_back(ofVec2f(730, 398));
    tops.push_back(ofVec2f(568 , 218));
    outlines.push_back( cone( ofVec2f(287,846), tops[0], ofVec2f(410,800), 1));
    outlines.push_back( cone( ofVec2f(595,814), tops[1], ofVec2f(503,727), 2));
    outlines.push_back( cone( ofVec2f(482 , 705), tops[2], ofVec2f(363 , 671), 3));
    
    
    //grass
    ofVec2f center = ofVec2f(438, 880);
    float width = 350;
    float height = 250;
    //gcode.circle(center.x, center.y, width);
    //gcode.circle(center.x, center.y, height);
    for (int i=0; i<6000; i++){
        
        float angle = ofRandom(TWO_PI);
        float dist_prc = powf( ofRandomuf(), 0.5);
        
        float x = center.x + cos(angle) * dist_prc * width;
        float y = center.y + sin(angle) * dist_prc * height;
        
        float chance_to_show = ofMap(y, center.y-height, center.y+height, 1.3, -0.5);
        
        //is it inside a cone?
        for (int k=0; k<outlines.size(); k++){
            if (gcode[0].checkInPolygon(outlines[k], x, y)){
                chance_to_show = 0;
            }
        }
        
        if (ofRandomuf() < chance_to_show){
            float length = ofRandom(6,14);
            float stalk_angle = -PI/2 + ofRandom(-0.5,0.5);
            gcode[0].line(x,y, x+cos(stalk_angle)*length, y+sin(stalk_angle)*length);
        }
    }
    
    //orbs
    
    for (int o=0; o<3; o++){
        for (int t=0; t<tops.size(); t++){
            center.x = tops[t].x;
            center.y = tops[t].y - 100;
            
            float offset_angle = o * (TWO_PI/3);
            float offset_dist = 5;
            center.x += cos(offset_angle) * offset_dist;
            center.y += sin(offset_angle) * offset_dist;
            
            int num_rings = 10;
            for (int d=1; d<=num_rings; d++){
                
                float d_prc = (float)d / (float)num_rings;
                
                float dist  = d_prc * 200;
                
                int num_pnts = 80;
                float angle_step = TWO_PI / (float)num_pnts;
                
                for (float a=0; a<TWO_PI; a+=angle_step){
                    
                    float x1 = center.x + cos(a) * dist;
                    float y1 = center.y + sin(a) * dist;
                    float x2 = center.x + cos(a+angle_step) * dist;
                    float y2 = center.y + sin(a+angle_step) * dist;
                    
                    float chance_to_show = ofMap(d, 1, num_rings, 0.7, 0.03);
                    
                    //is it inside a cone?
                    for (int k=0; k<outlines.size(); k++){
                        if (gcode[0].checkInPolygon(outlines[k], x1, y1) || gcode[0].checkInPolygon(outlines[k], x2, y2)){
                            chance_to_show = 0;
                        }
                    }
                    
                    if (ofRandomuf() < chance_to_show){
                        gcode[0].line(x1,y1, x2,y2);
                    }
                    
                }
                
            }
        }
        
        
    }
    
    ofPopMatrix();
    
    for (int i=0; i<NUM_GCODES; i++){
        gcode[i].sort();
        gcode[i].save("inktober_06_"+ofToString(i)+".nc");
    }
}

//--------------------------------------------------------------
vector<ofVec2f> ofApp::cone(ofVec2f end_a, ofVec2f top, ofVec2f end_b, int code_id){
    
    gcode[code_id].line(end_a,top);
    gcode[code_id].line(top,end_b);
    
    int num_lines = 10;
    
    vector<ofVec2f> bottom_curve;
    
    for (int i=0; i<num_lines-1; i++){
        float prc1 = (float)i / (float)num_lines;
        prc1 = powf(prc1, 1.25);
        float prc2 = prc1;// + 0.1;
        
        ofVec2f a = (1.0-prc1) * end_a + prc1 * top;
        ofVec2f b = (1.0-prc2) * end_b + prc2 * top;
        
        int steps = 40;
        float angle_step = PI/steps;
        ofVec2f prev = ofVec2f(a);
        if (i==0){
            bottom_curve.push_back(prev);
        }
        for (int k=1; k<=steps; k++){
            float step_prc = (float)k / (float) steps;
            float angle = k*angle_step;
            
            ofVec2f mid = (1.0-step_prc) * a + step_prc * b;
            mid.y += sin(angle) * (10 + (1.0-prc1) * 20);
            
            gcode[code_id].line(prev, mid);
            if (i!=0){
                gcode[code_id].line(mid.x,mid.y, mid.x, mid.y+5);
            }
            prev.set(mid);
            
            if (i==0){
                bottom_curve.push_back(mid);
            }
            
            int draw_point = 2 + (1.0-prc1)*8;
            int end_point = draw_point - 2;// (1.0-prc1) * 8;
            if (i > 0 && i<num_lines-1  && (k == draw_point) ){
                gcode[code_id].line(mid, bottom_curve[end_point]);
            }
        }
    }
    
    //ring the tip
    num_lines = 20;
    for (int i=1; i<num_lines-1; i++){
        float prc = (float)i / (float)num_lines;
        prc = ofMap(prc, 0, 1, 0.85, 1);
        
        ofVec2f a = (1.0-prc) * end_a + prc * top;
        ofVec2f b = (1.0-prc) * end_b + prc * top;
        
        gcode[code_id].line(a,b);
    }
    
    //put it into points
    vector<ofVec2f> pnts;
    pnts.push_back(end_b);
    pnts.push_back(top);
    pnts.push_back(end_a);
    for (int i=0; i<bottom_curve.size(); i++){
        pnts.push_back(bottom_curve[i]);
    }
    
    return pnts;
    
}

//--------------------------------------------------------------
void ofApp::update(){
    cout<<ofGetWidth()<<" , "<<ofGetHeight()<<endl;
}

//--------------------------------------------------------------
void ofApp::draw(){

    for (int i=0; i<NUM_GCODES; i++){
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
     cout<<ofGetMouseX()<<" , "<<ofGetMouseY()<<endl;
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
