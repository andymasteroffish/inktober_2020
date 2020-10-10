#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofBackground(250);
    
    gcode.setup(100);
    gcode.show_transit_lines = false;
    gcode.show_path_with_color = false;
    gcode.show_do_not_reverse = true;
    
    main_circle_dist = ofGetHeight()*0.3;
    
    float grid_size = 100;
    int cols = ofGetWidth()/grid_size;
    int rows = ofGetHeight()/grid_size;
    
    for (int c=1; c<cols-1; c++){
        for (int r=1; r<rows-1; r++){
            for (int d=0; d<4; d++){
                //gcode.rect(c*grid_size, r*grid_size, grid_size, grid_size);
                float start_dist = 3 + d * 5;
                for (float dist=start_dist; dist<start_dist+4; dist++){
                
                    ofVec2f center = ofVec2f(c*grid_size+grid_size/2, r*grid_size+grid_size/2);
                    
                    float pnt_dist = grid_size * 0.25;
                    ofVec2f pivot_a, pivot_b;
                    
                    if ( (c+r) % 2 == 0){
                        pivot_a.set(center.x-pnt_dist, center.y-pnt_dist);
                        pivot_b.set(center.x+pnt_dist, center.y+pnt_dist);
                    }else{
                        pivot_a.set(center.x-pnt_dist, center.y+pnt_dist);
                        pivot_b.set(center.x+pnt_dist, center.y-pnt_dist);
                    }
                    //gcode.circle(pivot_a.x, pivot_a.y, 3);
                    //gcode.circle(pivot_b.x, pivot_b.y, 1);
                    
                    //section_lines.clear();
                    
                    if (c%2==0 && r%2 == 1){
                        //coming from the top
                        float start_x = center.x - pnt_dist + dist;
                        float start_y = r*grid_size;
                        line_shoot(c, r, grid_size, start_x, start_y, dist, 1, true, pivot_a, d);
                        
                        //coming from the left
                        start_x = c*grid_size;
                        start_y = center.y - pnt_dist + dist;
                        line_shoot(c, r, grid_size, start_x, start_y, dist, 0, false, pivot_b, d);
                    
                    }
                    
                    if (c%2==1 && r%2 == 1){
                        //coming from the left
                        float start_x = c*grid_size;
                        float start_y = center.y + pnt_dist - dist;
                        line_shoot(c, r, grid_size, start_x, start_y, dist, 0, true, pivot_b, d);
                        
                        //coming from the bottom
                        start_x = center.x - pnt_dist + dist;
                        start_y = r*grid_size + grid_size;
                        line_shoot(c, r, grid_size, start_x, start_y, dist, 3, false, pivot_a, d);
                        
                    }

                    if (c%2==1 && r%2 == 0){
                        //coming from the bottom
                        float start_x = center.x + pnt_dist - dist;
                        float start_y = r*grid_size+grid_size;
                        line_shoot(c, r, grid_size, start_x, start_y, dist, 3, true, pivot_b, d);

                        //coming from the right
                        start_x = c*grid_size + grid_size;
                        start_y = center.y + pnt_dist - dist;
                        line_shoot(c, r, grid_size, start_x, start_y, dist, 2, false, pivot_a, d);
                    }
                    if (c%2==0 && r%2 == 0){
                        //coming from the right
                        float start_x = c*grid_size + grid_size;
                        float start_y = center.y - pnt_dist + dist;
                        line_shoot(c, r, grid_size, start_x, start_y, dist, 2, true, pivot_a, d);

                        //coming from the top
                        start_x = center.x + pnt_dist - dist;
                        start_y = r*grid_size;
                        line_shoot(c, r, grid_size, start_x, start_y, dist, 1, false, pivot_b, d);
                    }
                }
            }
            
        }
    }
    
    //distort
    for (int d=0; d<2; d++){
        float max_dist = main_circle_dist;// 400;
        float max_push = 20;
        float curve = 0.2;//0.5;
        
        ofVec2f push_center;
        push_center.x = ofGetWidth()/2;
        push_center.y = ofGetHeight()/2 + 40;
        if (d==1)   push_center.y = ofGetHeight()/2 - 40;
        
        
        for (int c=0; c<NUM_COLS; c++){
            for (int i=0;i<lines[c].size(); i++){
                
                ofVec2f center = push_center;
                
                ofVec2f dif_a = center-lines[c][i].a;
                float prc_a = MAX(1.0 - dif_a.length() / max_dist, 0);
                prc_a = powf(prc_a, curve);
                dif_a.normalize();
                lines[c][i].a -= dif_a * prc_a * max_push;
                
                ofVec2f dif_b = center-lines[c][i].b;
                float prc_b = MAX(1.0 - dif_b.length() / max_dist, 0);
                prc_b = powf(prc_b, curve);
                dif_b.normalize();
                lines[c][i].b -= dif_b * prc_b * max_push;
                
            }
        }
    }
    
    //chop off lines in a circle
    vector<Fray> frays;
    for (int c=0; c<NUM_COLS; c++){
        for (int i=lines[c].size()-1; i>=0; i--){
            GLine * line = &lines[c][i];
            
            bool a_in = ofDistSquared(line->a.x, line->a.y, ofGetWidth()/2, ofGetHeight()/2) < main_circle_dist*main_circle_dist;
            bool b_in = ofDistSquared(line->b.x, line->b.y, ofGetWidth()/2, ofGetHeight()/2) < main_circle_dist*main_circle_dist;
            
            if (!a_in && !b_in){
                lines[c].erase(lines[c].begin() + i);
            }
            else if (a_in != b_in){
                //make sure A is on the inside
                if (b_in){
                    line->swap_a_and_b();
                }
                
                //make a fray
                Fray fray;
                fray.pos.set(line->a);
                fray.color = c;
                fray.angle = atan2(line->b.y-line->a.y, line->b.x-line->a.x);
                frays.push_back(fray);
                
                //kill it
                lines[c].erase(lines[c].begin() + i);
                
            }
        }
    }
    
    //push the frays out
    for (int i=0; i<frays.size(); i++){
        curve_shoot(frays[i]);
    }
    
    
    
    if (true){
        ofRectangle safeZone;
        safeZone.set(10, 10, ofGetWidth()-20, ofGetHeight()-20);
        
        for (int c=0; c<NUM_COLS; c++){
            gcode.clear();
            for (int i=0; i<lines[c].size(); i++){
                gcode.line(lines[c][i]);
            }
            gcode.set_outwards_only_bounds(safeZone);
            gcode.sort();
            
            gcode.save("inktober_10_"+ofToString(c)+".nc");
        }
    }
   
}

