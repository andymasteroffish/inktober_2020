#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofBackground(250);
    
    ofSeedRandom(150);
    
    gcode.setup(100);
    
    padding = 100;
    tile_size = 2;
   
    for (int i=0; i<20; i++){
        ofVec2f pos = get_free_spot();
        Node n = Node( pos.x, pos.y);
        nodes.push_back(n);
    }
    
    //filling the dird to make a voronoi diagram
    //http://www.procjam.com/seeds/issues/4/issue4.pdf
    for (int x=0; x<GRID_W; x++){
        for (int y=0; y<GRID_H; y++){
            grid[x][y] = -1;
            ofVec2f pos = ofVec2f(x*tile_size, y*tile_size);
            if (pos.x>padding && pos.x < ofGetWidth()-padding && pos.y>padding && pos.y<ofGetHeight()-padding){
                float closest_dist = 99999999;
                for (int i=0; i<nodes.size(); i++){
                    float dist_sq = ofDistSquared(pos.x, pos.y, nodes[i].x, nodes[i].y);
                    if (dist_sq < closest_dist){
                        closest_dist = dist_sq;
                        grid[x][y] = i;
                    }
                }
            }
        }
    }
    
    
    //after filling the grid, find the edges
    bool kill_cell[GRID_W][GRID_H];
    for (int x=0; x<GRID_W; x++){
        for (int y=0; y<GRID_H; y++){
            kill_cell[x][y] = false;
            //ignore -1
            if (grid[x][y] >= 0){
                int num_dif_neighbors = 0;
                //cardinal directions
                if (grid[x][y-1] != grid[x][y]) num_dif_neighbors++;
                if (grid[x+1][y] != grid[x][y]) num_dif_neighbors++;
                if (grid[x][y+1] != grid[x][y]) num_dif_neighbors++;
                if (grid[x-1][y] != grid[x][y]) num_dif_neighbors++;
                
                if (num_dif_neighbors == 0) kill_cell[x][y] = true;
            }
        }
    }
    
    //knock 'em out
    for (int x=0; x<GRID_W; x++){
        for (int y=0; y<GRID_H; y++){
            if (kill_cell[x][y])    grid[x][y] = -1;
        }
    }
    
    //for each node, find all points
    for (int i=0; i<nodes.size(); i++){
        vector<ofVec2f> unordered;
        for (int x=0; x<GRID_W; x++){
            for (int y=0; y<GRID_H; y++){
                if (grid[x][y] == i){
                    ofVec2f pos = ofVec2f(x*tile_size + tile_size/2, y*tile_size + tile_size/2);
                    unordered.push_back(pos);
                }
            }
        }
        nodes[i].sort_pnts(unordered);
    }
    
    //fill 'em!
    for (int i=0; i<nodes.size(); i++){
        nodes[i].make_gcode(&gcode);
    }

    
//    ofPushMatrix();
//    ofTranslate(-289, -346);
//    nodes[2].make_gcode(&gcode);
//    ofPopMatrix();
    
    gcode.sort();
    gcode.save("inktober_31.nc");

}

//--------------------------------------------------------------
ofVec2f ofApp::get_free_spot(){
    vector<ofVec2f> possible;
    
    int num_choices = 5;
    
    
    ofVec2f best_choice = ofVec2f();
    float furthest_close_dist = 0;
    for (int i=0; i<num_choices; i++){
        ofVec2f pos = ofVec2f( ofRandom(padding,ofGetWidth()-padding), ofRandom(padding,ofGetHeight()-padding) );
        
        //find the closest existing node
        float closest_dist = 99999;
        for (int n=0; n<nodes.size(); n++){
            float dist_sq = ofDistSquared(nodes[n].x, nodes[n].y, pos.x, pos.y);
            if (dist_sq < closest_dist){
                closest_dist = dist_sq;
            }
        }
        
        //see if this is the furthest away so far
        if (closest_dist > furthest_close_dist){
            furthest_close_dist = closest_dist;
            best_choice = pos;
        }
    }
    
    return best_choice;
    
}



//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw(){
    
    gcode.draw();
    
//    for (int x=0; x<GRID_W; x++){
//        for (int y=0; y<GRID_H; y++){
//            if (grid[x][y] >= 0){
//                ofFill();
//                ofSetColor( grid[x][y]*12);
//                ofDrawRectangle(x*tile_size, y*tile_size, tile_size, tile_size);
//            }
//        }
//    }

    
//    for (int i=0; i<nodes.size(); i++){
//        ofSetColor(ofColor::fromHsb(i*12, 255, 200));
//        nodes[i].draw( true );
//    }
    
//    ofSetColor(0);
//    for (int i=0; i<outlines.size(); i++){
//        outlines[i].draw();
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
    cout<<"mouse "<<x<<" , "<<y<<endl;
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
