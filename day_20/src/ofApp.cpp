#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofBackground(250);
    gcode.setup(100);
    gcode.show_transit_lines = false;
    
    show_lines = true;
    show_fill = false;
    
    float trim_padding = 100;
    trim_rect.set(trim_padding,trim_padding, ofGetWidth()-trim_padding*2, ofGetHeight()-trim_padding*2);
    
    
    int num_steps = 6;
    
    float line_spacing = 0.75;
    
    
    tile_size = 10;
    float noise_zoom = 0.04;
    float noise_offset = 50;//99;    //randomize me
    for (int x=0; x<NUM_COLS; x++){
        for (int y=0; y<NUM_ROWS; y++){
            grid[x][y] = ofNoise(x*noise_zoom, y*noise_zoom, noise_offset) * num_steps;
        }
    }
    
    //go through and draw lines
    ofPushMatrix();
    
    ofTranslate(NUM_COLS*tile_size*0.15, NUM_ROWS*tile_size*0.15 );
    ofRotateRad(PI/4);
    
    ofTranslate(-NUM_COLS*tile_size*0.5, -NUM_ROWS*tile_size*0.5 );
    for (int x=1; x<NUM_COLS-1; x++){
        for (int y=1; y<NUM_ROWS-1; y++){
            
            int grid_val = grid[x][y];
            
            //check all four directions
            //left
            if (grid[x-1][y] < grid[x][y]){
                gcode.thick_line(x*tile_size, y*tile_size, x*tile_size, (y+1)*tile_size, line_spacing, grid_val);
            }
            //right
            if (grid[x+1][y] < grid[x][y]){
                gcode.thick_line((x+1)*tile_size, y*tile_size, (x+1)*tile_size, (y+1)*tile_size, line_spacing, grid_val*1.5);
            }
            
            //up
            if (grid[x][y-1] < grid[x][y]){
                gcode.thick_line(x*tile_size, y*tile_size, (x+1)*tile_size, y*tile_size, line_spacing, grid_val);
            }
            
            //down
            if (grid[x][y+1] < grid[x][y]){
                gcode.thick_line(x*tile_size, (y+1)*tile_size, (x+1)*tile_size, (y+1)*tile_size, line_spacing, grid_val*2);
            }
            
        }
    }
    ofPopMatrix();
    
    gcode.clip_outside(trim_rect);
    
    //draw the frame
    float frame_layers = 4;
    gcode.thick_line(trim_rect.x, trim_rect.y, trim_rect.x+trim_rect.width, trim_rect.y, line_spacing, frame_layers);
    gcode.thick_line(trim_rect.x, trim_rect.y, trim_rect.x, trim_rect.y+trim_rect.height, line_spacing, frame_layers);
    gcode.thick_line(trim_rect.x, trim_rect.y+trim_rect.height, trim_rect.x+trim_rect.width, trim_rect.y+trim_rect.height, line_spacing, frame_layers);
    gcode.thick_line(trim_rect.x+trim_rect.width, trim_rect.y, trim_rect.x+trim_rect.width, trim_rect.y+trim_rect.height, line_spacing, frame_layers);
    
    float frame_padding = 25;
    float frame_left_x = trim_rect.x - frame_padding;
    float frame_top_y = trim_rect.y - frame_padding;
    float frame_right_x = trim_rect.x + trim_rect.width + frame_padding;
    float frame_bot_y = trim_rect.y + trim_rect.height + frame_padding;
    
    //outter frame
    gcode.thick_line(frame_left_x, frame_top_y, frame_left_x, frame_bot_y, line_spacing, frame_layers);
    gcode.thick_line(frame_right_x, frame_top_y, frame_right_x, frame_bot_y, line_spacing, frame_layers);
    gcode.thick_line(frame_left_x, frame_top_y, frame_right_x, frame_top_y, line_spacing, frame_layers);
    gcode.thick_line(frame_left_x, frame_bot_y, frame_right_x, frame_bot_y, line_spacing, frame_layers);
    
    //corners
    gcode.thick_line(frame_left_x, frame_top_y, trim_rect.x, trim_rect.y, line_spacing, 3);
    gcode.thick_line(frame_right_x, frame_top_y, trim_rect.x+trim_rect.width, trim_rect.y, line_spacing, 2);
    gcode.thick_line(frame_right_x, frame_bot_y, trim_rect.x+trim_rect.width, trim_rect.y+trim_rect.height, line_spacing, 3);
    gcode.thick_line(frame_left_x, frame_bot_y, trim_rect.x, trim_rect.y+trim_rect.height, line_spacing, 4);
    
    int circle_spacing = 25;
    float circle_size = 5;
    //horizontal
    int num_circles = (trim_rect.width)/circle_spacing;
    for (int i=0; i<=num_circles; i++){
        float prc = (float)i / (float)num_circles;
        float x = trim_rect.x + circle_size + prc * (trim_rect.width-circle_size*2);
        float y = (trim_rect.y + frame_top_y)/2;
        gcode.circle(x, y, circle_size);
        
        y = (trim_rect.y+trim_rect.height + frame_bot_y)/2;
        gcode.circle(x, y, circle_size);
    }
    
    //vertical
    num_circles = (trim_rect.height)/circle_spacing;
    for (int i=0; i<=num_circles; i++){
        float prc = (float)i / (float)num_circles;
        float x = (trim_rect.x + frame_left_x)/2;
        float y = trim_rect.y + circle_size + prc * (trim_rect.height-circle_size*2);
        
        gcode.circle(x, y, circle_size);
        
        x = (trim_rect.x+trim_rect.width + frame_right_x)/2;
        gcode.circle(x, y, circle_size);
    }
}

//--------------------------------------------------------------
void ofApp::update(){

    if (ofGetFrameNum() == 2){
        gcode.sort();
        gcode.save("inktober_20.nc");
    }
}

//--------------------------------------------------------------
void ofApp::draw(){
    if (show_fill){
        ofFill();
        for (int x=0; x<NUM_COLS; x++){
            for (int y=0; y<NUM_ROWS; y++){
                ofSetColor( ofMap(grid[x][y],0,10, 100,160) );
                ofDrawRectangle(x*tile_size, y*tile_size, tile_size, tile_size);

            }
        }
    }
    
    if (show_lines){
        gcode. draw();
    }
    
    
//    ofSetColor(255,0,0);
//    ofNoFill();
//    ofDrawRectangle(trim_rect);

}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    if (key == '1') show_lines = !show_lines;
    if (key == '2') show_fill = !show_fill;
    
    if (key == 's'){
        gcode.sort();
        gcode.save("inktober_20.nc");
    }
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
    cout<<"mouse  "<<x<<" , "<<y<<endl;
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
