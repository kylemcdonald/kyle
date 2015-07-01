#include "VideoScrubber.h"

void VideoScrubber::setup() {
	loadMovie("pppd.mov");
	//video.play();
	newTarget = false;
	newPlayStatus = false;
	playStatus = false;
}

void VideoScrubber::update() {	
	if(newMovie) {
		video.loadMovie(movieFilename);
		if(playStatus)
			video.play();
		newMovie = false;
	}
	
	if(newTarget) {
		video.setPosition(nextTarget);
		newTarget = false;
	}
	
	if(newPlayStatus) {
		if(playStatus)
			video.play();
		else
			video.setPaused(true);
		newPlayStatus = false;
	}
}

void VideoScrubber::draw() {
	//ofBackground(0, 0, 0);
	glColor4f(1, 1, 1, 1);
	video.setAnchorPercent(.5, .5);
	float aspect = video.getWidth() / video.getHeight();
	float width = ofGetWidth();
	float height = width / aspect;
	video.draw(ofGetWidth() / 2, ofGetHeight() / 2, width, height);
}

void VideoScrubber::midi(ofxMidiEventArgs &args) {
	int y = args.byteOne;
	float x = args.byteTwo / 127.;
	
	bool tap = args.status == 144 && args.byteTwo != 0;
	switch(y) {
		case 17:
			nextTarget = x;
			newTarget = true;
			break;
		case 1:
			if(tap)
				togglePlay();
			break;
	}
	if(tap && y >= 2 && y <= 16) {
		nextTarget = ofMap(y, 2, 16, 0, 1);
		newTarget = true;
	}
}

void VideoScrubber::togglePlay() {
	playStatus = !playStatus;
	newPlayStatus = true;
}

void VideoScrubber::keyPressed(int key) {
	switch(key) {
		case ' ': togglePlay(); break;
		case 'q': loadMovie("GLITCHFEST.mov"); break;
		case 'w': loadMovie("pppd.mov"); break;
		case 'e': loadMovie("antigray.mov"); break;
		case 'r': loadMovie("candide.mov"); break;
		case 't': loadMovie("pollen.mov"); break;
		case 'a': loadMovie("toki1.mov"); break;
		case 's': loadMovie("toki2.mov"); break;
		case 'd': loadMovie("toki3.mov"); break;
		case 'f': loadMovie("toki4.mov"); break;
		case 'g': loadMovie("toki5.mov"); break;
	}
}

void VideoScrubber::loadMovie(string filename) {
	newMovie = true;
	movieFilename = "movies/" + filename;
}