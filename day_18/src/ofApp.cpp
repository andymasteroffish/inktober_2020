#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofBackground(250);
    
    thick_line_layers = 4;
    thick_line_offset = 0.5;
    
    for (int i=0; i<NUM_COLS; i++){
        gcode[i].setup(100);
        gcode[i].show_transit_lines = false;
        gcode[i].demo_col = ofColor::fromHsb(i*70,255,150);
    }
    
    ofSeedRandom(0);
    
    float tile_s = 40;
    int num_cols = 18;
    int num_rows = 20;
    
    float padding_x = (ofGetWidth()-(num_cols)*tile_s)/2;
    float padding_y = (ofGetHeight()-(num_rows-1)*tile_s)/2;
    float section_y_spacing = 30;
    
    
    //int num_cols = 13;
    
    //float tile_s = (ofGetWidth()-padding_x*2)/num_cols;
    //cout<<"tile_s"<<tile_s<<endl;
    
    //int num_rows = 1 + (ofGetHeight()-padding_y*2)/tile_s;
    
    ofPushMatrix();
    ofTranslate(padding_x-4, padding_y-section_y_spacing);
    
//    for (int c=0; c<num_cols; c++){
//        for (int r=0; r<num_rows; r++){
//            gcode[0].rect(c*tile_s+5, r*tile_s+5, tile_s-10, tile_s-10);
//        }
//    }
    
    //section 1
    stroke_col = 0;
    draw_polar(ofRectangle(0*tile_s, 0*tile_s, tile_s*6, tile_s*6), 2);
    draw_bar(ofRectangle(7*tile_s, 0*tile_s, 4*tile_s, 3*tile_s));
    draw_dial(ofRectangle(7*tile_s, 4*tile_s, 5*tile_s, 2*tile_s));
    draw_pie(ofRectangle(15*tile_s, 0*tile_s, 3*tile_s, 3*tile_s), 2);
    draw_line(ofRectangle(12*tile_s, 0*tile_s, 2*tile_s, 3*tile_s), 2);
    draw_line(ofRectangle(13*tile_s, 4*tile_s, 5*tile_s, 2*tile_s), 3);
    
    //section 2
    ofTranslate(0, section_y_spacing);
    stroke_col = 1;
    draw_line(ofRectangle(6*tile_s, 7*tile_s, tile_s*6, tile_s*6), 4);
    draw_pie(ofRectangle(0*tile_s, 7*tile_s, 2*tile_s, 2*tile_s), 1);
    draw_pie(ofRectangle(3*tile_s, 7*tile_s, 2*tile_s, 2*tile_s), 2);
    draw_dial(ofRectangle(0*tile_s, 10*tile_s, 5*tile_s, 3*tile_s));
    draw_bar(ofRectangle(13*tile_s, 7*tile_s, 5*tile_s, 3*tile_s));
    draw_polar(ofRectangle(13*tile_s, 11*tile_s, tile_s*2, tile_s*2), 1);
    draw_polar(ofRectangle(16*tile_s, 11*tile_s, tile_s*2, tile_s*2), 1);

    //section 3
    ofTranslate(0, section_y_spacing);
    stroke_col = 2;
    draw_pie(ofRectangle(12*tile_s, 14*tile_s, 6*tile_s, 6*tile_s), 3);
    draw_pie(ofRectangle(9*tile_s, 14*tile_s, 2*tile_s, 2*tile_s), 1);
    draw_line(ofRectangle(5*tile_s, 17*tile_s, 6*tile_s, 3*tile_s), 2);
    draw_dial(ofRectangle(5*tile_s, 14*tile_s, 3*tile_s, 2*tile_s));
    draw_bar(ofRectangle(0*tile_s, 14*tile_s, 4*tile_s, 3*tile_s));
    draw_bar(ofRectangle(0*tile_s, 18*tile_s, 4*tile_s, 2*tile_s));

    ofPopMatrix();
    
    //doing some fixes
    //clip_boxes.push_back(ofRectangle(0,0, ofGetWidth(),766));
    