//--------------------------------------------------------------
void ofApp::curve_shoot(Fray fray){
    ofVec2f start_pos = ofVec2f(fray.pos);
    float angle = fray.angle;
    ofVec2f pos = ofVec2f(fray.pos);
    ofVec2f prev_pos;
    
    int num_steps = ofRandom(10, 130);
    float stepDist = 2;
    
    float curve_range = 0.05;
    float curve = ofRandom(-curve_range,curve_range);
    
    for (int i=0; i<num_steps; i++){
        pos.x += cos(angle) * stepDist;
        pos.y += sin(angle) * stepDist;
        
        angle += curve;
        angle += (1-ofNoise(start_pos.x, start_pos.y, i*0.01) *2) * 0.1;
        
        if (ofDistSquared(pos.x, pos.y, ofGetWidth()/2, ofGetHeight()/2) < main_circle_dist*main_circle_dist){
            return;
        }
        
        if (i!=0){
            GLine line;
            line.set(pos, prev_pos);
            lines[fray.color].push_back(line);
        }
        prev_pos.set(pos);
    }
}

//--------------------------------------------------------------
void ofApp::line_shoot(int c, int r, float grid_size, float start_x, float start_y, float dist, int angle_dir, bool clockwise, ofVec2f pivot, int color){
    ofVec2f pos = ofVec2f(start_x, start_y);
    ofVec2f prev_pos = ofVec2f(pos);
    float start_angle = angle_dir * (PI/2);
    float angle = start_angle;
    int turn_phase = 0;
    
    while (pos.x >= c*grid_size && pos.x <= c*grid_size+grid_size && pos.y >= r*grid_size && pos.y <= r*grid_size+grid_size){
        if (turn_phase == 0 || turn_phase == 2){
            pos.x += cos(angle);
            pos.y += sin(angle);
        }
        
        //where do we pivot?
        if (turn_phase == 0){
            if ( (angle_dir == 0 && pos.x >= pivot.x)
                || (angle_dir == 1 && pos.y >= pivot.y)
                || (angle_dir == 2 && pos.x <= pivot.x)
                || (angle_dir == 3 && pos.y <= pivot.y)
            ){
                turn_phase = 1;
            }
        }
        
        float turn_speed = 0.1;
        if (turn_phase == 1){
            if (clockwise){
                angle += turn_speed;
                
                pos.x = pivot.x + cos(angle-PI/2) * dist;
                pos.y = pivot.y + sin(angle-PI/2) * dist;
                
                if (angle > start_angle + PI * 1.5){
                    angle = start_angle + PI * 1.5;
                    turn_phase = 2;
                    pos.x = pivot.x + cos(angle-PI/2) * dist;
                    pos.y = pivot.y + sin(angle-PI/2) * dist;
                }
            }
            else{
                angle -= turn_speed;
                
                pos.x = pivot.x + cos(angle+PI/2) * dist;
                pos.y = pivot.y + sin(angle+PI/2) * dist;
                
                if (angle < start_angle - PI * 1.5){
                    angle = start_angle - PI * 1.5;
                    turn_phase = 2;
                    pos.x = pivot.x + cos(angle+PI/2) * dist;
                    pos.y = pivot.y + sin(angle+PI/2) * dist;
                }
            }
        }
        
        GLine line;
        line.set(prev_pos, pos);
        bool can_add = true;
        
        if(can_add){
            lines[color].push_back(line);
        }
        
        //gcode.line(prev_pos, pos);
        prev_pos.set(pos);
    }
}

//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw(){
    
    
    ofNoFill();
    
    for (int c=0; c<NUM_COLS; c++){
        ofColor col = ofColor::fromHsb(c*50,255,255);
        ofSetColor(col);
        
        for (int i=0; i<lines[c].size(); i++){
            lines[c][i].draw();
        }
    }
    
    gcode.draw();

}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){
    cout<< ((float)x/(float)ofGetWidth())<< " " <<((float)y/(float)ofGetHeight())<<endl;
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
