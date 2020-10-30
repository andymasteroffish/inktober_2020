#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofBackground(250);
    gcode.setup(100);
    gcode_red.setup(100);
    gcode_red.demo_col = ofColor::red;
    
    show_pnts = false;
    show_max_dist = false;
    show_field = true;
    show_grid = true;
    
    cur_dragging = -1;
    
    seed = 18;
    
    
    max_dist = 600;
    
    refresh();
}
//--------------------------------------------------------------
void ofApp::refresh(){
    gcode.clear();
    gcode_red.clear();
    
    ofSeedRandom(seed);
    
    populate(20);
    
    int spacing = 20;
    
    float thick_num = 2;
    float thick_spacing = 0.5;
    
   
    ofPushMatrix();
    //ofTranslate(-500, -300);
    
    //grid
    int num_cols = ofGetWidth()/spacing;
    int num_rows = ofGetHeight()/spacing;
    ofVec2f grid_pos[num_cols][num_rows];
    bool inside_frame[num_cols][num_rows];
    
    float high_total_prc = 0;
    
    //bounding polygon
    vector<ofVec2f> frame, frame2;
    
    float frame_w = ofGetWidth() * 0.45;
    float frame_h = ofGetHeight() * 0.45;
    
    float num_pnts = 60;
    float angle_step = TWO_PI / num_pnts;
    float noise_channel = seed + ofRandom(10);
    for (int i=0; i<num_pnts; i++){
        float angle = angle_step * i;
        ofVec2f pnt;
        float noise_mod = ofMap( ofNoise(i*0.05, noise_channel), 0, 1, 0.6, 1.1);
        pnt.x = ofGetWidth()/2 + cos(angle) * frame_w * noise_mod;
        pnt.y = ofGetHeight()/2 + sin(angle) * frame_h * noise_mod;
        frame.push_back(pnt);
        
        int frame_2_dist = 15;
        noise_mod = ofMap( ofNoise(i*0.05, noise_channel, 0.1), 0, 1, 0.6, 1.1);
        pnt.x = ofGetWidth()/2 + cos(angle) * (frame_w * noise_mod + frame_2_dist);
        pnt.y = ofGetHeight()/2 + sin(angle) * (frame_h * noise_mod + frame_2_dist);
        frame2.push_back(pnt);
    }
    
    //get the frame average pos
    ofVec2f avg = ofVec2f(0,0);
    ofVec2f red_avg = ofVec2f(0,0);
    for (int i=0; i<frame.size(); i++){
        avg += frame[i];
        red_avg += frame2[i];
    }
    avg /= (float)frame.size();
    red_avg /= (float)frame2.size();
    //cout<<"avg    :"<<avg.x<<" , "<<avg.y<<endl;
    //cout<<"as prc :"<<(avg.x/ofGetWidth())<<" , "<<(avg.y/ofGetHeight())<<endl;
    
    ofVec2f delta_from_center = ofVec2f(ofGetWidth()/2, ofGetHeight()/2) - avg;
    ofVec2f delta_from_center_red = ofVec2f(ofGetWidth()/2, ofGetHeight()/2) - red_avg;
    //cout<<"delta "<<delta_from_center<<endl;
    
    for (int i=0; i<num_pnts; i++){
        frame[i] += delta_from_center * 2;
        frame2[i] += delta_from_center_red * 2;
    }
    
    
    //field lines
    for (int c=0; c<num_cols; c++){
        for (int r=0; r<num_rows; r++){
            grid_pos[c][r].set(0,0);
            
            float x = c * spacing;
            float y = r * spacing;
            
            //get the angle
            float angle = PI/2;
            
            float total_strength = 0;
            for (int i=0; i<pnts.size(); i++){
                Node pnt = pnts[i];
                float prc = 1.0 - ofDist(x, y, pnt.x, pnt.y) / max_dist;
                prc = CLAMP(prc, 0, 1);
                float dir = pnt.is_pos ? 1 : -1;
                angle += atan2(pnt.y-y, pnt.x-x) * prc;
                
                total_strength += prc;
            }
            
            
            bool inside = gcode.checkInPolygon(frame, x, y);
            inside_frame[c][r] = gcode.checkInPolygon(frame2, x, y);
            
            if (total_strength > 0.01 && inside){
                //draw it
                float length = ofMap(total_strength, 0, 5, 1, 8, true);
                
                ofPushMatrix();
                ofTranslate(x, y);
                ofRotateRad(angle);
                gcode.thick_line(-length, 0, length, 0, thick_spacing, thick_num);
                //gcode.line(-length,0,length,0);
                
                float curve_angle = 0;
                ofVec2f curve_pos = ofVec2f(length, 0);
                float curve_step =  ofMap(total_strength,0,5.5, 0.1, 3, true);
                float curve_prc = total_strength / 6.0;
                curve_prc = powf(curve_prc, 2);
                float curve_angle_step = ofMap(curve_prc,0,1, 0.1, 0.7, true);
                //gcode.begin_shape();
                ofVec2f prev_pos = ofVec2f(curve_pos);
                for (int i=0; i<5; i++){
                    //gcode.vertex(curve_pos);
                    curve_pos.x += cos(curve_angle) * curve_step;
                    curve_pos.y += sin(curve_angle) * curve_step;
                    curve_angle += curve_angle_step;
                    
                    gcode.thick_line(prev_pos, curve_pos, thick_spacing, thick_num);
                    
                    
                    prev_pos = ofVec2f(curve_pos);
                }
                //gcode.end_shape(false);
                
                //gcode.circle(0, 0, 2);
                ofPopMatrix();
                
                
            }
            
            //mark the grid
            ofVec2f g_pos = ofVec2f(x,y);//ofVec2f(x+spacing*0.5,y+spacing*0.5);
            float offset = 6;
            g_pos.x += cos(angle) * total_strength * offset;
            g_pos.y += sin(angle) * total_strength * offset;
            grid_pos[c][r] = g_pos;
            
            
            
            //testing
            if (total_strength > high_total_prc){
                high_total_prc = total_strength;
            }
        }
    }
    
    cout<<"highest prc: "<<high_total_prc<<endl;
    
    //make the grid
    int num_segments = 8;
    int seg_spacing = 2;
    for (int x=0; x<num_cols; x++){
        for (int y=0; y<num_rows; y++){
            if (inside_frame[x][y]){
                //gcode_red.circle(grid_pos[x][y].x, grid_pos[x][y].y, 2);
                if (x<num_cols-1){
                    if (inside_frame[x+1][y]){
                        GLine line = GLine(grid_pos[x][y], grid_pos[x+1][y]);
                        vector<GLine> segs = line.get_segments(num_segments);
                        for (int i=0; i<segs.size(); i+=seg_spacing){
                            gcode_red.line(segs[i]);
                        }
                    }
                }
                    
                if (y<num_rows-1){
                     if (inside_frame[x][y+1]){
                        GLine line = GLine(grid_pos[x][y], grid_pos[x][y+1]);
                        vector<GLine> segs = line.get_segments(num_segments);
                        for (int i=0; i<segs.size(); i+=seg_spacing){
                            gcode_red.line(segs[i]);
                        }
                     }
                }
            }
        }
    }
    
    ofPopMatrix();
    
     ofRectangle testo = ofRectangle(1,1, 300, 250);
    
    //save
    //gcode.clip_outside(testo);
//    gcode.sort();
//    gcode.save("field.nc");
//    //gcode_red.clip_outside(testo);
//    gcode_red.sort();
//    gcode_red.save("grid.nc");
    
    
}

