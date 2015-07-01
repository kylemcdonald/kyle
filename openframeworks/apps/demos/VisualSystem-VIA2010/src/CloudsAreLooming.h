#pragma once

#include "ofMidiApp.h"
#include "ofxShader.h"
#include "Particle.h"
#include "ofxFbo.h"
#include "ofxVec2f.h"

// walk through "development"

// audio disturbance of system? point brightness, focus distance
// smooth any parameters out?

class CloudsAreLooming : public ofMidiApp {
public:
	void setup();
	void update();
	void draw();
	void midi(ofxMidiEventArgs &args);
	void keyPressed(int key);
	
	void resetCloud();	
	void drawWithAberration();
	
	void zeroGoal();
	void sphereGoal(int slices, int stacks);

	ofxShader dofShader;
	ofxFbo chroma;
	ofTexture tex;
	
	float lastTime;
	
	float pointBrightness, aberration, aperture, cameraDistance, focusDistance, turbulence;
	float fov;
	
	ofxVec2f rotationVelocity, rotationPosition;
	
	ofxVec3f smoothedAverage, cameraPosition;
	float minimumFollowDistance;
	float cameraSmoothing, cameraTargetSmoothing;
	
	int debugLevel;
	bool cameraMode;
};
