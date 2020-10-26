#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofBackground(250);
    
    for (int i=0; i<2; i++){
        gcode[i].setup();
        gcode[i].show_do_not_reverse = true;
    }
    gcode[0].demo_col = ofColor::darkGreen;
    gcode[1].demo_col = ofColor::darkBlue;
    
    tile_size = ofGetWidth() / GRID_W + 1;
    
    grid_offset.x = 0;
    grid_offset.y = (ofGetHeight()-tile_size*GRID_H)/2;
    
    //test vals
//    grid_offset.x = -tile_size*1.5;
//    grid_offset.y = 15;
    
    for (int c=0; c<GRID_W; c++){
        for (int r=0; r<GRID_H; r++){
            grid[c][r] = NONE;
        }
    }
    load();
    
    refresh();
}

//--------------------------------------------------------------
void ofApp::refresh(){
    gcode[0].clear();
    gcode[1].clear();
    
    // 05 micron
//    float line_spacing = 3;//4;
//    int num_lines = 19;//15;
    
    //005 micron
    float line_spacing = 3;//4;
    int num_lines = 19;//15;
    
    float wave_h = 10;
    
    float straight_dist_from_edge = tile_size/2 - (line_spacing*num_lines)/2;
    
    for (int c=0; c<GRID_W; c++){
        for (int r=0; r<GRID_H; r++){
            ofPushMatrix();
            ofTranslate(c*tile_size+grid_offset.x, r*tile_size+grid_offset.y);
            
            int val = grid[c][r];
            
            for (int i=0; i<num_lines; i++){
                
                int col = snake_a[c][r] ? 0 : 1;
                
                int mid_number = floor(num_lines/2.0);
                //05
                //if (i== mid_number){
                
                //005
                if (i== mid_number || i== mid_number-1 || i==mid_number+1){
                    col = (col+1)%2;
                }
            
                if (val == VERT || val == HORZ){
                    
                    gcode[col].begin_shape();
                    for (float y=0; y<=tile_size; y++){
                        float prc = y/tile_size;
                        float angle = prc * TWO_PI;
                        float x = straight_dist_from_edge + i*line_spacing + sin(angle) * wave_h;
                        if (val == VERT){
                            gcode[col].vertex(x,y);
                        }else{
                            gcode[col].vertex(y,x);
                        }
                    }
                    gcode[col].end_shape(false);
                }
            
            
                if (val == TOP_LEFT){
                     ofVec2f start;
                     start.x = straight_dist_from_edge + i*line_spacing;
                     start.y = tile_size;
                     
                     ofVec2f end;
                     end.x = tile_size;
                     end.y = straight_dist_from_edge + i*line_spacing;
                     
                     ofVec2f c1, c2;
                     
                     c1.x = start.x;
                     c1.y =tile_size/2 + c1.x/2;
                     
                     c2.y = end.y;
                     c2.x = tile_size/2 + c2.y/2;
                    
                     //gcode.bezier(start, c1, c2, end);
                    gcode[col].line(start, end);
                 }
                
                if (val == TOP_RIGHT){
                    ofVec2f start;
                    start.x = straight_dist_from_edge + i*line_spacing;
                    start.y = tile_size;
                    
                    ofVec2f end;
                    end.x = 0;
                    end.y = tile_size - straight_dist_from_edge - (i+1)*line_spacing;
                    
                    ofVec2f c1, c2;
                    
                    c1.x = start.x;
                    c1.y = tile_size/2 + c1.x/2;
                    
                    c2.y = end.y;
                    c2.x = tile_size/2 - c2.y/2;
                    
                    //gcode.bezier(start, c1, c2, end);
                    gcode[col].line(start, end);
                }
                
                if (val == BOTTOM_RIGHT){
                    ofVec2f start;
                    start.x = straight_dist_from_edge + i*line_spacing;
                    start.y = 0;
                    
                    ofVec2f end;
                    end.x = 0;
                    end.y = straight_dist_from_edge + i*line_spacing;
                    
                    //gcode.bezier(start, c1, c2, end);
                    gcode[col].line(start, end);
                }
                
                if (val == BOTTOM_LEFT){
                    ofVec2f start;
                    start.x = straight_dist_from_edge + i*line_spacing;
                    start.y = 0;
                    
                    ofVec2f end;
                    end.x = tile_size;
                    end.y = tile_size - straight_dist_from_edge - (i+1)*line_spacing;
                    
                    //gcode.bezier(start, c1, c2, end);
                    gcode[col].line(start, end);
                }
                
                
            }
            
            
            ofPopMatrix();
            
            
        }
    }
    
    for (int i=0; i<2; i++){
        ofRectangle safe;
        safe.set(10,10,ofGetWidth()-20,ofGetHeight()-20);
        gcode[i].set_outwards_only_bounds(safe);
        gcode[i].sort();
        gcode[i].save("inktober_"+ofToString(i)+".nc");
    }
    
}

