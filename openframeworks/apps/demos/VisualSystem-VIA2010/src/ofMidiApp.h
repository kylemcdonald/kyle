#pragma once

#include "ofMain.h"
#include "ofxMidi.h"

class ofMidiApp : public ofBaseApp {
public:
	virtual void midi(ofxMidiEventArgs &args) = 0;
};