#pragma once

#include "ofMain.h"

#include "Particle.hpp"
#include "ofxGCode.hpp"
#include "Shape.h"

class ofApp : public ofBaseApp{

	public:
		void setup();
    void tick();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
    
    #define NUM_LAYERS 3
    vector<Particle> particles[NUM_LAYERS];
    vector<Shape> shapes[NUM_LAYERS];
    
    int tick_count;
    
    ofxGCode gcode[4];
		
};
