#pragma once

#include "ofMidiApp.h"

#include "CloudsAreLooming.h"
#include "VideoScrubber.h"
#include "PureColor.h"
#include "Credits.h"

// glitch image w rle, sorting, etc. glitch points
// --> "for source code visit kyle.googlecode.com"

// almost ok, need to add the ability to use the buttons for triggering moments
// and (probably) save those values
// more importantly, need to export content from processing for use

class testApp : public ofMidiApp {
public:
	void setup();
	void update();
	void draw();
	void midi(ofxMidiEventArgs &args);
	void keyPressed(int key);
	void setApp(ofMidiApp* app);
	
	CloudsAreLooming cloudsApp;
	PureColor colorApp;
	VideoScrubber scrubberApp;
	Credits creditsApp;
	
	ofMidiApp* cur;
	
	ofxMidiIn midiIn;
	ofxMidiEventArgs lastMidi;
	bool midiDebug;
};