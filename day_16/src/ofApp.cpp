#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofBackground(250);
    
    gcode.setup(100);
    gcode.show_transit_lines = false;
    //gcode.show_path_with_color = true;
    
    reset();
}

//--------------------------------------------------------------
void ofApp::make_background(ofVec2f maze_offset){
    
    //set the bounds
    vector<ofVec2f> maze_bounds;
    maze_bounds.push_back(ofVec2f(maze_offset.x, maze_offset.y));
    maze_bounds.push_back(ofVec2f(ofGetWidth()-maze_offset.x, maze_offset.y));
    maze_bounds.push_back(ofVec2f(ofGetWidth()-maze_offset.x, ofGetHeight()-maze_offset.y));
    maze_bounds.push_back(ofVec2f(maze_offset.x, ofGetHeight()-maze_offset.y));
    
    //and the start and end point circles
    ofRectangle start_box;
    start_box.x = 0;
    start_box.y = 0;
    start_box.width = maze_offset.x + tile_size;
    start_box.height = maze_offset.y + tile_size;
    
    ofRectangle end_box;
    end_box.x = ofGetWidth()-maze_offset.x-tile_size;
    end_box.y = ofGetHeight()-maze_offset.y-tile_size;
    end_box.width = ofGetWidth()-end_box.x;
    end_box.height = ofGetHeight()-end_box.y;
    
    
    float padding = 50;
    ofVec2f top_left, top_right, bot_left, bot_right;
    top_left.x = maze_offset.x - padding;
    top_left.y = maze_offset.y - padding;
    top_right.x = ofGetWidth()-maze_offset.x + padding;
    top_right.y = maze_offset.y - padding;
    bot_left.x = maze_offset.x - padding;
    bot_left.y = ofGetHeight()-maze_offset.y + padding;
    bot_right.x = ofGetWidth()-maze_offset.x + padding;
    bot_right.y = ofGetHeight()-maze_offset.y + padding;
    
    int num_lines = (bot_right.x - top_left.x) / 10;
    for (int i=0; i<=num_lines; i++){
        float prc = (float)i / (float)num_lines;
        
        GLine line1;
        line1.a = (1.0-prc) * top_left + prc * top_right;
        line1.b = (1.0-prc) * top_right + prc * bot_right;
        gcode.line(line1);
        
        GLine line2;
        line2.a = (1.0-prc) * top_right + prc * top_left;
        line2.b = (1.0-prc) * top_left + prc * bot_left;
        gcode.line(line2);
        
        GLine line3;
        line3.a = (1.0-prc) * bot_right + prc * bot_left;
        line3.b = (1.0-prc) * top_right + prc * bot_right;
        gcode.line(line3);
        
        GLine line4;
        line4.a = (1.0-prc) * bot_left + prc * bot_right;
        line4.b = (1.0-prc) * top_left + prc * bot_left;
        gcode.line(line4);
    }
    
    //clip
    gcode.clip_inside(maze_bounds);
    gcode.clip_inside(start_box);
    gcode.clip_inside(end_box);
    
    cout<<"top: "<<top_left<<endl;
    cout<<"bottom "<<bot_right<<endl;
    
}

