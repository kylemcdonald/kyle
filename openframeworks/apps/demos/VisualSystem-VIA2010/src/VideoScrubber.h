#pragma once

#include "ofMidiApp.h"

class VideoScrubber : public ofMidiApp {
public:
	void setup();
	void update();
	void draw();
	void midi(ofxMidiEventArgs &args);
	void keyPressed(int key);
	void loadMovie(string filename);
	void togglePlay();
	
	ofVideoPlayer video;
	float nextTarget;
	bool newTarget;
	bool playStatus;
	bool newPlayStatus;
	
	bool newMovie;
	string movieFilename;
};
