#pragma once

#include "ofMidiApp.h"

class PureColor : public ofMidiApp {
public:
	void setup();
	void update();
	void draw();
	void midi(ofxMidiEventArgs &args);
	void keyPressed(int key);
	
	ofColor bg;
	ofImage vignette;
	string message;
	int maxMessageSize;
	
	char nextChar;
	bool newChar;	
};
