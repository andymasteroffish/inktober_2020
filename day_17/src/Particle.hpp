

#ifndef Particle_hpp
#define Particle_hpp

#include "ofMain.h"

class Particle{
public:
    
    void setup(float startX, float startY);
    void update();
    
    void simpleAttract(float targetX, float targetY, float strength);
    void complexAttract(ofVec2f target, float strength, float maxRange);
    
    void draw();
    
    ofVec2f pos;
    ofVec2f vel;
    
    float fric;
    
    float grav;
    
};

#endif /* Particle_hpp */
