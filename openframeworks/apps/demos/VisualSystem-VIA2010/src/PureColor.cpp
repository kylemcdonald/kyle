#include "PureColor.h"

void PureColor::setup() {
	vignette.loadImage("images/vignette.png");
	maxMessageSize = 6;
	bg.r = bg.g = bg.b = 0;
}

void PureColor::update() {
	if(newChar) {
		message += nextChar;
		if(message.size() > maxMessageSize)
			message = message.substr(1, message.size());
		newChar = false;
	}
}

void PureColor::draw() {
	ofSetColor(bg.r, bg.g, bg.b);
	vignette.draw(0, 0);
	ofSetColor(255, 255, 255);
	ofDrawBitmapString(message, ofGetWidth() / 2, ofGetHeight() / 2);
}

void PureColor::midi(ofxMidiEventArgs &args) {
	int x = args.byteTwo * 2;
	switch(args.byteOne) {
		case 1: case 17: bg.r = x; break;
		case 2: case 18: bg.g = x; break;
		case 3: case 19: bg.b = x; break;
		case 4: case 20: bg.r = x; bg.g = x; bg.b = x; break;
	}
}

void PureColor::keyPressed(int key) {
	if(key == '\t')
		message = "";
	if((key >= 'a' && key <= 'z') || key == ' ' || key == '.') {
		nextChar = key;
		newChar = true;
	}
}