//--------------------------------------------------------------
void ofApp::reset(){
    gcode.clear();
    
    //ofSeedRandom(0);
    tile_size = 30;//50;
    
    ofVec2f maze_offset;
    maze_offset.x = (ofGetWidth()-tile_size*NUM_COLS)/2;
    maze_offset.y = (ofGetHeight()-tile_size*NUM_ROWS)/2;
    
    make_background(maze_offset);
    
    
    //define some walls
    for (int c=0; c<NUM_COLS+1; c++){
        for (int r=0; r<NUM_ROWS+1; r++){
            //vertical
            if (r<NUM_ROWS){
                v_walls[c][r].pos.a.x = maze_offset.x + c*tile_size;
                v_walls[c][r].pos.a.y = maze_offset.y + r*tile_size;
                v_walls[c][r].pos.b.x = maze_offset.x + c*tile_size;
                v_walls[c][r].pos.b.y = maze_offset.y + (r+1)*tile_size;
            }
            
            
            //horizontal
            if (c<NUM_COLS){
                h_walls[c][r].pos.a.x = maze_offset.x + c*tile_size;
                h_walls[c][r].pos.a.y = maze_offset.y + r*tile_size;
                h_walls[c][r].pos.b.x = maze_offset.x + (c+1)*tile_size;
                h_walls[c][r].pos.b.y = maze_offset.y + r*tile_size;
            }
            
            v_walls[c][r].reset();
            h_walls[c][r].reset();
        }
    }
    
    //setup tiles
    for (int c=0; c<NUM_COLS; c++){
        for (int r=0; r<NUM_ROWS; r++){
            tiles[c][r].walls[DIR_UP] = &h_walls[c][r];
            tiles[c][r].walls[DIR_DOWN] = &h_walls[c][r+1];
            
            tiles[c][r].walls[DIR_LEFT] = &v_walls[c][r];
            tiles[c][r].walls[DIR_RIGHT] = &v_walls[c+1][r];
            
            tiles[c][r].reset();
            
            tiles[c][r].c = c;
            tiles[c][r].r = r;
            
            //turn on edge walls
            if (c==0){
                tiles[c][r].walls[DIR_LEFT]->set_and_lock(true);
            }
            if (c==NUM_COLS-1){
                tiles[c][r].walls[DIR_RIGHT]->set_and_lock(true);
            }
            if (r==0){
                tiles[c][r].walls[DIR_UP]->set_and_lock(true);
            }
            if (r==NUM_ROWS-1){
                tiles[c][r].walls[DIR_DOWN]->set_and_lock(true);
            }
        }
    }
    
    //force the starts and ends open
    tiles[0][0].walls[DIR_UP]->set_and_lock(false);
    tiles[0][0].walls[DIR_LEFT]->set_and_lock(false);
    tiles[NUM_COLS-1][NUM_ROWS-1].walls[DIR_RIGHT]->set_and_lock(false);
    tiles[NUM_COLS-1][NUM_ROWS-1].walls[DIR_DOWN]->set_and_lock(false);
    
    //keep going until we find a good start path
    vector<Tile *> path;
    while (path.size() == 0){
        path = find_path(&tiles[0][0], &tiles[NUM_COLS-1][NUM_ROWS-1]);
    }
    
    //block off walls on that path
    trace_path(path);
    
    //add new little paths
    while (get_num_unexplored() > 0){
        Tile * start = get_explored_tile();
        vector<Tile *> side_path = explore(start, ofRandom(3,8));
        if (side_path.size() > 2){
            trace_path(side_path);
            for (int i=0; i<side_path.size()-1; i++){
                side_path[i]->demo_col.set(200,200,255);
            }
        }
        find_orphans();
    }
    
    open_closed_tiles();
    
    //draw this shit
    
    for (int c=0; c<NUM_COLS; c++){
        for (int r=0; r<NUM_ROWS; r++){
            tiles[c][r].make_gcode(&gcode, tile_size);
        }
    }
    
    //draw any undrawn walls
    for (int c=0; c<NUM_COLS+1; c++){
        for (int r=0; r<NUM_ROWS+1; r++){
            if (!v_walls[c][r].has_been_drawn && v_walls[c][r].is_on){
                v_walls[c][r].make_free_wall_gcode(&gcode, false, tile_size);
            }
            if (!h_walls[c][r].has_been_drawn && h_walls[c][r].is_on){
                h_walls[c][r].make_free_wall_gcode(&gcode, true, tile_size);
            }
        }
    }
    
    gcode.sort();
    gcode.save("maze.nc");
    
    
}

//--------------------------------------------------------------
void ofApp::trace_path(vector<Tile *> path){
    
    for (int i=0; i<path.size()-1; i++){
        Tile * cur = path[i];
        Tile * next = path[i+1];
        
        cur->explored = true;
        cur->demo_col.set(200,255,200);
        
        int open_dir = 0;
        if (next->c > cur->c)   open_dir = DIR_RIGHT;
        if (next->c < cur->c)   open_dir = DIR_LEFT;
        if (next->r > cur->r)   open_dir = DIR_DOWN;
        if (next->r < cur->r)   open_dir = DIR_UP;
        
        //open that one
        cur->walls[open_dir]->set_and_lock(false);
        
        //close all non locked walls
        for (int d=0; d<4; d++){
            cur->walls[d]->set(true);
        }
    }
    
}

//--------------------------------------------------------------
//This will ussualy fail. that's fine
vector<Tile *> ofApp::find_path(Tile * start_tile, Tile * end){
    
    vector<Tile *> explored;
    Tile * cur = start_tile;
    
    while (true){
        explored.push_back(cur);
        
        if (cur->c == end->c && cur->r == end->r){
            return explored;
        }
        
        vector<Tile *> possible;
        
        for (int i=0; i<4; i++){
            int new_c = cur->c;
            int new_r = cur->r;
            
            if (i==DIR_UP)  new_r--;
            if (i==DIR_DOWN)  new_r++;
            if (i==DIR_LEFT)  new_c--;
            if (i==DIR_RIGHT)  new_c++;
            
            if (new_c >= 0 && new_c < NUM_COLS && new_r >= 0 && new_r < NUM_ROWS){
                
                Tile * new_tile = &tiles[new_c][new_r];
                
                if (!contains(explored, new_tile)){
                    possible.push_back(new_tile);
                }
            }
        }
        
        if (possible.size() == 0){
            vector<Tile *> empty;
            empty.clear();
            return empty;
        }
        else{
            cur = possible[ (int)ofRandom(possible.size()) ];
        }
    }
}

