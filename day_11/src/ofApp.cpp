#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofBackground(250);
    
    gcode.setup(100);
    gcode.show_transit_lines = false;
    gcode.show_path_with_color = false;
    
    line_spacing = 5;
    line_step_dist= 1;
    
    min_circle_size = 20;
    
    grow_step = 2;
    grow_padding = 10;
    
    float prc_to_remove = 0.0;  //eh, not sure this looks good
    
    root = new Circle(ofGetWidth() * 0.5, ofGetHeight() * 0.5, ofGetWidth() * 0.4);
    
    //fill it
    int num_circles = 60;
    int num_options = 7;
    for (int k=0; k<num_circles; k++){
        
        //generate some options. This leakes a lot of memory but I don't care for a quick plotter drawing
        vector<Circle *> options;
        for (int i=0; i<num_options; i++){
            Circle * new_c = generate_circle();
            if (new_c != NULL){
                options.push_back(new_c);
            }
        }
        
        if (options.size() > 0){
            //find the biggest one
            Circle * best = options[0];
            for (int i=0; i<options.size(); i++){
                if (options[i]->size > best->size){
                    best = options[i];
                }
            }
            
            Circle * parent = root->find_deepest_container(best->x, best->y);
            parent->children.push_back(best);
        }
    }
    
    
    
    
    //add the lines
    for (int i=0; i<root->children.size(); i++){
        circle_fill(root->children[i], 0);
    }
    //circle_fill(root, 0);
    
    //trim some
    int num_to_kill = lines.size() * prc_to_remove;
    for (int i=0; i<num_to_kill; i++){
        int rand = ofRandom(lines.size());
        lines.erase(lines.begin() + rand);
    }
    
    for (int i=0; i<lines.size(); i++){
        gcode.line(lines[i]);
    }
    
    gcode.sort();
    gcode.save("inktober_11.nc");
    

}

//--------------------------------------------------------------
Circle * ofApp::generate_circle(){
    
    ofVec2f origin;
    origin.x = ofRandom(ofGetWidth());
    origin.y = ofRandom(ofGetHeight());
    //if it is outside of root, reject it
    while (ofDistSquared(origin.x, origin.y, root->x, root->y) > powf(root->size,2)){
        origin.x = ofRandom(ofGetWidth());
        origin.y = ofRandom(ofGetHeight());
    }
    
    Circle * circ = new Circle( origin.x , origin.y, min_circle_size );
    
    //figure out what circle we're in
    Circle * parent = root->find_deepest_container(circ->x, circ->y);
    
    //grow it
    bool keep_going = true;
    while (keep_going){
        circ->size += grow_step;
        
        int num_test_pnts = 50;
        float angle_step = (TWO_PI) / (float)num_test_pnts;
        for (int i=0; i<num_test_pnts; i++){
            float angle = i*angle_step;
            float test_x = circ->x + cos(angle) * circ->size;
            float test_y = circ->y + sin(angle) * circ->size;
            
            //make sure it is inside the parent
            if (!parent->inside(test_x, test_y)){
                keep_going = false;
            }
            
            //make sure it is not inside and children of the parent
            for (int c=0; c<parent->children.size(); c++){
                if (parent->children[c]->inside(test_x, test_y)){
                    keep_going = false;
                }
            }
            
        }
        
    }
    circ->size -= grow_padding;
    
    
    //if it is below the min size, reject it
    if (circ->size < min_circle_size){
        return NULL;
    }
    
    //otherwise return it
    return circ;
    
    
    
    
}


//--------------------------------------------------------------
void ofApp::circle_fill(Circle * circ, float angle){
    float tan_angle = angle + PI/2;
    ofVec2f start_pos, end_pos;
    start_pos.x = circ->x + cos(tan_angle) * circ->size;
    start_pos.y = circ->y + sin(tan_angle) * circ->size;
    end_pos.x = circ->x - cos(tan_angle) * circ->size;
    end_pos.y = circ->y - sin(tan_angle) * circ->size;
    
    int num_lines = (circ->size*2) / line_spacing;
    for (int i=1; i<num_lines;i++){
        float prc = (float)i / (float)num_lines;
        ofVec2f origin = (1.0-prc) * start_pos + prc * end_pos;
        
        //shoot lines
        line_shoot(origin, angle, circ);
        
    }
    
    for (int i=0; i<circ->children.size(); i++){
        circle_fill(circ->children[i], angle + PI/4);
    }
}

//--------------------------------------------------------------
void ofApp::line_shoot(ofVec2f origin, float angle, Circle * circ){
    //left
    ofVec2f pos_a = ofVec2f(origin);
    
    //step back
    while( ofDistSquared(pos_a.x, pos_a.y, circ->x, circ->y) < powf(circ->size,2)){
        pos_a.x -= cos(angle) * line_step_dist;
        pos_a.y -= sin(angle) * line_step_dist;
    }
    pos_a.x += cos(angle) * line_step_dist;
    pos_a.y += sin(angle) * line_step_dist;
    
    ofVec2f cur_pos = ofVec2f(pos_a);
    bool is_on = true;
    while( ofDistSquared(cur_pos.x, cur_pos.y, circ->x, circ->y) < powf(circ->size,2)){
        if (is_on){
            if(inside_other(cur_pos, circ->children)){
                is_on = false;
                GLine line;
                line.set(pos_a, cur_pos);
                lines.push_back(line);
                //gcode.line(pos_a, cur_pos);
            }
        }
        else{
            if(!inside_other(cur_pos, circ->children)){
                is_on = true;
                pos_a.set(cur_pos);
            }
        }
         
        
        cur_pos.x += cos(angle) * line_step_dist;
        cur_pos.y += sin(angle);
    }
    if (is_on){
        GLine line;
        line.set(pos_a, cur_pos);
        lines.push_back(line);
        //gcode.line(pos_a, cur_pos);
    }
}

//--------------------------------------------------------------
bool ofApp::inside_other(ofVec2f pos, vector<Circle *> others){
    for (int i=0; i<others.size(); i++){
        if (ofDistSquared(pos.x,pos.y, others[i]->x, others[i]->y) < powf(others[i]->size,2)){
            return true;
        }
        
    }
    return false;
}

//--------------------------------------------------------------
void ofApp::update(){
    
    

}

//--------------------------------------------------------------
void ofApp::draw(){
    
    gcode.draw();
    
    ofNoFill();
    ofSetColor(100, 0, 0);
    //root->draw();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){
//    Circle * demo = root->find_deepest_container(x, y);
//    cout<<"demo center "<<demo->x<<" , "<<demo->y<<endl;
    
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
