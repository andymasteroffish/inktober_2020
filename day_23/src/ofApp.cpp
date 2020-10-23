#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofBackground(250);
    gcode.setup(100);
    
    ofSeedRandom(10);
    
    ofPushMatrix();
    ofTranslate(0, -30);
    
    ofVec2f moon_pos = ofVec2f(ofGetWidth()/2, ofGetHeight()*0.55);
    float moon_size = 200;
    
    float line_spacing = 3; //was 2 in test print
    float line_length = 2;
    
    float top_edge_fade_dist = 100;
    float bottom_edge_fade_dist = 100;
    float side_bounds = 50;
    float side_edge_fade_dist = 100;
    float top_bound = ofGetHeight() * 0.1 + top_edge_fade_dist;
    float bottom_bound = ofGetHeight() * 0.9;
    
    float water_angle = PI/12;//PI/8;
    float water_dist = 600;
    float water_top_y = ofGetHeight()*0.8;
    
    int row = 0;
    for (float y=0; y<ofGetHeight(); y+=line_spacing){
        row++;
        for (float x=0; x<ofGetWidth(); x+=line_spacing){
            float this_x = row % 2 == 0 ? x : x+line_spacing/2;
            float chance = 0.5; //base chance
            
            float y_prc = y / ofGetHeight();
            //int y_band = y_prc * 10;
            
            int y_band = ofMap(y, top_bound, bottom_bound, 0, 10, true);
            
            //drop prc for each band
            chance -= y_band * 0.054;
            
            //noise curve
            float noise_pump = 2.1;
            float noise_y = bottom_bound - 100 - ofNoise(x*0.003) * 400;
            if (y > noise_y){
                chance *= noise_pump;
            }

            noise_y = bottom_bound - 0 - ofNoise(x*0.003,100) * 300;
            if (y > noise_y){
                chance *= noise_pump;
            }
            
            //fading off at the edges
            if (y < top_bound){
                chance = ofMap(y, top_bound, top_bound-top_edge_fade_dist, chance, 0, true);
            }
            if (y > bottom_bound-bottom_edge_fade_dist){
                chance = ofMap(y, bottom_bound-bottom_edge_fade_dist, bottom_bound, chance, 0, true);
            }
            
            if (x < side_bounds+side_edge_fade_dist){
                chance = ofMap(x, side_bounds+side_edge_fade_dist, side_bounds, chance, 0, true);
            }
            if (x > ofGetWidth()- side_bounds-side_edge_fade_dist){
                chance = ofMap(x, ofGetWidth()- side_bounds-side_edge_fade_dist, ofGetWidth()- side_bounds, chance, 0, true);
            }
            
            
            //are we inside the moon?
            if (ofDistSquared(this_x, y, moon_pos.x, moon_pos.y) < powf(moon_size,2)){
                
                float moon_prc = ofMap(y,moon_pos.y-moon_size, moon_pos.y+moon_size, 1, 0);
                moon_prc = powf(moon_prc, 2);
                float moon_drop = 0.05 + 0.7 * moon_prc;
                
                chance *= moon_drop;
            }
            
            
            //draw the line
            if (ofRandomuf() < chance){
                gcode.line(this_x,y, this_x+line_length, y+line_length);
            }
            
        }
    }
    
    
    //water line
    vector<GLine> lines;
    
    for (float start_x=-ofGetWidth()*0.6; start_x<ofGetWidth()/2; start_x+= 13){
        float start_y = water_top_y + ofNoise(abs(start_x)*0.01) * abs(start_x)*0.15;  //start_x*0.1;
        
        float right_x = ofGetWidth()/2 + start_x;
        GLine right_line;
        right_line.set(right_x+cos(water_angle)*water_dist, start_y+sin(water_angle)*water_dist, right_x, start_y);
        right_line.clip_to_other_line(ofGetWidth()/2, water_top_y, ofGetWidth()/2, ofGetHeight());
        
        vector<GLine> segments = right_line.get_segments(30);
        for (int i=0; i<segments.size(); i++){
            lines.push_back(segments[i]);
        }
        
        
        float left_x = ofGetWidth()/2 - start_x;
        GLine left_line;
        left_line.set(left_x-cos(water_angle)*water_dist, start_y+sin(water_angle)*water_dist, left_x, start_y);
        left_line.clip_to_other_line(ofGetWidth()/2, water_top_y, ofGetWidth()/2, ofGetHeight());
        segments.clear();
        segments = left_line.get_segments(30);
        for (int i=0; i<segments.size(); i++){
            lines.push_back(segments[i]);
        }
    }
    
    
    
    
    //shines
    for (int k=0; k<80; k++){
        ofVec2f center = ofVec2f(ofRandom(0,ofGetWidth()), ofRandom(water_top_y,ofGetHeight()));
        float base_angle = PI/4;
        float angle = PI*0.1;
        float length = 25;
        float width = 3;
        
        float tan_angle = base_angle + PI/2;
        
        ofVec2f top = ofVec2f(center.x+cos(base_angle)*length, center.y+sin(base_angle)*length);
        ofVec2f bottom = ofVec2f(center.x-cos(base_angle)*length, center.y-sin(base_angle)*length);
        
        vector<ofVec2f> pnts;
        pnts.push_back(ofVec2f( top.x+cos(tan_angle)*width, top.y+sin(tan_angle)*width ));
        pnts.push_back(ofVec2f( top.x-cos(tan_angle)*width, top.y-sin(tan_angle)*width ));
        pnts.push_back(ofVec2f( bottom.x-cos(tan_angle)*width, bottom.y-sin(tan_angle)*width ));
        pnts.push_back(ofVec2f( bottom.x+cos(tan_angle)*width, bottom.y+sin(tan_angle)*width ));
        for (int i=0; i<lines.size(); i++){
            lines[i].clip_outside_polygon(pnts);
        }
    }
    
    //water's edge
    vector<ofVec2f> left_edge;
    float cur_x = side_bounds;
    for (float y = water_top_y; y<ofGetHeight(); y++){
        cur_x += 0.2 + ofMap(ofNoise(y*0.01), 0, 1, -1,3);
        left_edge.push_back(ofVec2f(cur_x,y));
    }
    left_edge.push_back(ofVec2f(cur_x, ofGetHeight()));
    left_edge.push_back(ofVec2f(0, ofGetHeight()));
    left_edge.push_back(ofVec2f(0, water_top_y));
    
    vector<ofVec2f> right_edge;
    cur_x = ofGetWidth()-side_bounds;
    for (float y = water_top_y; y<ofGetHeight(); y++){
        cur_x -= 0.2 + ofMap(ofNoise(y*0.01,99), 0, 1, -1,3);
        right_edge.push_back(ofVec2f(cur_x,y));
    }
    right_edge.push_back(ofVec2f(cur_x, ofGetHeight()));
    right_edge.push_back(ofVec2f(ofGetWidth(), ofGetHeight()));
    right_edge.push_back(ofVec2f(ofGetWidth(), water_top_y));
    
    //tetsing
    //gcode.polygon(edge);
    
    
    //add 'em
    cout<<"lines: "<<lines.size()<<endl;
    for (int i=0; i<lines.size(); i++){
        lines[i].clip_outside_polygon(left_edge);
        lines[i].clip_outside_polygon(right_edge);
        gcode.line(lines[i]);
    }
    
    ofPopMatrix();
    
    ofRectangle clip;
    clip.set(1,1, 565, 757);
    //gcode.clip_outside(clip);
    
    gcode.sort();
    gcode.save("inktober_23.nc");
    
    cout<<"height: "<<ofGetHeight()<<endl;
}

//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw(){
    
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

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
    cout<<"mouse: "<<x<<" , "<<y<<endl;
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