//    clip_boxes.push_back(ofRectangle(0,778, ofGetWidth(),ofGetHeight()-778));
//    clip_boxes.push_back(ofRectangle(408,0, 445-408,220));
//    clip_boxes.push_back(ofRectangle(0,270, ofGetWidth(),ofGetHeight()-270));
//    clip_boxes.push_back(ofRectangle(408+37,0, ofGetWidth()-(408+37),ofGetHeight()));

    
    for (int i=0; i<NUM_COLS; i++){
//        for (int k=0; k<clip_boxes.size(); k++){
//            gcode[i].clip_inside(clip_boxes[k]);
//        }
        gcode[i].sort();
        gcode[i].save("inktober_18_"+ofToString(i)+".nc");
    }
}

//--------------------------------------------------------------
void ofApp::draw_polar(ofRectangle border, int num_layers){
    draw_frame(border);
    
    float max_dist = border.width*0.51;
    ofVec2f center;
    center.x = border.x + border.width/2 ;
    center.y = border.y + border.height/2 ;
    
    int num_spokes = 6;
    float angle_step = TWO_PI/num_spokes;
    
    vector<ofVec2f> pnts[2];
    
    for (int d=0; d<num_layers; d++){
        for (int a=0; a<num_spokes; a++){
            float angle = a*angle_step;
            float dist_prc = ofRandom(0.2,num_layers==1 ? 0.95 : 0.6);
            if (d>0)    dist_prc = ofRandom(dist_prc,0.95);
            dist_prc = powf(dist_prc, 0.85);
            float dist = dist_prc * max_dist;
            
            ofVec2f pos;
            pos.x = center.x + cos(angle) * dist;
            pos.y = center.y + sin(angle) * dist;
            
            pnts[d].push_back(pos);
            
            //draw a hash line to end
            if (d==num_layers-1){
                ofVec2f end;
                end.x = center.x + cos(angle) * max_dist;
                end.y = center.y + sin(angle) * max_dist;

                float dist_to_end = ofDist(pos.x, pos.y, end.x, end.y);
                int num_segments = dist_to_end/5;
                float segment_length = dist_to_end / (float)num_segments;
                ofVec2f prev_pos;
                for (int k=0; k<num_segments; k++){
                    ofVec2f seg_pos = ofVec2f(pos);
                    seg_pos.x += cos(angle) * segment_length * k;
                    seg_pos.y += sin(angle) * segment_length * k;

                    if (k%2==1){
                        draw_thick_line(prev_pos, seg_pos);
                    }

                    prev_pos.set(seg_pos);
                }
            }
            
        }
        
        
        //connect 'em
        if (d==0){
            for (int i=0; i<pnts[d].size(); i++){
                draw_thick_line(pnts[d][i], pnts[d][(i+1)%pnts[d].size()]);
                draw_thick_line(pnts[d][i], center);
                
                //fill a triangle
                vector<ofVec2f> fill;
                fill.push_back(pnts[d][i]);
                fill.push_back(pnts[d][(i+1)%pnts[d].size()]);
                fill.push_back(center);
                fill_pattern(fill);
            }
        }
        else{
            for (int i=0; i<pnts[d].size(); i++){
                draw_thick_line(pnts[d][i], pnts[d][(i+1)%pnts[d].size()]);
                draw_thick_line(pnts[d][i], pnts[d-1][i]);
                
                //fill a trapazoid
                vector<ofVec2f> fill;
                fill.push_back(pnts[d][i]);
                fill.push_back(pnts[d][(i+1)%pnts[d].size()]);
                fill.push_back(pnts[d-1][(i+1)%pnts[d].size()]);
                fill.push_back(pnts[d-1][i]);
                fill_pattern(fill);
            }
        }
    }
    
}
//--------------------------------------------------------------
void ofApp::draw_dial(ofRectangle border){
    draw_frame(border);
    
    vector<ofVec2f> pnts;
    ofVec2f bottom_center = ofVec2f(border.x+border.width*0.5, border.y + border.height);
    
    //float angle_range = PI/4;
    
    float base_y = border.y + border.height * 0.7;
    float curve_offset = border.height * 0.4;
    
    int steps = 30;
    for (int c=0; c<=steps; c++){
        float prc = (float) c / (float) steps;
        
        float angle_padding_prc = 0.3;
        float angle = ofMap(prc, -angle_padding_prc,1+angle_padding_prc, 0,PI);
        float y_offset = sin(angle) * curve_offset;
        
        float x = border.x + border.width * prc;
        float y = base_y - y_offset;
        
        pnts.push_back(ofVec2f(x,y));
    }
    
    int hand_pos = ofRandom(steps*0.2, steps*0.8);
    
    //trace out the fill areas
    vector<ofVec2f> fill1;
    fill1.push_back(bottom_center);
    fill1.push_back(ofVec2f(border.x, border.y+border.height));
    //add poitns until we hit the hand
    for (int i=0; i<=hand_pos; i++){
        fill1.push_back(pnts[i]);
    }
    
    vector<ofVec2f> fill2;
    //add poitns until we hit the end
    for (int i=hand_pos; i<pnts.size(); i++){
        fill2.push_back(pnts[i]);
    }
    fill2.push_back(ofVec2f(border.x+border.width, border.y+border.height));
    fill2.push_back(bottom_center);
    
    
    //fill 'em!
    fill_pattern(fill1);
    fill_pattern(fill2);
    
    //add the bottoms
    pnts.push_back(ofVec2f(border.x+border.width, border.y+border.height));
    pnts.push_back(ofVec2f(border.x, border.y+border.height));
    
    //draw the whole shape
    for (int i=0; i<pnts.size(); i++){
        draw_thick_line(pnts[i], pnts[(i+1)%pnts.size()]);
    }
    
    //draw the hand
    
    draw_thick_line(pnts[hand_pos], bottom_center);
    
    float hand_angle = atan2(pnts[hand_pos].y-bottom_center.y, pnts[hand_pos].x-bottom_center.x);
    float hand_offset = 14;
    float hand_size = 5;
    for (int t=0; t<thick_line_layers; t++){
        gcode[stroke_col].circle(pnts[hand_pos].x+cos(hand_angle)*hand_offset, pnts[hand_pos].y+sin(hand_angle)*hand_offset, hand_size+t*thick_line_offset);
    }
    
}

