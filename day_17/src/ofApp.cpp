#include "ofApp.h"


//--------------------------------------------------------------
void ofApp::setup(){
    ofBackground(250);
    
    for (int i=0; i<4; i++){
        gcode[i].setup(100);
        gcode[i].show_transit_lines = false;
        gcode[i].show_do_not_reverse = true;
    }
    gcode[0].demo_col = ofColor::maroon;
    gcode[1].demo_col = ofColor::orange;
    gcode[2].demo_col = ofColor::green;
    gcode[3].demo_col = ofColor::blue;
    
    ofSeedRandom(0);
    
    tick_count = 0;
    
    for (int l=0; l<NUM_LAYERS; l++){
        int num_particles = 30 + l*30;
        float start_dist = 10 + l*25;
        
        float center_angle = l *  (TWO_PI/NUM_LAYERS);
        float center_offset = 10 + (NUM_LAYERS-l-1)*30;
        ofVec2f center;
        center.x = ofGetWidth()/2 + cos(center_angle)*center_offset;
        center.y = ofGetHeight()/2 + sin(center_angle)*center_offset;
        
        for (int i=0; i<num_particles; i++){
            float angle = i * (TWO_PI/num_particles);
            
            Particle p;
            p.setup(center.x+cos(angle)*start_dist, center.y+sin(angle)*start_dist);
            
            float angle_range = 0;//PI/16;
            float vel_angle = angle + ofRandom(-angle_range, angle_range);
            float vel_force = ofRandom(4,9) * (l+1);
            p.vel.x += cos(vel_angle) * vel_force * 1.2;
            p.vel.y += sin(vel_angle) * vel_force;
            
            particles[l].push_back(p);
        }
    }
    
    for (int i=0; i<2000; i++){
        tick();
    }
    
    //generate bounds
    vector<ofVec2f> bounds[NUM_LAYERS];
    for (int l=0; l<NUM_LAYERS; l++){
        for (int i=0; i<particles[l].size(); i++){
            bounds[l].push_back(particles[l][i].pos);
        }
    }
    
    
    
    
    //make shapes
    for (int l=0; l<NUM_LAYERS; l++){
        for (int i=0; i<12000; i++){
            if (i%100 == 0) cout<<"layer "<<l<<" shape "<<i<<endl;
            float x = ofRandom(ofGetWidth());
            float y = ofRandom(ofGetWidth());
            
            Shape shape;
            int sides = l+3;
            if (l==NUM_LAYERS-1)    sides = 15;
            
            float size = ofRandom(6,12);
            
            shape.setup(x, y, size, sides);
            
            //stay inside out shape
            shape.clip_inside(bounds[l]);
            
            //do not go inside any inner shapes
            for (int k=l-1; k>=0; k--){
                shape.clip_outside(bounds[k]);
            }
            
            //is any part still visible?
            if (shape.check_is_visible()){
                
                //trim it against other shapes
                for (int k=0; k<shapes[l].size(); k++){
                    shape.clip_outside(shapes[l][k].pnts);
                }
                
                //add it if it is still visible
                if (shape.check_is_visible()){
                    shapes[l].push_back(shape);
                }
            }
        }
    }
    
    //generate gcode
    ofRectangle safe_zone;
    safe_zone.set(10,0,ofGetWidth()-20, ofGetHeight());
    for (int l=0; l<NUM_LAYERS; l++){
        //gcode.clear();
        for (int i=0; i<shapes[l].size(); i++){
            shapes[l][i].make_gcode(&gcode[l]);
        }
        
        
        gcode[l].set_outwards_only_bounds(safe_zone);
        
        gcode[l].sort();
        
        gcode[l].save("inktober_16_"+ofToString(l)+".nc");
    }
    
    //arcs
    float arc_size_x = 15;
    float arc_size_y = 10;
    float arc_spacing_y = 40;
    int num_arc_pnts = 10;
    float arc_step = PI/num_arc_pnts;
    
    bool face_up = true;
    
    vector<GLine> arc_lines;
    int row = 0;

     for (float y=-6; y<ofGetHeight(); y+=arc_spacing_y){
         row++;
         face_up = row%2==0;
        
         for (float x=0; x<ofGetWidth(); x+=arc_size_x*2){
            
            ofVec2f prev_pos;
            for (int i=0; i<=num_arc_pnts; i++){
                float angle = i*arc_step;
                ofVec2f pos;
                pos.x = x + cos(angle) * arc_size_x;
                pos.y = y + sin(angle) * arc_size_y * (face_up ? 1 : -1);
                if (face_up){
                    pos.y += 10;
                }
                if (row%2==0){
                    pos.x += arc_size_x;
                }
                
                if (i>0){
                    GLine line;
                    line.set(prev_pos, pos);
                    line.clip_outside_polygon(bounds[NUM_LAYERS-1]);
                    arc_lines.push_back(line);
                }
                
                prev_pos.set(pos);
            }
            face_up = !face_up;
        }
    }
    
    cout<<"arc lines "<<arc_lines.size()<<endl;
    for (int i=0; i<arc_lines.size(); i++){
        gcode[3].line(arc_lines[i]);
    }
    
    gcode[3].set_outwards_only_bounds(safe_zone);
    gcode[3].sort();
    gcode[3].save("inktober_16_3.nc");

}

//--------------------------------------------------------------
void ofApp::tick(){
    tick_count++;
    
    float repel_strength = 0.001;
    float repel_range = 100;
    
    float attract_strength = 0.004;
    float attract_range = 400;
    
    float noise_strength = .07;
    
    for (int l=0; l<NUM_LAYERS; l++){
        for (int i=0; i<particles[l].size(); i++){
            //repel
            for (int k=i+1; k<particles[l].size(); k++){
                particles[l][i].complexAttract(particles[l][k].pos, -repel_strength, repel_range);
                particles[l][k].complexAttract(particles[l][i].pos, -repel_strength, repel_range);
            }
            
            //attract to neighbors
            particles[l][i].complexAttract(particles[l][(i+1)%particles[l].size()].pos, attract_strength, attract_range);
            particles[l][i].complexAttract(particles[l][(i-1+particles[l].size())%particles[l].size()].pos, attract_strength, attract_range);
            
            particles[l][i].update();
        }
    }
    
}

//--------------------------------------------------------------
void ofApp::update(){
    //tick();
}

//--------------------------------------------------------------
void ofApp::draw(){
    
    for (int i=0; i<4; i++){
        gcode[i].draw();
    }
    
//    ofSetColor(0);
//    for (int l=0; l<NUM_LAYERS; l++){
//        for (int i=0; i<particles[l].size(); i++){
//            particles[l][i].draw();
//            ofDrawLine(particles[l][i].pos, particles[l][(i+1)%particles[l].size()].pos);
//        }
//    }
//    ofDrawBitmapString("ticks: "+ofToString(tick_count), 10,15);

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