//--------------------------------------------------------------
//This will ussualy fail. that's fine
vector<Tile *> ofApp::explore(Tile * start_tile, int max_length){
    
    vector<Tile *> explored;
    Tile * cur = start_tile;
    
    while (true){
        explored.push_back(cur);
        
        if (explored.size() >= max_length){
            return explored;
        }
        
        vector<Tile *> possible;
        
        for (int i=0; i<4; i++){
            int new_c = cur->c;
            int new_r = cur->r;
            
            if (i==DIR_UP)  new_r--;
            if (i==DIR_DOWN)  new_r++;
            if (i==DIR_LEFT)  new_c--;
            if (i==DIR_RIGHT)  new_c++;
            
            if (new_c >= 0 && new_c < NUM_COLS && new_r >= 0 && new_r < NUM_ROWS){
                
                Tile * new_tile = &tiles[new_c][new_r];
                
                if (!contains(explored, new_tile) && !new_tile->explored){
                    possible.push_back(new_tile);
                }
            }
        }
        
        if (possible.size() == 0){
            return explored;
        }
        else{
            cur = possible[ (int)ofRandom(possible.size()) ];
        }
    }
}

//--------------------------------------------------------------
bool ofApp::contains(vector<Tile *> list, Tile * tile){
    for (int i=0; i<list.size(); i++){
        if (list[i]->c == tile->c && list[i]->r == tile->r){
            return  true;
        }
    }

    return false;
}

//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw(){
    
//    for (int c=0; c<NUM_COLS; c++){
//        for (int r=0; r<NUM_ROWS; r++){
//            //tiles[c][r].draw();
//        }
//    }
//
//    for (int c=0; c<NUM_COLS+1; c++){
//        for (int r=0; r<NUM_ROWS+1; r++){
//            v_walls[c][r].draw();
//            h_walls[c][r].draw();
//        }
//    }
    
    gcode.draw();

}

//--------------------------------------------------------------
int ofApp::get_num_unexplored(){
    int total = 0;
    for (int c=0; c<NUM_COLS; c++){
        for (int r=0; r<NUM_ROWS; r++){
            if (!tiles[c][r].explored){
                total++;
            }
        }
    }
    
    return total;
}

//--------------------------------------------------------------
Tile * ofApp::get_explored_tile(){
    vector<Tile *> possible;
    for (int c=0; c<NUM_COLS; c++){
        for (int r=0; r<NUM_ROWS; r++){
            if (tiles[c][r].explored){
                possible.push_back(&tiles[c][r]);
            }
        }
    }
    
    if (possible.size() == 0){
        return NULL;
    }
    
    return possible[ (int)ofRandom(possible.size()) ];
}

//--------------------------------------------------------------
//unexplored with no unexplored neigbors can just get marked as good
void ofApp::find_orphans(){
    for (int c=0; c<NUM_COLS; c++){
        for (int r=0; r<NUM_ROWS; r++){
            if (!tiles[c][r].explored){
                int unexplored_neighbors = 0;
                for (int d=0; d<4; d++){
                    
                    int new_c = c;
                    int new_r = r;
                    
                    if (d==DIR_UP)      new_r--;
                    if (d==DIR_DOWN)    new_r++;
                    if (d==DIR_LEFT)    new_c--;
                    if (d==DIR_RIGHT)   new_c++;
                    
                    if (new_c >= 0 && new_c < NUM_COLS && new_r >= 0 && new_r < NUM_ROWS){
                        if (!tiles[new_c][new_r].explored){
                            unexplored_neighbors++;
                        }
                    }
                }
                
                if (unexplored_neighbors == 0){
                    tiles[c][r].explored = true;
                    tiles[c][r].demo_col.set(255,200,200);
                }
            }
        }
    }
}

//--------------------------------------------------------------
void ofApp::open_closed_tiles(){
    for (int c=0; c<NUM_COLS; c++){
        for (int r=0; r<NUM_ROWS; r++){
            
            int wall_count = 0;
            for (int d=0; d<4; d++){
                if (tiles[c][r].walls[d]->is_on){
                    wall_count++;
                }
            }
            
            if (wall_count == 4){
                int rand_side = (int)ofRandom(4);
                while(true){
                    rand_side++;
                    int side = rand_side%4;
                    if ( !tiles[c][r].walls[ side ]->locked){
                        tiles[c][r].walls[ side ]->set(false);
                        break;
                    }
                }
                
                //int rand = ofRandom(4)
            }
            
        }
    }
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    
    if (key == 'r'){
        reset();
    }
    
    if (key == 'c'){
        open_closed_tiles();
    }
    
    if (key == ' '){
        Tile * start = get_explored_tile();
        vector<Tile *> side_path = explore(start, ofRandom(3,8));
        if (side_path.size() > 2){
            trace_path(side_path);
            for (int i=0; i<side_path.size()-1; i++){
                side_path[i]->demo_col.set(200,200,255);
            }
        }else{
            cout<<"reject for too short"<<endl;
        }
        find_orphans();
    }
    
    if (key == 's'){
        ofImage img;
        img.grabScreen(0, 0 , ofGetWidth(), ofGetHeight());
        img.save("screenshot"+ofToString(ofGetFrameNum())+".png");
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
    for (int c=0; c<NUM_COLS; c++){
        for (int r=0; r<NUM_ROWS; r++){
            if (tiles[c][r].test_click(x,y)){
                for (int i=0; i<4; i++){
                    tiles[c][r].walls[i]->is_on = !tiles[c][r].walls[i]->is_on;
                }
            }
            
        }
    }
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
