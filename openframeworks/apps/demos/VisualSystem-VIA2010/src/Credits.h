#pragma once

#include "ofMidiApp.h"

class Credits : public ofMidiApp {
public:
	void setup();
	void update();
	void draw();
	void midi(ofxMidiEventArgs &args);
	void keyPressed(int key);
	
	ofImage credits;
};
