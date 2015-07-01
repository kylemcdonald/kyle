#include "Credits.h"

void Credits::setup() {
	credits.loadImage("images/source-note.png");
	credits.update();
}

void Credits::update() {
}

void Credits::draw() {
	ofEnableAlphaBlending();
	ofBackground(0, 0, 0);
	glColor4f(1, 1, 1, 1);
	credits.draw(0, 0);
}

void Credits::midi(ofxMidiEventArgs &args) {
}

void Credits::keyPressed(int key) {
}