//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw(){
    
    for (int i=0; i<2; i++){
        gcode[i].draw();
    }
    
//    ofNoFill();
//    ofSetColor(0);
//    for (int c=0; c<GRID_W; c++){
//        for (int r=0; r<GRID_H; r++){
//            ofPushMatrix();
//            ofTranslate(c*tile_size+grid_offset.x, r*tile_size+grid_offset.y);
//            ofDrawRectangle(0,0, tile_size, tile_size);
//
//            ofPopMatrix();
//        }
//    }


}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    if (ofGetMouseX() > grid_offset.x && ofGetMouseX() < grid_offset.x+GRID_W*tile_size && ofGetMouseY() > grid_offset.y && ofGetMouseY() < grid_offset.y+GRID_H*tile_size){
        //cout<<"good"<<endl;
        int c = (ofGetMouseX()-grid_offset.x) / tile_size;
        int r = (ofGetMouseY()-grid_offset.y) / tile_size;
        
        int op_c = GRID_W-1-c;
        int op_r = GRID_H-1-r;
        
        if (key == ' '){
            snake_a[c][r] = !snake_a[c][r];
        }
        if (key == 'w'){
            grid[c][r] = VERT;
            grid[op_c][op_r] = VERT;
        }
        if (key == 's'){
            grid[c][r] = HORZ;
            grid[op_c][op_r] = HORZ;
        }
        if (key == 'q'){
            grid[c][r] = TOP_LEFT;
            grid[op_c][op_r] = (TOP_LEFT+2)%4;
        }
        if (key == 'e'){
            grid[c][r] = TOP_RIGHT;
            grid[op_c][op_r] = (TOP_RIGHT+2)%4;
        }
        if (key == 'd'){
            grid[c][r] = BOTTOM_RIGHT;
            grid[op_c][op_r] = (BOTTOM_RIGHT+2)%4;
        }
        if (key == 'a'){
            grid[c][r] = BOTTOM_LEFT;
            grid[op_c][op_r] = (BOTTOM_LEFT+2)%4;
        }
        if (key == 'x'){
            grid[c][r] = NONE;
            grid[op_c][op_r] = NONE;
        }
    }
    
    
    if (key == 'p'){
        for (int c=0; c<GRID_W; c++){
            for (int r=0; r<GRID_H; r++){
                cout<<"snake_a["<<ofToString(c)<<"]["<<ofToString(r)<<"] = "<<ofToString(snake_a[c][r])<<";"<<endl;
            }
        }
        for (int c=0; c<GRID_W; c++){
            for (int r=0; r<GRID_H; r++){
                cout<<"grid["<<ofToString(c)<<"]["<<ofToString(r)<<"] = "<<ofToString(grid[c][r])<<";"<<endl;
            }
        }
    }
    
    refresh();
}