//--------------------------------------------------------------
void ofApp::populate(int target_num){
    pnts.clear();
    
    int num_selections = 3;
    float padding = 100;
    
    while (pnts.size() < target_num){
        
        vector<ofVec2f> possible;
        for (int i=0; i<num_selections; i++){
            possible.push_back(ofVec2f(ofRandom(-padding,ofGetWidth()+padding), ofRandom(-padding,ofGetHeight()+padding)));
        }
        
        ofVec2f best_pos = possible[0];
        float best_dist_sq = 99999;
        for (int k=0; k<possible.size(); k++){
            
            float closest_dist_sq = 999999;
            
            for (int i=0; i<pnts.size(); i++){
                float dist_sq = ofDistSquared(pnts[i].x, pnts[i].y, possible[k].x, possible[k].y);
                if (dist_sq < closest_dist_sq){
                    closest_dist_sq = dist_sq;
                }
            }
            
            if (closest_dist_sq > best_dist_sq){
                best_dist_sq = closest_dist_sq;
                best_pos = possible[k];
            }
        }
        
        Node new_pnt;
        new_pnt.setup(best_pos.x, best_pos.y, ofRandomf() > 0.5);
        pnts.push_back(new_pnt);
        
    }
    
    
    
}

//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw(){
    
    if (show_grid)  gcode_red.draw();
    if (show_field) gcode.draw();
    
    ofNoFill();
    
    for (int i=0; i<pnts.size(); i++){
        if (pnts[i].is_pos){
            ofSetColor(255,0,0);
        }else{
             ofSetColor(0,0,255);
        }
        if (show_pnts)      ofDrawCircle(pnts[i].x, pnts[i].y, 10);
        if(show_max_dist)   ofDrawCircle(pnts[i].x, pnts[i].y, max_dist);
    }
   

}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
//    if (key == 'm') show_max_dist = !show_max_dist;
//    if (key == 'p') show_pnts = !show_pnts;
//    if (key == 'f') show_field = !show_field;
//    if (key == 'g') show_grid = !show_grid;
    
    if (key == 'p'){
        gcode.sort();
        gcode.save("field.nc");
        gcode_red.sort();
        gcode_red.save("grid.nc");
    }
    
    if (key == 'r'){
        seed++;
        cout<<"new seed "<<seed<<endl;
        refresh();
    }
    
    if (key == '1') pnts.push_back(Node(ofGetMouseX(), ofGetMouseY(), true));
    if (key == '2') pnts.push_back(Node(ofGetMouseX(), ofGetMouseY(), false));
    
    //refresh();
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
    if (cur_dragging >= 0){
        pnts[cur_dragging].x = x;
        pnts[cur_dragging].y = y;
    }
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
    for (int i=0; i<pnts.size(); i++){
        if (ofDist(pnts[i].x, pnts[i].y, x, y) < 10){
            cur_dragging = i;
        }
    }
    
    cout<<"mouse "<<x<<" , "<<y<<endl;
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
    if (cur_dragging >= 0){
        refresh();
        cur_dragging = -1;
    }
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
