

#include "Particle.hpp"


void Particle::setup(float startX, float startY){
    pos.set(startX, startY);
    
    vel.set(0,0);   //start neutral
    
    fric = 0.94;    //higher number = less friction
    grav = 0.00;    //no gravity
}

void Particle::update(){
    //move
    pos += vel;
    
    //gravity
    vel.y += grav;
    
    //friction
    vel *= fric;
}

void Particle::simpleAttract(float targetX, float targetY, float strength){
    //convetr the target position to a Vec2f so we can do easy vector math with it
    ofVec2f target;
    target.set(targetX, targetY);
    
    //the delta is the distance between where we are and the target
    ofVec2f delta = target - pos;
    
    //scale that based on the strength
    delta *= strength;
    
    //apply it to the velocity
    vel += delta;
}

void Particle::complexAttract(ofVec2f target, float strength, float maxRange){
    //get the distance to the target
    float dist = ofDist(pos.x, pos.y, target.x, target.y);
    if (dist > maxRange){
        //do nothing
        return;
    }
    
    //if we're close enough to have an effect, we want to figure out the delta
    //ofVec2f target;
    //target.set(targetX, targetY);
    ofVec2f delta = target - pos;
    
    //this time, we just want it as a direction, so we normalize. This brings the Vec2f to a length of 1 in whatever direction it was pointing
    delta.normalize();
    
    //get the percentage of the way our target is between maxRange and 0
    float prc = dist / maxRange;
    //reverse it because we should get most pull when distance is close, and lowest pull when distance is far
    prc = 1.0 - prc;
    
    //apply strength in the delta direction according to the percentage
    vel += delta * prc * strength;
}

void Particle::draw(){
    ofDrawCircle(pos.x, pos.y, 2);
}
