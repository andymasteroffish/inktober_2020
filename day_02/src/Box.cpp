//
//  Box.cpp
//  inktober_02
//
//  Created by Andrew Wallace on 10/1/20.
//

#include "Box.hpp"

void Box::setup(float x1, float y1, float x2, float y2){
    top_left.x = x1;
    top_left.y = y1;
    
    bot_right.x = x2;
    bot_right.y = y2;
}

Box Box::split(){
    Box b;
    
    float prc = ofRandom(0.25, 0.75);
    
    float die_roll = ofRandomuf();
    
    float nudge = 0.02;
    if ((bot_right.x-top_left.x) > (bot_right.y-top_left.y)){
        die_roll += nudge;
    }else{
        die_roll -= nudge;
    }
    
    //vertical split
    if (die_roll > 0.5){
        float split_pos = top_left.x*prc + bot_right.x*(1.0-prc);
        //make a new box
        b.setup(split_pos, top_left.y, bot_right.x, bot_right.y);
        //resize my box
        bot_right.x = split_pos;
    }
    //horizontal split
    else{
        float split_pos = top_left.y*prc + bot_right.y*(1.0-prc);
        //make a new box
        b.setup(top_left.x, split_pos, bot_right.x, bot_right.y);
        //resize my box
        bot_right.y = split_pos;
    }
    
    return b;
    
}

float Box::get_size(){
    return (bot_right.x-top_left.x) * (bot_right.y-top_left.y);
}

void Box::draw(){
    ofDrawRectangle(top_left.x, top_left.y, bot_right.x-top_left.x, bot_right.y-top_left.y);
}