//--------------------------------------------------------------
void ofApp::draw_bar(ofRectangle border){
    draw_frame(border);
    
    int num_bars = border.width / ofRandom(30,40);
    float spacing = ofRandom(4,6);
    float bar_width = (border.width-( (num_bars+1)*spacing) ) / num_bars;
    
    for (int c=0; c<num_bars; c++){
        float bar_h = ofRandom(border.height*0.2, border.height);
        
        vector<ofVec2f> pnts;
        
        float x = border.x+spacing + spacing*c + bar_width * c;
        float y = border.y+border.height;
        
        pnts.push_back(ofVec2f(x, y));
        pnts.push_back(ofVec2f(x, y - bar_h));
        pnts.push_back(ofVec2f(x+bar_width, y - bar_h));
        pnts.push_back(ofVec2f(x+bar_width, y));
        
        for (int i=0; i<pnts.size(); i++){
            draw_thick_line(pnts[i], pnts[(i+1)%pnts.size()]);
        }
        
        fill_pattern(pnts);
        
    }
    
}
    

//--------------------------------------------------------------
void ofApp::draw_pie(ofRectangle border, int num_rings){
    draw_frame(border);
    
    ofVec2f center;
    center.x = border.x + border.width/2;
    center.y = border.y + border.height/2;
    
    float max_dist = border.width * 0.48;
    
    for (int d=0; d<num_rings; d++){
        int num_ring_pnts = 30;
        float ring_step = TWO_PI/num_ring_pnts;
        
        float dist = (d+1) * (max_dist/num_rings);
        float prev_dist = d * (max_dist/num_rings);
        
        int num_sections = 3;
        int section_breaks[num_sections];
        for (int s=0; s<num_sections; s++){
            section_breaks[s] = 0;
        }
        
        int num_left = num_ring_pnts;
        while (num_left > 0){
            num_left--;
            section_breaks[ (int)ofRandom(num_sections)] ++;
        }
        
        int start = ofRandom(num_ring_pnts);
        for (int s=0; s<num_sections; s++){
            
            vector<ofVec2f> pnts;
            
            
            for (int i=start; i<=start+section_breaks[s]; i++){
                float angle = i*ring_step;
                ofVec2f pos;
                pos.x = center.x + cos(angle) * dist;
                pos.y = center.y + sin(angle) * dist;
                pnts.push_back(pos);
            }
            
            //add the center
            if (d==0){
                pnts.push_back(center);
            }
            else{
                for (int i=start+section_breaks[s]; i>=start; i--){
                    float angle = i*ring_step;
                    ofVec2f pos;
                    pos.x = center.x + cos(angle) * prev_dist;
                    pos.y = center.y + sin(angle) * prev_dist;
                    pnts.push_back(pos);
                }
            }
            
            //draw the line
            draw_thick_line(pnts[0], pnts[pnts.size()-1]);
            
            //draw the ring
            for (int t=0; t<thick_line_layers; t++){
                gcode[stroke_col].circle(center.x, center.y, dist+t*thick_line_offset);
            }
            
            
            //move to next section
            start += section_breaks[s];
            
            //fill it
            fill_pattern(pnts);
        }
    }
    
}

