//
//  Node.h
//  inktober_extra_28
//
//  Created by Andrew Wallace on 10/27/20.
//

#ifndef Node_h
#define Node_h

class Node{
public:
    float x,y;
    bool is_pos;
    
    Node(){
        setup(0,0,true);
    }
    Node(float _x, float _y, bool _is_pos){
        setup(_x,_y,_is_pos);
    }
    
    void setup(float _x, float _y, bool _is_pos){
        x=_x;
        y=_y;
        is_pos = _is_pos;
    }
};

#endif /* Node_h */
