#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){

    ofBackground(250);
    gcode.setup(100);
    gcode.show_path_with_color = false;
    gcode.show_transit_lines = false;
    
    //setup the circle grid
    for (int d=0; d<NUM_OUTWARD; d++){
        for (int a=0; a<NUM_AROUND; a++){
            float angle = a * (TWO_PI / (float)NUM_AROUND);
            float dist_prc = (float)d / (float)NUM_OUTWARD;
            dist_prc = powf(dist_prc, 1.25);
            float dist = 30 + dist_prc * 300;
            nodes[d][a].setup(angle, dist);
        }
    }
    
    //make connections
    for (int d=0; d<NUM_OUTWARD; d++){
        for (int a=0; a<NUM_AROUND; a++){
            nodes[d][a].neighbors[CLOCK_DIR] = &nodes[d][(a+1)%NUM_AROUND];
            nodes[d][a].neighbors[COUNTER_CLOCK_DIR] = &nodes[d][(a+NUM_AROUND-1)%NUM_AROUND];
            
            if (d<NUM_OUTWARD-1){
                nodes[d][a].neighbors[OUT_DIR] = &nodes[d+1][a];
            }
            if (d>0){
                nodes[d][a].neighbors[IN_DIR] = &nodes[d-1][a];
            }
        }
    }
    
    //block in
    int num_blocks = 8;
    int spacing = NUM_AROUND/num_blocks;
    int block_size = 4;
    for (int i=0; i<num_blocks; i++){
        int a = 7 + (i%2) * 5;
        block(a, i*spacing + spacing - 2, block_size);
    }
    
    for (int i=0; i<num_blocks; i++){
        int a = 10 + 7 + (i%2) * 5;
        block(a, i*spacing + spacing - 2, block_size);
    }
    
    
    
    //maze 'em up
    while (true){
        //get a list of unused nodes
        vector<Node *> unused;
        for (int d=0; d<NUM_OUTWARD; d++){
            for (int a=0; a<NUM_AROUND; a++){
                if (!nodes[d][a].used){
                    unused.push_back(&nodes[d][a]);
                }
            }
        }
        
        //are we done?
        if (unused.size() == 0){
            break;
        }
        else{
            //select one at random
            Node * selection = unused[ (int)ofRandom(unused.size()) ];
            explore_node(selection);
        }
    }
    
    //add and print
    for (int d=0; d<NUM_OUTWARD; d++){
        for (int a=0; a<NUM_AROUND; a++){
            nodes[d][a].make_gcode(&gcode);
        }
    }
    
    gcode.sort();
    gcode.save("inktober_04.nc");
}

//--------------------------------------------------------------
void ofApp::block(int block_start_d, int block_start_a, int size){
    //top
    for (int a=block_start_a; a<block_start_a+size; a++ ){
        nodes[block_start_d][a%NUM_AROUND].connections[CLOCK_DIR] = true;
    }
    //bottom
    for (int a=block_start_a; a<block_start_a+size; a++ ){
        nodes[block_start_d+size][a%NUM_AROUND].connections[CLOCK_DIR] = true;
    }
    
    //left
    for (int d=block_start_d; d<block_start_d+size; d++ ){
        nodes[d][block_start_a].connections[OUT_DIR] = true;
    }
    //right
    for (int d=block_start_d; d<block_start_d+size; d++ ){
        nodes[d][(block_start_a+size)%NUM_AROUND].connections[OUT_DIR] = true;
    }
    
    //just turn off eveyrthing in there
    for (int a=block_start_a; a<=block_start_a+size; a++ ){
        for (int d=block_start_d; d<=block_start_d+size; d++ ){
            nodes[d][a%NUM_AROUND].used = true;
            if (d>block_start_d && d<block_start_d+size && a>block_start_a && a<block_start_a+size){
                nodes[d][a%NUM_AROUND].draw_circle = true;
            }
        }
    }
         
}

//--------------------------------------------------------------
void ofApp::explore_node(Node * node){
    
    node->used = true;
    
    vector<Ticket> tics;
    
    float horz_chance = 0.75;
    float vert_chance = 0.25;
    
    for (int dir=0; dir<4; dir++){
        
        if (node->neighbors[dir] != NULL){
            if (node->neighbors[dir]->used == false){
                Ticket tic;
                tic.dir = dir;
                
                if (dir == CLOCK_DIR || dir == COUNTER_CLOCK_DIR){
                    tic.chance = 1;
                }else{
                    tic.chance = .5;
                }
                
                tics.push_back(tic);
            }
        }
    }
    
    //if there are no valid choices, bounce
    if (tics.size() == 0){
        return;
    }
    
    //otherwise try to do them
    float total_chance = 0;
    for (int i=0; i<tics.size(); i++){
        total_chance += tics[i].chance;
    }
    
    float roll = ofRandom(total_chance);
    for (int i=0; i<tics.size(); i++){
        roll -= tics[i].chance;
        if (roll < 0){
            int dir = tics[i].dir;
            
            node->connections[dir] = true;
            
            explore_node(node->neighbors[dir]);
            return;
        }
    }
    
    
    
}


//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw(){
    gcode.draw();
    
//    ofSetColor(0);
//    for (int d=0; d<NUM_OUTWARD; d++){
//        for (int a=0; a<NUM_AROUND; a++){
//            nodes[d][a].draw();
//        }
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