//--------------------------------------------------------------
void ofApp::draw_line(ofRectangle border, int num_lines){
    
    draw_frame(border);
    
    int spacing = 25;
    
    float min_x = border.x;
    float max_x = border.x+border.width;
    float min_y = border.y;
    float max_y = border.y+border.height;
    
    int num_pnts = border.width / spacing;
    
    vector<ofVec2f> pnts[4];
    for (int d=0; d<num_lines; d++){
        float noise_start = ofRandom(999);
        float noise_speed = ofRandom(0.5,0.1);//0.1;//1;//0.05;
        float nudge = 0.15 * num_lines;
        if (num_lines == 4) nudge = 0.3;
        for (int i=0; i<=num_pnts; i++){
            float prc = (float)i / (float)num_pnts;
            ofVec2f pos;
            pos.x = min_x + prc*border.width;
            float y_height = (ofNoise(i*noise_speed, noise_start)+nudge) * (border.height / num_lines);
            if (d==0){
                pos.y = max_y - y_height;
            }else{
                pos.y = pnts[d-1][i].y - y_height;
            }
            pos.y = CLAMP(pos.y, min_y, max_y);
            pnts[d].push_back(pos);
        }
        
        //draw that
        for (int i=0; i<pnts[d].size(); i++){
            for (int t=0; t<thick_line_layers; t++){
                if (i<pnts[d].size()-1){
                    ofPushMatrix();
                    ofTranslate(0, t*thick_line_offset);
                    gcode[stroke_col].line(pnts[d][i],pnts[d][i+1]);
                    ofPopMatrix();
                }
                gcode[stroke_col].circle(pnts[d][i].x, pnts[d][i].y, 2+t*thick_line_offset);
            }
        }
        
        //add connecting points to make it a circuit
        if (d==0){
            pnts[d].push_back(ofVec2f(border.x+border.width, border.y+border.height));
            pnts[d].push_back(ofVec2f(border.x, border.y+border.height));
        }else{
            for (int i=pnts[d].size(); i>=0; i--){
                pnts[d].push_back(pnts[d-1][i]);
            }
        }
        
        //fill
        fill_pattern(pnts[d]);
    }
}

//--------------------------------------------------------------
void ofApp::draw_frame(ofRectangle rect){
    
    //box and lines should be thick
    for (int d=0; d<thick_line_layers; d++){
        float thick = thick_line_offset * d;
        
    
        float padding = 10;
        float offset = 10;
        float spacing = 10;
        ofRectangle outter;
        outter.set(rect.x-padding, rect.y-padding, rect.width+padding*2, rect.height+padding*2);
        
        
        //bottom
        int num_lines = outter.width / spacing;
        for(int i=0; i<=num_lines; i++){
            float prc = (float)i / num_lines;
            float x1 = outter.x + prc * outter.width + thick;
            float x2 = outter.x + prc * outter.width + offset + thick;
            gcode[stroke_col].line(x1,outter.y+outter.height, x2, outter.y+outter.height+offset);
        }
        
        
        //side
        num_lines = outter.height / spacing;
        for(int i=0; i<num_lines; i++){
            float prc = (float)i / num_lines;
            float x1 = outter.x + outter.width;
            float y1 = outter.y + prc * outter.height;
            float x2 = outter.x + outter.width + offset;
            float y2 = outter.y + prc * outter.height + offset;
            gcode[stroke_col].line(x1,y1+thick, x2, y2+thick);
        }
        
        //boxes and border lines
        ofPushMatrix();
        ofTranslate(thick, thick);
        gcode[stroke_col].rect(outter.x, outter.y, outter.width, outter.height);
        gcode[stroke_col].line(outter.x+outter.width+offset, outter.y+offset, outter.x+outter.width+offset, outter.y+outter.height+offset);
        gcode[stroke_col].line(outter.x+offset, outter.y+outter.height+offset, outter.x+outter.width+offset, outter.y+outter.height+offset);
        ofPopMatrix();
    }
}


