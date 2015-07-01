#include "testApp.h"

void testApp::setup() {
	midiIn.openPort();
	ofAddListener(midiIn.newMessageEvent, this, &testApp::midi);
	
	cloudsApp.setup();
	scrubberApp.setup();
	colorApp.setup();
	creditsApp.setup();
	
	cur = &cloudsApp;
	
	ofHideCursor();
	
	midiDebug = true;
}

void testApp::update() {
	cur->update();
}

void testApp::draw() {	
	cur->draw();
	
	if(midiDebug) {
		glColor4f(1, 1, 1, .9);
		stringstream midiMessage;
		midiMessage << "port(" << lastMidi.port << ") " <<
			"channel(" << lastMidi.channel << ") " <<
			"status(" << lastMidi.status << ") " <<
			"byteOne(" << lastMidi.byteOne << ") " <<
			"byteTwo(" << lastMidi.byteTwo << ") " <<
			"timestamp(" << lastMidi.timestamp << ")";
		ofDrawBitmapString(midiMessage.str(), 10, ofGetHeight() - 20);
	}
}

void testApp::midi(ofxMidiEventArgs &args) {
	cur->midi(args);
	lastMidi = args;
}

void testApp::keyPressed(int key) {
	cur->keyPressed(key);
	switch(key) {
		case ',': ofHideCursor(); break;
		case '.': ofShowCursor(); break;
		case '/': midiDebug = !midiDebug; break;
		case '\\': ofToggleFullscreen(); break;
		case '1': setApp(&cloudsApp); break;
		case '2': setApp(&scrubberApp); break;
		case '3': setApp(&colorApp); break;
		case '0': setApp(&creditsApp); break;
	}
}

void testApp::setApp(ofMidiApp* app) {
	if(cur == &scrubberApp)
		scrubberApp.video.setPaused(true);
	
	if(app == &colorApp) {
		ofSetVerticalSync(true); // this should be handled in the class itself
	} else {
		ofSetVerticalSync(false);
	}
	
	cur = app;
}