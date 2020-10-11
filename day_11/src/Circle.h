//
//  Circle.h
//  inktober_11
//
//  Created by Andrew Wallace on 10/11/20.
//

#ifndef Circle_h
#define Circle_h

#include "ofMain.h"

class Circle{
public:
    
    float x,y;
    float size;
    
    vector<Circle *> children;
    
    Circle(float _x, float _y, float _size){
        x = _x;
        y = _y;
        size = _size;
        children.clear();
    }
    
    //assumes that x and y are inside this circle
    Circle * find_deepest_container(float x, float y){
        for (int i=0; i<children.size(); i++){
            if ( ofDistSquared(x,y, children[i]->x, children[i]->y) < powf(children[i]->size, 2)){
                return children[i]->find_deepest_container(x,y);
            }
        }
        
        return this;
        
    }
   
    bool inside(float other_x, float other_y){
        return ofDistSquared(x,y, other_x, other_y) < powf(size,2);
    }
    
    void draw(){
        ofDrawCircle(x,y, size);
        for (int i=0; i<children.size(); i++){
            children[i]->draw();
        }
    }
    
};


#endif /* Circle_h */