void ofApp::load(){
    snake_a[0][0] = 1;
    snake_a[0][1] = 1;
    snake_a[0][2] = 1;
    snake_a[0][3] = 1;
    snake_a[0][4] = 1;
    snake_a[0][5] = 1;
    snake_a[0][6] = 1;
    snake_a[0][7] = 1;
    snake_a[0][8] = 1;
    snake_a[0][9] = 1;
    snake_a[0][10] = 1;
    snake_a[0][11] = 1;
    snake_a[0][12] = 1;
    snake_a[0][13] = 1;
    snake_a[1][0] = 1;
    snake_a[1][1] = 1;
    snake_a[1][2] = 1;
    snake_a[1][3] = 1;
    snake_a[1][4] = 1;
    snake_a[1][5] = 1;
    snake_a[1][6] = 1;
    snake_a[1][7] = 1;
    snake_a[1][8] = 1;
    snake_a[1][9] = 1;
    snake_a[1][10] = 1;
    snake_a[1][11] = 1;
    snake_a[1][12] = 1;
    snake_a[1][13] = 1;
    snake_a[2][0] = 1;
    snake_a[2][1] = 1;
    snake_a[2][2] = 1;
    snake_a[2][3] = 1;
    snake_a[2][4] = 1;
    snake_a[2][5] = 1;
    snake_a[2][6] = 1;
    snake_a[2][7] = 1;
    snake_a[2][8] = 1;
    snake_a[2][9] = 1;
    snake_a[2][10] = 1;
    snake_a[2][11] = 1;
    snake_a[2][12] = 0;
    snake_a[2][13] = 0;
    snake_a[3][0] = 1;
    snake_a[3][1] = 1;
    snake_a[3][2] = 1;
    snake_a[3][3] = 1;
    snake_a[3][4] = 1;
    snake_a[3][5] = 1;
    snake_a[3][6] = 1;
    snake_a[3][7] = 1;
    snake_a[3][8] = 1;
    snake_a[3][9] = 1;
    snake_a[3][10] = 1;
    snake_a[3][11] = 1;
    snake_a[3][12] = 0;
    snake_a[3][13] = 0;
    snake_a[4][0] = 1;
    snake_a[4][1] = 1;
    snake_a[4][2] = 0;
    snake_a[4][3] = 0;
    snake_a[4][4] = 0;
    snake_a[4][5] = 1;
    snake_a[4][6] = 1;
    snake_a[4][7] = 1;
    snake_a[4][8] = 1;
    snake_a[4][9] = 1;
    snake_a[4][10] = 1;
    snake_a[4][11] = 0;
    snake_a[4][12] = 0;
    snake_a[4][13] = 0;
    snake_a[5][0] = 1;
    snake_a[5][1] = 1;
    snake_a[5][2] = 0;
    snake_a[5][3] = 0;
    snake_a[5][4] = 0;
    snake_a[5][5] = 1;
    snake_a[5][6] = 1;
    snake_a[5][7] = 1;
    snake_a[5][8] = 1;
    snake_a[5][9] = 1;
    snake_a[5][10] = 1;
    snake_a[5][11] = 0;
    snake_a[5][12] = 0;
    snake_a[5][13] = 0;
    snake_a[6][0] = 1;
    snake_a[6][1] = 1;
    snake_a[6][2] = 0;
    snake_a[6][3] = 0;
    snake_a[6][4] = 0;
    snake_a[6][5] = 1;
    snake_a[6][6] = 1;
    snake_a[6][7] = 0;
    snake_a[6][8] = 0;
    snake_a[6][9] = 1;
    snake_a[6][10] = 1;
    snake_a[6][11] = 1;
    snake_a[6][12] = 0;
    snake_a[6][13] = 0;
    snake_a[7][0] = 1;
    snake_a[7][1] = 1;
    snake_a[7][2] = 0;
    snake_a[7][3] = 0;
    snake_a[7][4] = 0;
    snake_a[7][5] = 1;
    snake_a[7][6] = 1;
    snake_a[7][7] = 0;
    snake_a[7][8] = 0;
    snake_a[7][9] = 1;
    snake_a[7][10] = 1;
    snake_a[7][11] = 1;
    snake_a[7][12] = 0;
    snake_a[7][13] = 0;
    snake_a[8][0] = 1;
    snake_a[8][1] = 1;
    snake_a[8][2] = 1;
    snake_a[8][3] = 0;
    snake_a[8][4] = 0;
    snake_a[8][5] = 0;
    snake_a[8][6] = 0;
    snake_a[8][7] = 0;
    snake_a[8][8] = 0;
    snake_a[8][9] = 1;
    snake_a[8][10] = 1;
    snake_a[8][11] = 1;
    snake_a[8][12] = 0;
    snake_a[8][13] = 0;
    snake_a[9][0] = 1;
    snake_a[9][1] = 1;
    snake_a[9][2] = 1;
    snake_a[9][3] = 0;
    snake_a[9][4] = 0;
    snake_a[9][5] = 0;
    snake_a[9][6] = 0;
    snake_a[9][7] = 0;
    snake_a[9][8] = 0;
    snake_a[9][9] = 1;
    snake_a[9][10] = 1;
    snake_a[9][11] = 1;
    snake_a[9][12] = 0;
    snake_a[9][13] = 0;
    snake_a[10][0] = 1;
    snake_a[10][1] = 1;
    snake_a[10][2] = 0;
    snake_a[10][3] = 0;
    snake_a[10][4] = 0;
    snake_a[10][5] = 0;
    snake_a[10][6] = 0;
    snake_a[10][7] = 0;
    snake_a[10][8] = 0;
    snake_a[10][9] = 0;
    snake_a[10][10] = 0;
    snake_a[10][11] = 0;
    snake_a[10][12] = 0;
    snake_a[10][13] = 0;
    snake_a[11][0] = 1;
    snake_a[11][1] = 1;
    snake_a[11][2] = 0;
    snake_a[11][3] = 0;
    snake_a[11][4] = 0;
    snake_a[11][5] = 0;
    snake_a[11][6] = 0;
    snake_a[11][7] = 0;
    snake_a[11][8] = 0;
    snake_a[11][9] = 0;
    snake_a[11][10] = 0;
    snake_a[11][11] = 0;
    snake_a[11][12] = 0;
    snake_a[11][13] = 0;
    snake_a[12][0] = 0;
    snake_a[12][1] = 0;
    snake_a[12][2] = 0;
    snake_a[12][3] = 0;
    snake_a[12][4] = 0;
    snake_a[12][5] = 0;
    snake_a[12][6] = 0;
    snake_a[12][7] = 0;
    snake_a[12][8] = 0;
    snake_a[12][9] = 0;
    snake_a[12][10] = 0;
    snake_a[12][11] = 0;
    snake_a[12][12] = 0;
    snake_a[12][13] = 0;
    snake_a[13][0] = 0;
    snake_a[13][1] = 0;
    snake_a[13][2] = 0;
    snake_a[13][3] = 0;
    snake_a[13][4] = 0;
    snake_a[13][5] = 0;
    snake_a[13][6] = 0;
    snake_a[13][7] = 0;
    snake_a[13][8] = 0;
    snake_a[13][9] = 0;
    snake_a[13][10] = 0;
    snake_a[13][11] = 0;
    snake_a[13][12] = 0;
    snake_a[13][13] = 0;
    grid[0][0] = -1;
    grid[0][1] = -1;
    grid[0][2] = -1;
    grid[0][3] = -1;
    grid[0][4] = 1;
    grid[0][5] = 4;
    grid[0][6] = 3;
    grid[0][7] = 0;
    grid[0][8] = 4;
    grid[0][9] = 2;
    grid[0][10] = -1;
    grid[0][11] = -1;
    grid[0][12] = -1;
    grid[0][13] = -1;
    grid[1][0] = -1;
    grid[1][1] = -1;
    grid[1][2] = -1;
    grid[1][3] = -1;
    grid[1][4] = -1;
    grid[1][5] = -1;
    grid[1][6] = 5;
    grid[1][7] = 5;
    grid[1][8] = -1;
    grid[1][9] = -1;
    grid[1][10] = -1;
    grid[1][11] = -1;
    grid[1][12] = -1;
    grid[1][13] = -1;
    grid[2][0] = 0;
    grid[2][1] = 4;
    grid[2][2] = 4;
    grid[2][3] = 4;
    grid[2][4] = 4;
    grid[2][5] = 4;
    grid[2][6] = 2;
    grid[2][7] = 5;
    grid[2][8] = 0;
    grid[2][9] = 4;
    grid[2][10] = 4;
    grid[2][11] = 3;
    grid[2][12] = 0;
    grid[2][13] = 3;
    grid[3][0] = 5;
    grid[3][1] = 0;
    grid[3][2] = 4;
    grid[3][3] = 4;
    grid[3][4] = 4;
    grid[3][5] = 4;
    grid[3][6] = 3;
    grid[3][7] = 5;
    grid[3][8] = 1;
    grid[3][9] = 3;
    grid[3][10] = 0;
    grid[3][11] = 2;
    grid[3][12] = 5;
    grid[3][13] = 5;
    grid[4][0] = 5;
    grid[4][1] = 5;
    grid[4][2] = 0;
    grid[4][3] = 4;
    grid[4][4] = 3;
    grid[4][5] = 0;
    grid[4][6] = 2;
    grid[4][7] = 1;
    grid[4][8] = 4;
    grid[4][9] = 2;
    grid[4][10] = 5;
    grid[4][11] = 0;
    grid[4][12] = 2;
    grid[4][13] = 5;
    grid[5][0] = 5;
    grid[5][1] = 5;
    grid[5][2] = 5;
    grid[5][3] = 0;
    grid[5][4] = 2;
    grid[5][5] = 5;
    grid[5][6] = 0;
    grid[5][7] = 4;
    grid[5][8] = 4;
    grid[5][9] = 3;
    grid[5][10] = 5;
    grid[5][11] = 1;
    grid[5][12] = 3;
    grid[5][13] = 5;
    grid[6][0] = 5;
    grid[6][1] = 5;
    grid[6][2] = 5;
    grid[6][3] = 1;
    grid[6][4] = 3;
    grid[6][5] = 5;
    grid[6][6] = 5;
    grid[6][7] = 0;
    grid[6][8] = 3;
    grid[6][9] = 5;
    grid[6][10] = 1;
    grid[6][11] = 3;
    grid[6][12] = 5;
    grid[6][13] = 5;
    grid[7][0] = 5;
    grid[7][1] = 5;
    grid[7][2] = 1;
    grid[7][3] = 3;
    grid[7][4] = 5;
    grid[7][5] = 1;
    grid[7][6] = 2;
    grid[7][7] = 5;
    grid[7][8] = 5;
    grid[7][9] = 1;
    grid[7][10] = 3;
    grid[7][11] = 5;
    grid[7][12] = 5;
    grid[7][13] = 5;
    grid[8][0] = 5;
    grid[8][1] = 1;
    grid[8][2] = 3;
    grid[8][3] = 5;
    grid[8][4] = 1;
    grid[8][5] = 4;
    grid[8][6] = 4;
    grid[8][7] = 2;
    grid[8][8] = 5;
    grid[8][9] = 0;
    grid[8][10] = 2;
    grid[8][11] = 5;
    grid[8][12] = 5;
    grid[8][13] = 5;
    grid[9][0] = 5;
    grid[9][1] = 0;
    grid[9][2] = 2;
    grid[9][3] = 5;
    grid[9][4] = 0;
    grid[9][5] = 4;
    grid[9][6] = 3;
    grid[9][7] = 0;
    grid[9][8] = 2;
    grid[9][9] = 1;
    grid[9][10] = 4;
    grid[9][11] = 2;
    grid[9][12] = 5;
    grid[9][13] = 5;
    grid[10][0] = 5;
    grid[10][1] = 5;
    grid[10][2] = 0;
    grid[10][3] = 2;
    grid[10][4] = 1;
    grid[10][5] = 3;
    grid[10][6] = 5;
    grid[10][7] = 1;
    grid[10][8] = 4;
    grid[10][9] = 4;
    grid[10][10] = 4;
    grid[10][11] = 4;
    grid[10][12] = 2;
    grid[10][13] = 5;
    grid[11][0] = 1;
    grid[11][1] = 2;
    grid[11][2] = 1;
    grid[11][3] = 4;
    grid[11][4] = 4;
    grid[11][5] = 2;
    grid[11][6] = 5;
    grid[11][7] = 0;
    grid[11][8] = 4;
    grid[11][9] = 4;
    grid[11][10] = 4;
    grid[11][11] = 4;
    grid[11][12] = 4;
    grid[11][13] = 2;
    grid[12][0] = -1;
    grid[12][1] = -1;
    grid[12][2] = -1;
    grid[12][3] = -1;
    grid[12][4] = -1;
    grid[12][5] = -1;
    grid[12][6] = 5;
    grid[12][7] = 5;
    grid[12][8] = -1;
    grid[12][9] = -1;
    grid[12][10] = -1;
    grid[12][11] = -1;
    grid[12][12] = -1;
    grid[12][13] = -1;
    grid[13][0] = -1;
    grid[13][1] = -1;
    grid[13][2] = -1;
    grid[13][3] = -1;
    grid[13][4] = 0;
    grid[13][5] = 4;
    grid[13][6] = 2;
    grid[13][7] = 1;
    grid[13][8] = 4;
    grid[13][9] = 3;
    grid[13][10] = -1;
    grid[13][11] = -1;
    grid[13][12] = -1;
    grid[13][13] = -1;



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
