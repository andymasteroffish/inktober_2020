#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){

    ofBackground(250);
    gcode.setup(100);
    gcode.show_path_with_color = false;
    gcode.show_transit_lines = false;
    
    gcode_red.setup(100);
    gcode_red.show_path_with_color = false;
    gcode_red.show_transit_lines = false;
    gcode_red.show_do_not_reverse = true;
    gcode_red.demo_col = ofColor(255,0,0);
    
    
    float padding = 100;
    float spacing = 150;
    
    for (int i=1; i<=3; i++){
        for (int k=-1;k<=1;k+=2){
            
            cout<<"i live"<<endl;
            
            float x = ofGetWidth()/2 + (40 + 130 * i) * k;
            float y = 100 * i;
            float y2 = y + 300 + 40 * i;
            float size = 40 + i * 10;
            
            cout<<"x: "<<x<<"  y: "<<y<<endl;
            
            Shape shape;
            shape.setup(x,y, size );
            shapes.push_back(shape);
            
            Shape shape2;
            shape.setup(x,y2, size);
            shapes.push_back(shape);
            
        }
    }
    
    
    //bricks
    int num_rows = 20;
    int num_cols = 15;
    float bottom_w = ofGetWidth() * 0.6;
    float top_w = ofGetWidth() * 0.12;
    float top_y = ofGetHeight() * 0.45;
    
    GLine left,right;
    left.set(ofGetWidth()/2-bottom_w, ofGetHeight()+10, ofGetWidth()/2-top_w, top_y);
    right.set(ofGetWidth()/2+bottom_w, ofGetHeight()+10, ofGetWidth()/2+top_w, top_y);
    
    float curve = 0.7;
    for (int r=1; r<=num_rows; r++){
        float yt_prc = (float)r / (float)num_rows;
        float yb_prc = (float)(r-1) / (float)num_rows;
        
        yt_prc = powf(yt_prc, curve);
        yb_prc = powf(yb_prc, curve);
        
        float yt = (1.0-yt_prc)*left.a.y + yt_prc * left.b.y;
        float yb = (1.0-yb_prc)*left.a.y + yb_prc * left.b.y;
        
        float top_start_x = (1.0-yt_prc)*left.a.x + yt_prc * left.b.x;
        float top_end_x = (1.0-yt_prc)*right.a.x + yt_prc * right.b.x;
        float bot_start_x = (1.0-yb_prc)*left.a.x + yb_prc * left.b.x;
        float bot_end_x = (1.0-yb_prc)*right.a.x + yb_prc * right.b.x;
        
        for (int c=0; c<num_cols; c++){
            float x_prc1 = (float)c / (float)num_cols;
            float x_prc2 = (float)(c+1) / (float)num_cols;
            
            float spacing = (0.25/num_cols);
            if (r%2==0){
                x_prc1 -= spacing;
                x_prc2 -= spacing;
            }else{
                x_prc1 += spacing;
                x_prc2 += spacing;
            }
            
            Brick brick;
            //top left
            brick.pnts[0].x = (1.0-x_prc1)*top_start_x + x_prc1 * top_end_x;
            brick.pnts[0].y = yt;
            //top right
            brick.pnts[1].x = (1.0-x_prc2)*top_start_x + x_prc2 * top_end_x;
            brick.pnts[1].y = yt;
            //bottom right
            brick.pnts[2].x = (1.0-x_prc2)*bot_start_x + x_prc2 * bot_end_x;
            brick.pnts[2].y = yb;
            //bottom left
            brick.pnts[3].x = (1.0-x_prc1)*bot_start_x + x_prc1 * bot_end_x;
            brick.pnts[3].y = yb;
            
            brick.set_center();
            brick.row = r;
            
            float shift_prc = (float)r / (float)num_rows;
            shift_prc = powf(shift_prc, 2);
            float max_y_shift = shift_prc*40;
            float max_x_shift = shift_prc*10;
            brick.shift(ofRandom(-max_x_shift, max_x_shift), -ofRandom(0,max_y_shift));
            
            brick.make_gcode(&gcode_red);
            
            bricks.push_back(brick);
            
        }
    }
    
    
    //the eye
    ofVec2f eye_center = ofVec2f(ofGetWidth()/2, ofGetHeight()*0.32);
    vector<ofVec2f> eye_curve;
    float eye_angle_end = PI/8;
    float eye_w = 90;
    float eye_h = 65;
    for (float a=eye_angle_end; a<=PI-eye_angle_end; a+=0.1) {
        ofVec2f top_pos;
        top_pos.x = eye_center.x + cos(a)*eye_w;
        top_pos.y = eye_center.y - sin(a)*eye_h;
        eye_curve.push_back(top_pos);
        
    }
    for (float a=PI-eye_angle_end; a>=eye_angle_end; a-=0.1) {
        ofVec2f bot_pos;
        bot_pos.x = eye_center.x + cos(a)*eye_w;
        bot_pos.y = eye_center.y + sin(a)*eye_h;
        eye_curve.push_back(bot_pos);
    }
    
    for (int i=0; i<eye_curve.size(); i++){
        gcode.line(eye_curve[i],eye_curve[(i+10)%eye_curve.size()]);
        gcode.line(eye_curve[i],eye_curve[(i+30)%eye_curve.size()]);
    }
    
    //red center
    for (float a=0; a<PI; a+=0.3){
        float dist = 13;
        ofVec2f p1 = ofVec2f(eye_center);
        ofVec2f p2 = ofVec2f(eye_center);
        p1.x += cos(a)*dist;
        p1.y += sin(a)*dist;
        p2.x += cos(a+PI)*dist;
        p2.y += sin(a+PI)*dist;
        gcode_red.line(p1,p2);
    }
    
    //the star
    vector<ofVec2f> star_pnts;
    ofVec2f star_center = ofVec2f(ofGetWidth()/2, ofGetHeight()*0.1);
    for (int i=0; i<5; i++){
        float angle = i * (TWO_PI/5.0) + PI/2;
        float dist = 50;
        ofVec2f pos;
        pos.x = star_center.x + cos(angle) * dist;
        pos.y = star_center.y + sin(angle) * dist;
        star_pnts.push_back(pos);
    }
    
    //star lines
    for (int x=-2;x<=2;x++){
        for (int y=-2;y<=2;y++){
            ofVec2f offset = ofVec2f(x*0.5,y*0.5);
            gcode_red.line(star_pnts[0]+offset, star_pnts[2]+offset);
            gcode_red.line(star_pnts[2]+offset, star_pnts[4]+offset);
            gcode_red.line(star_pnts[4]+offset, star_pnts[1]+offset);
            gcode_red.line(star_pnts[1]+offset, star_pnts[3]+offset);
            gcode_red.line(star_pnts[3]+offset, star_pnts[0]+offset);
        }
    }
    
    //bonus lines
    
    
    //connection
    for (int i=0; i<eye_curve.size()/2; i++){
        for (int k=0; k<star_pnts.size(); k++){
            gcode.line(eye_curve[i].x, eye_curve[i].y-15, star_pnts[k].x, star_pnts[k].y);
        }
    }
    
    
    //add the shapes
    for (int i=0; i<shapes.size(); i++){
        for (int k=0; k<shapes[i].outline.size(); k++){
            gcode.line(shapes[i].outline[k], shapes[i].outline[ (k+1)%shapes[i].outline.size()]);
        }
        for (int k=0; k<shapes[i].outline.size(); k+=2){
            gcode.line(shapes[i].outline[k], shapes[i].center);
        }
        for (int k=0; k<shapes[i].shading.size(); k++){
            gcode.line(shapes[i].shading[k]);
        }
    }
    
    float safe_bounds = 10;
    safe_zone.set(safe_bounds, safe_bounds, ofGetWidth()-safe_bounds, ofGetHeight()-safe_bounds);
    
    gcode.sort();
    gcode.save("inktober_03_black.nc");
    
    gcode_red.sort();
    gcode_red.set_outwards_only_bounds(safe_zone);
    gcode_red.save("inktober_03_red.nc");
}

//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw(){
    
//    ofSetColor(0);
//    ofNoFill();
//    for (int i=0; i<shapes.size(); i++){
//        shapes[i].draw();
//    }

    gcode_red.draw();
    gcode.draw();
    
    
    
//    ofSetColor(255,0,0);
//    ofNoFill();
//    for (int i=0; i<bricks.size(); i++){
//        bricks[i].draw();
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
