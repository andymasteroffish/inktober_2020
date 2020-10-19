#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofBackground(250);
    
    gcode.setup(100);
    gcode.show_transit_lines = false;
    gcode.show_do_not_reverse = true;
    
    float angle = PI/16;
    
    
    float spacing = 5;
    int num_lines = 20;
    
    float line_length = 400 ;
    
    
    
    //gcode.circle(start_pos.x, start_pos.y, 5);
    
    int num_sections = 4;
    float y_padding = ofGetHeight()*0.2;
    float x_padding = 150;
    
    vector<ofVec2f> first_noise_pnts;
    vector<ofVec2f> prev_noise_pnts;
    vector<ofVec2f> cur_noise_pnts;
    
    vector<ofVec2f> first_horz_pnts;
    vector<ofVec2f> prev_horz_pnts;
    vector<ofVec2f> cur_horz_pnts;
    
    //noise lines
    cur_noise_pnts.clear();
    for (int d=0; d<num_sections; d++){
        angle *= -1;
        float tan_angle = angle + PI/2;
        
        ofVec2f start_pos;
        start_pos.x = ofGetWidth()/2 - cos(angle) * line_length * 0.5;
        float base_y = ofMap(d, 0, num_sections-1, y_padding, ofGetHeight()-y_padding);
        start_pos.y = base_y - sin(angle) * line_length * 0.5;
        
        //noise lines
        cur_noise_pnts.clear();
        for (int i=0; i<num_lines; i++){

            ofVec2f base_pos = ofVec2f (start_pos);
            base_pos.x += cos(tan_angle) * (i-num_lines/2)*spacing;
            base_pos.y += sin(tan_angle) * (i-num_lines/2)*spacing;
            ofVec2f prev_base_pos;

            ofVec2f prev_pos;
            ofVec2f first_pos, last_pos;

            //shoot the noise line
            for (int k=0; k<=line_length; k++){
                base_pos.x += cos(angle);
                base_pos.y += sin(angle);

                float push = ofNoise(i*0.1,k*0.03,d) * spacing * 2;

                ofVec2f pos;
                pos.x = base_pos.x + cos(tan_angle) * push;
                pos.y = base_pos.y + sin(tan_angle) * push;

                if (k>0){
                    gcode.line(prev_pos, pos);
                }

                prev_base_pos.set(base_pos);
                prev_pos.set(pos);
                
                if (k==0)               first_pos.set(pos);
                if (k==line_length-1)   last_pos.set(pos);
            }
            
            //store the end point we'll want to connect to next
            if (d%2==0){
                cur_noise_pnts.push_back(first_pos);
            }else{
                cur_noise_pnts.push_back(last_pos);
            }
            
            if (d==0){
                first_noise_pnts.push_back(last_pos);
            }
            
            //conect it with bezzie
            if (d>0){
                //float bez_offset = 50 + i * 2;
                float bez_ctrl_dist = 200 - i * 8;
                if (d%2==1){
                    ofVec2f top_pnt = prev_noise_pnts[i];
                    ofVec2f ctrl1 = ofVec2f(top_pnt);
                    ctrl1.x -= cos(angle) * bez_ctrl_dist;
                    ctrl1.y -= sin(angle) * bez_ctrl_dist;
                    ofVec2f ctrl2 = ofVec2f(first_pos);
                    ctrl2.x -= cos(-angle) * bez_ctrl_dist;
                    ctrl2.y -= sin(-angle) * bez_ctrl_dist;
                    gcode.bezier(top_pnt, ctrl1, ctrl2, first_pos);
                }
                else{
                    ofVec2f top_pnt = prev_noise_pnts[i];
                    ofVec2f ctrl1 = ofVec2f(top_pnt);
                    ctrl1.x += cos(angle) * bez_ctrl_dist;
                    ctrl1.y += sin(angle) * bez_ctrl_dist;
                    ofVec2f ctrl2 = ofVec2f(last_pos);
                    ctrl2.x += cos(-angle) * bez_ctrl_dist;
                    ctrl2.y += sin(-angle) * bez_ctrl_dist;
                    gcode.bezier(top_pnt, ctrl1, ctrl2, last_pos);
                }
            }
            
            
        }
        
        //transfer the cur points to the prev points list in reverse
        prev_noise_pnts.clear();
        for (int i=cur_noise_pnts.size()-1; i>=0; i--){
            prev_noise_pnts.push_back(cur_noise_pnts[i]);
        }
        
        cout<<"prev lines count "<<prev_noise_pnts.size()<<endl;
        
        //horizontal
        cur_horz_pnts.clear();
        for (int i=0; i<num_lines; i++){
            float y = base_y + (i-num_lines/2) * spacing;
            
            ofVec2f left = ofVec2f(x_padding,y);
            ofVec2f right = ofVec2f(ofGetWidth()-x_padding, y);
            gcode.line(left, right);
            
            //bezier to prev point
            if (d>0){
                float bez_offset = 100 + i * 3;
                if (d%2==1){
                    float crtl_x = right.x + bez_offset;
                    ofVec2f top_pnt = prev_horz_pnts[i];
                    ofVec2f ctrl1 = ofVec2f(crtl_x, top_pnt.y);
                    ofVec2f ctrl2 = ofVec2f(crtl_x, y);
                    gcode.bezier(top_pnt, ctrl1, ctrl2, right);
                }
                else{
                    float crtl_x = left.x - bez_offset;
                    ofVec2f top_pnt = prev_horz_pnts[i];
                    ofVec2f ctrl1 = ofVec2f(crtl_x, top_pnt.y);
                    ofVec2f ctrl2 = ofVec2f(crtl_x, y);
                    gcode.bezier(top_pnt, ctrl1, ctrl2, left);      //looks dope if you put "right" here instead of "left
                }
            }
            
            //store the end point we'll want to connect to next
            if (d%2==0){
                cur_horz_pnts.push_back(ofVec2f(ofGetWidth()-x_padding, y));
            }else{
                cur_horz_pnts.push_back(ofVec2f(x_padding, y));
            }
            
            if (d==0){
                first_horz_pnts.push_back(ofVec2f(x_padding, y));
            }
        }
        
        //transfer the cur points to the prev points list in reverse
        prev_horz_pnts.clear();
        for (int i=cur_horz_pnts.size()-1; i>=0; i--){
            prev_horz_pnts.push_back(cur_horz_pnts[i]);
        }
        
    }
    
    
    
    //end the shape
    vector<ofVec2f> bottom_conntection;
    
    for (int i=0; i<prev_horz_pnts.size(); i++){
        ofVec2f pnt1 = prev_horz_pnts[i];
        ofVec2f ctrl1 = ofVec2f(-300 - i*5, ofGetHeight()  - i*3);
        ofVec2f ctrl2 = ofVec2f(ofGetWidth()+100+i*20, ofGetHeight());
        ofVec2f pnt2 = prev_noise_pnts[i];
        gcode.bezier(pnt1, ctrl1, ctrl2, pnt2, 100);
    }
    
    for (int i=0; i<first_horz_pnts.size(); i++){
        ofVec2f pnt1 = first_horz_pnts[i];
        ofVec2f ctrl1 = ofVec2f(pnt1.x-200,pnt1.y);
        ofVec2f ctrl2 = ofVec2f(0,0);
        ofVec2f pnt2 = ofVec2f(100+i*10, 0);
        gcode.bezier(pnt1, ctrl1, ctrl2, pnt2, 50);
    }
    
    for (int i=0; i<first_noise_pnts.size(); i++){
        ofVec2f pnt1 = first_noise_pnts[i];
        ofVec2f ctrl1 = ofVec2f(pnt1.x+200,pnt1.y);
        ofVec2f ctrl2 = ofVec2f(ofGetWidth(),0);
        ofVec2f pnt2 = ofVec2f(ofGetWidth()-200+i*20, 0);
        gcode.bezier(pnt1, ctrl1, ctrl2, pnt2, 50);
    }
    
    ofRectangle safe_zone;
    safe_zone.set(7, 7, ofGetWidth()-14, ofGetHeight()-14);
    
    gcode.set_outwards_only_bounds(safe_zone);
    
    gcode.sort();
    gcode.save("inktober_19.nc");
    

}

//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw(){
    
    gcode.draw();
    
//    ofNoFill();
//    ofSetColor(255,0,0);
//    ofDrawCircle(ofGetWidth()/2, ofGetHeight()/2, 5);

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
