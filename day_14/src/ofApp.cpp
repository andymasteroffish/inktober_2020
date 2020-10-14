#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofBackground(250);
    
    gcode.setup(100);
    gcode.show_transit_lines = false;
    gcode.show_path_with_color = false;
    
    //****************
    //OPTION C
    //****************
    if (true){
        float y_padding = 100;
        float x_dist_to_grow = 140;
        
        float noise_x_speed = 0.004;//ofMap(ofGetMouseX(), 0, ofGetWidth(), 0.001, 0.1);
        float noise_y_speed = 0.005;//ofMap(ofGetMouseY(), 0, ofGetHeight(), 0.001, 0.1);
        
        float turn_off_val = 0.15;
        
        float y_step = 40;
        float max_offset = 50;
        float prc_step = 0.125;
        
        
        int kill_count = 0;
        
        for (float y=y_padding; y<ofGetHeight()-y_padding+50; y+=y_step ){
            
            float padding_noise_speed = 0.002;
            float padding_min = 50;
            float padding_max = 100;
            float x_padding_left = padding_min + ofNoise(y*padding_noise_speed,  99999) * (padding_max-padding_min);
            float x_padding_right = padding_min + ofNoise(y*padding_noise_speed, 999999) * (padding_max-padding_min);
            
            vector<ofVec2f> used_pnts;
            
            for (float noise_power=0; noise_power<1; noise_power += prc_step){
                
                bool is_on = false;
                
                for (float x=x_padding_left; x<ofGetWidth()-x_padding_right; x+=1 ){
                    
                    float noise_val = ofNoise(x*noise_x_speed, y*noise_y_speed) * noise_power;
                    
                    if (is_on){
                        if (noise_val < turn_off_val){
                            gcode.end_shape(false);
                            is_on = false;
                        }
                    }else{
                        if (noise_val > turn_off_val){
                            gcode.begin_shape();
                            is_on = true;
                        }
                        
                    }
                    
                    if (x<x_dist_to_grow){
                        float prc = (x-x_padding_left)/(x_dist_to_grow-x_padding_left);
                        noise_val *= prc;
                    }
                    if (x>ofGetWidth()-x_padding_right-x_dist_to_grow){
                        float prc = ofMap(x,ofGetWidth()-x_padding_right-x_dist_to_grow, ofGetWidth()-x_padding_right, 1, 0);
                        noise_val *= prc;
                    }
                    
                    ofVec2f pos;
                    pos.x = x;
                    pos.y = y - noise_val * max_offset;
                    
                    bool can_draw = true;
                    for (int i=0; i<used_pnts.size(); i++){
                        if (ofDist(used_pnts[i].x, used_pnts[i].y, pos.x, pos.y) < 1){
                            can_draw = false;
                            kill_count++;
                            break;
                        }
                    }
                    
                    if (is_on && can_draw){
                        gcode.vertex(pos);
                        used_pnts.push_back(pos);
                    }
                    
                    
                }
                if (is_on){
                    gcode.end_shape(false);
                }
            }
        }
         gcode.sort();
         gcode.save("inktober_13_c.nc");
        cout<<"killed pnts: "<<kill_count<<endl;
    }
    
    
    
    //****************
    //OPTION B
    //****************
    if (false){
        float y_padding = 100;
        float x_dist_to_grow = 140;
        
        float noise_x_speed = 0.005;//ofMap(ofGetMouseX(), 0, ofGetWidth(), 0.001, 0.1);
        float noise_y_speed = 0.007;//ofMap(ofGetMouseY(), 0, ofGetHeight(), 0.001, 0.1);
        
        float turn_off_val = 0.1;
        
        
        for (float y=y_padding; y<ofGetHeight()-y_padding; y+=20 ){
            
            float padding_noise_speed = 0.003;
            float padding_min = 30;
            float padding_max = 60;
            float x_padding_left = padding_min + ofNoise(y*padding_noise_speed,  99999) * (padding_max-padding_min);
            float x_padding_right = padding_min + ofNoise(y*padding_noise_speed, 999999) * (padding_max-padding_min);
            
            for (float noise_power=0; noise_power<1; noise_power += 0.2){
                
                bool is_on = false;
                
                
                for (float x=x_padding_left; x<ofGetWidth()-x_padding_right; x+=1 ){
                    
                    float noise_val = ofNoise(x*noise_x_speed, y*noise_y_speed) * noise_power;
                    if (is_on){
                        if (noise_val < turn_off_val){
                            gcode.end_shape(false);
                            is_on = false;
                        }
                    }else{
                        if (noise_val > turn_off_val){
                            gcode.begin_shape();
                            is_on = true;
                        }
                    }
                    
                    if (x<x_dist_to_grow){
                        float prc = (x-x_padding_left)/(x_dist_to_grow-x_padding_left);
                        noise_val *= prc;
                    }
                    if (x>ofGetWidth()-x_padding_right-x_dist_to_grow){
                        float prc = ofMap(x,ofGetWidth()-x_padding_right-x_dist_to_grow, ofGetWidth()-x_padding_right, 1, 0);
                        noise_val *= prc;
                    }
                    
                    int offset_y = noise_val * 20;
                    offset_y /= 4;
                    offset_y *= 4;
                    
                    ofVec2f pos;
                    pos.x = x;
                    pos.y = y - offset_y;
                    
                    if (is_on){
                        gcode.vertex(pos);
                    }
                    
                    
                }
                if (is_on){
                    gcode.end_shape(false);
                }
            }
        }
        gcode.sort();
        gcode.save("inktober_13_b.nc");
    }
     
    //****************
    //OPTION A
    //****************
    if (false){
        float y_padding = 100;
        float x_dist_to_grow = 140;
        
        float noise_x_speed = 0.003;
        float noise_y_speed = 0.004;
        
        float turn_off_val = 0.1;
        
        float x_spacing = 20 ;
        float y_spacing = 40 ;
        float max_wave_height = 42;
        int y_step = 10 ;
        
        vector<GLine> lines;
        vector<ofVec2f> start_pnts;
        
        
        
        for (float y=y_padding ; y<ofGetHeight()-y_padding ; y+=y_spacing ){
            
            float padding_noise_speed = 0.003;
            float padding_min = 30;
            float padding_max = 120;
            float x_padding_left = padding_min + ofNoise(y*padding_noise_speed,  99999) * (padding_max-padding_min);
            float x_padding_right = padding_min + ofNoise(y*padding_noise_speed, 999999) * (padding_max-padding_min);
            
            bool is_on = false;
            Blob cur_blob;
            
            ofVec2f prev_pos = ofVec2f(x_padding_left, y);
            //gcode.begin_shape();
            for (float x=x_padding_left; x<ofGetWidth()-x_padding_right; x+=x_spacing ){
                
                float noise_val = ofNoise(x*noise_x_speed, y*noise_y_speed) ;
                
                if (x<x_dist_to_grow){
                    float prc = (x-x_padding_left)/(x_dist_to_grow-x_padding_left);
                    noise_val *= prc;
                }
                if (x>ofGetWidth()-x_padding_right-x_dist_to_grow){
                    float prc = ofMap(x,ofGetWidth()-x_padding_right-x_dist_to_grow, ofGetWidth()-x_padding_right, 1, 0);
                    noise_val *= prc;
                }
                
                int offset_y = noise_val * max_wave_height;
                offset_y /= y_step;
                offset_y *= y_step;
                
                if (is_on){
                    if (offset_y < 1){
                        is_on = false;
                       
                    }
                }else{
                    if (offset_y > 1){
                        is_on = true;
                        cur_blob.reset();
                    }
                }
                
                ofVec2f pos;
                pos.x = x;
                pos.y = y - offset_y;
                
                //if they were the same, just connect them
                if ( abs(prev_pos.y-pos.y) < 1){
                    if (is_on){
                        cur_blob.pnts.push_back(pos);
                    }
                }
                //otherwise curve it
                else{
                    //ofVec2f prev_mid_pos = ofVec2f(prev_pos);
                    for (float c=0; c<x_spacing; c++){
                        float prc = c/x_spacing;
                        prc = powf(prc, 2);
                        ofVec2f mid_pos;
                        mid_pos.x = prev_pos.x + c;
                        mid_pos.y = (1.0-prc)*prev_pos.y + prc * pos.y;
                        cur_blob.pnts.push_back(mid_pos);
                    }
                    
                    if (offset_y == 0){
                         blobs.push_back(cur_blob);
                    }
                }
                
                
                prev_pos.set(pos);
                
            }
            //gcode.end_shape(false);
            
        }
        
        //add the lines
        for (int i=0; i<blobs.size(); i++){
            blobs[i].make_gcode(&gcode);
        }
        
        //and the fill
        for (int i=0; i<start_pnts.size(); i+=5){
            GLine fill_line;
            fill_line.set(start_pnts[i], start_pnts[i]);
            bool keep_going = true;
            while (keep_going){
                fill_line.b.x += 2;
                
                for (int k=0; k<lines.size(); k++){
                    if (lines[k].intersects(fill_line)){
                        keep_going = false;
                    }
                }
                
            }
            
            gcode.line(fill_line);
        }
        
        gcode.sort();
        gcode.save("inktober_13_a.nc");
    }
    
}

//--------------------------------------------------------------
void ofApp::update(){
    
    
    //cout<<ofGetWidth()<<" , "<<ofGetHeight()<<endl;

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