//--------------------------------------------------------------
void ofApp::draw_thick_line(ofVec2f base_a, ofVec2f base_b){
    //get the angle of the line
    float angle = atan2(base_a.y-base_b.y, base_a.x-base_b.x);
    float tan_angle = angle + PI/2;
    
    float dist_offset = thick_line_offset * thick_line_layers * 0.5;
    
    for (int t=0; t<thick_line_layers; t++){
        float dist = t * thick_line_offset - dist_offset;
        ofVec2f a = ofVec2f(base_a);
        ofVec2f b = ofVec2f(base_b);
        a.x += cos(tan_angle) * dist;
        a.y += sin(tan_angle) * dist;
        b.x += cos(tan_angle) * dist;
        b.y += sin(tan_angle) * dist;
        gcode[stroke_col].line(a,b);
    }
}

//--------------------------------------------------------------
void ofApp::fill_pattern(vector<ofVec2f> bounds){
    int pattern_id = get_next_pattern();
    int color = get_next_color();
    
    vector<GLine> lines;
    
    //get min and max positions
    ofVec2f min_pos = ofVec2f(ofGetWidth(), ofGetHeight());
    ofVec2f max_pos = ofVec2f(0,0);
    for (int i=0; i<bounds.size(); i++){
        if (bounds[i].x < min_pos.x)   min_pos.x = bounds[i].x;
        if (bounds[i].x > max_pos.x)   max_pos.x = bounds[i].x;
        if (bounds[i].y < min_pos.y)   min_pos.y = bounds[i].y;
        if (bounds[i].y > max_pos.y)   max_pos.y = bounds[i].y;
    }
    
    //fill it in!
    int row = 0;
    float spacing = 16;
    float y_spacing = 0;
    
    if (pattern_id == PATTERN_CIRC ){
        spacing = 8;
    }
    if (pattern_id == PATTERN_SQUARE){
        spacing = 10;
    }
    if (pattern_id == PATTERN_PLUS ){
        spacing = 8;
    }
    if (pattern_id == PATTERN_X){
        spacing = 8;
    }
    
    if (pattern_id == PATTERN_STRIPE_SLANT){    //removed
        spacing = 8;
    }
    if (pattern_id == PATTERN_STRIPE_HORZ){
        spacing = 4;
    }
    if (pattern_id == PATTERN_STEP){
        spacing = 6;
    }
    
    
    for (int y=0; y<ofGetWidth(); y+= spacing){
        for (int x=0; x<ofGetWidth(); x+= spacing){
            if (x>min_pos.x-spacing && x<max_pos.x+spacing && y>min_pos.y-spacing && y<max_pos.y+spacing){
                //circles / squares
                if (pattern_id == PATTERN_CIRC || pattern_id == PATTERN_SQUARE){
                    float size = 2.5;
                    int num_pnts = 10;
                    
                    if (pattern_id == PATTERN_SQUARE){
                        size = 8;
                        num_pnts = 4;
                    }
                    
                    
                    float angle_step = TWO_PI/num_pnts;
                    
                    ofVec2f prev_pos;
                    for (int i=0; i<=num_pnts; i++){
                        float angle = i * angle_step;
                        ofVec2f pos;
                        pos.x = x+cos(angle)*size;
                        pos.y = y+sin(angle)*size;
                        if (row%2==0 && pattern_id == PATTERN_CIRC)   pos.x += spacing/2;
                        if (i>0){
                            GLine line;
                            line.set(prev_pos, pos);
                            lines.push_back(line);
                        }
                        prev_pos.set(pos);
                    }
                }
                
                //X / PLUS
                if (pattern_id == PATTERN_PLUS){
                    float size = 3;
                    //if (pattern_id == PATTERN_X ) size = 4;
                    int num_pnts = 4;
                    float angle_step = TWO_PI/num_pnts;
                    for (int i=0; i<=num_pnts; i++){
                        float angle = i * angle_step + PI/4;
                        if (pattern_id == PATTERN_PLUS) angle += PI/4;
                        ofVec2f pos;
                        float center_x = x;
                        if (row%2==0)   center_x += spacing/2;
                        pos.x = center_x+cos(angle)*size;
                        pos.y = y+sin(angle)*size;
                        
                        GLine line;
                        line.set(center_x,y, pos.x, pos.y);
                        lines.push_back(line);
                    }
                }
                
                //X
                if (pattern_id == PATTERN_X){
                    GLine line1;
                    line1.a.set(x, y);
                    line1.b.set(x-spacing, y-spacing);
                    lines.push_back(line1);
                    
                    GLine line2;
                    line2.a.set(x-spacing, y);
                    line2.b.set(x, y-spacing);
                    lines.push_back(line2);
                }
                
                //STRIPES
                if (pattern_id == PATTERN_STRIPE_HORZ || pattern_id == PATTERN_STRIPE_SLANT){
                    GLine line;
                    line.a.set(x, y);
                    if (pattern_id == PATTERN_STRIPE_SLANT ){
                        line.b.set(x-spacing, y-spacing);
                    }
                    if (pattern_id == PATTERN_STRIPE_HORZ ){
                        line.b.set(x+spacing, y);
                    }
                    lines.push_back(line);
                }
                
                //STEP
                if (pattern_id == PATTERN_STEP){
                    float x_offset = row%2 * spacing/2;
                    ofVec2f a,b,c,d;
                    a.x = x + x_offset;
                    a.y = y;
                    
                    b.x = x + x_offset;
                    b.y = y - spacing/2;
                    
                    c.x = x + spacing/2 + x_offset;
                    c.y = y - spacing/2;
                    
                    d.x = x + spacing/2 + x_offset;
                    d.y = y - spacing;
                    
                    GLine line1,line2,line3;
                    line1.set(a,b);
                    line2.set(b,c);
                    line3.set(c,d);
                    lines.push_back(line1);
                    lines.push_back(line2);
                    lines.push_back(line3);
                }
            }
        }
        row++;
    }
    
    for (int i=0; i<lines.size(); i++){
        lines[i].clip_inside_polygon(bounds);
        gcode[color].line(lines[i]);
    }
    
    
    
}

//--------------------------------------------------------------
int ofApp::get_next_pattern(){
    if (available_patterns.size() == 0){
        for (int i=0; i<NUM_PATTERNS; i++){
            available_patterns.push_back(i);
        }
    }
    
    int rand = ofRandom(available_patterns.size());
    int val = available_patterns[rand];
    available_patterns.erase(available_patterns.begin()+rand);
    return val;
}

//--------------------------------------------------------------
int ofApp::get_next_color(){
    if (available_colors.size() == 0){
        for (int i=0; i<NUM_COLS; i++){
            available_colors.push_back(i);
        }
    }
    
    int rand = ofRandom(available_colors.size());
    int val = available_colors[rand];
    available_colors.erase(available_colors.begin()+rand);
    return val;
}

//--------------------------------------------------------------
void ofApp::update(){
    //cout<<"height "<<ofGetHeight()<<endl;
}

//--------------------------------------------------------------
void ofApp::draw(){
    
    for (int i=0; i<NUM_COLS; i++){
        gcode[i].draw();
    }
    
    ofSetColor(0);
    ofNoFill();
    for (int i=0; i<clip_boxes.size(); i++){
        ofDrawRectangle(clip_boxes[i]);
    }

}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){
    cout<<"mouse "<<x<<" , "<<y<<endl;
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
