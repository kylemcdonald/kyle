#include "CloudsAreLooming.h"

void CloudsAreLooming::setup() {
	lastTime = 0;
	
	dofShader.setup("DOFCloud");
	
	resetCloud();

	chroma.setup(ofGetWidth(), ofGetHeight(), false);
	tex.allocate(ofGetWidth(), ofGetHeight(), GL_RGBA16F_ARB);
	chroma.attach(tex);
	
	ofBackground(0, 0, 0);
}

void CloudsAreLooming::resetCloud() {
	pointBrightness = 2;
	aberration = .02;
	aperture = .04;
	cameraDistance = 1600;
	focusDistance = 1200;
	turbulence = 1;
	fov = 60;
	debugLevel = 0;
	
	minimumFollowDistance = 200;
	smoothedAverage.set(0, 0, 0);
	cameraSmoothing = .01;
	cameraTargetSmoothing = .01;
	cameraPosition.set(0, 0, 0);
	
	cameraMode = false;
	
	Particle::setup();
	
	Particle::particles.clear();
	int n = 20000;
	float radius = 2000;
	for(int i = 0; i < n; i++)
		Particle::particles.push_back(Particle(radius));
}

void CloudsAreLooming::update() {
	float curTime = ofGetElapsedTimef();
	float diffTime = curTime - lastTime;
	lastTime = curTime;
	
	Particle::updateAll(turbulence * diffTime);
	rotationPosition += rotationVelocity * diffTime;
}

void CloudsAreLooming::draw() {
	//ofBackground(0, 0, 0);
	
	chroma.begin();
	chroma.setBackground(0, 0, 0, 0);
	chroma.setFov(fov);

	ofPushMatrix();

	ofTranslate(ofGetWidth() / 2, ofGetHeight() / 2, 0);

	glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_GREATER, 0);

	// super helpful: http://pmviewer.sourceforge.net/method.php
	glEnable(GL_POINT_SMOOTH);
	glEnable(GL_VERTEX_PROGRAM_POINT_SIZE_ARB);
	
	ofxVec3f& avg = Particle::avg;
	if(smoothedAverage.distance(avg) > minimumFollowDistance)
		smoothedAverage.interpolate(avg, cameraTargetSmoothing);
	ofxVec3f cameraTarget = smoothedAverage - avg;
	cameraTarget.normalize();
	cameraTarget *= -cameraDistance;
	cameraTarget += avg;
	cameraPosition.interpolate(cameraTarget, cameraSmoothing);
	
	if(cameraMode) {
		gluLookAt(0, 0, cameraDistance,
							avg.x, avg.y, avg.z,
							0, 1, 0);
	} else {
		gluLookAt(cameraPosition.x, cameraPosition.y, cameraPosition.z,
							avg.x, avg.y, avg.z,
							0, 1, 0);
	}
	
	glRotatef(rotationPosition.x, 0, 1, 0);
	glRotatef(rotationPosition.y, 1, 0, 0);
	
	if(debugLevel > 1) {
		glColor4f(1, 0, 0, .4);
		glBegin(GL_LINES);
		glVertex3f(avg.x, avg.y, avg.z);
		glVertex3f(cameraTarget.x, cameraTarget.y, cameraTarget.z);	
		glEnd();
		
		glPushMatrix();
		glTranslatef(smoothedAverage.x, smoothedAverage.y, smoothedAverage.z);
		glutWireCube(10);
		glPopMatrix();
	}
	
	if(debugLevel > 2) {
		glPushMatrix();
		glBegin(GL_LINES);
		glVertex3f(cameraTarget.x, cameraTarget.y, cameraTarget.z);	
		glVertex3f(cameraPosition.x, cameraPosition.y, cameraPosition.z);
		glEnd();
		glTranslatef(cameraPosition.x, cameraPosition.y, cameraPosition.z);
		glScalef(100, 100, 100);
		glutWireTetrahedron();
		glPopMatrix();
	}
	
	if(debugLevel > 3) {
		glColor4f(0, 1, 0, 1);
		glPushMatrix();
		glScalef(100, 100, 100);
		glutWireTetrahedron();
		glPopMatrix();
	}
	
	if(debugLevel > 9) {
		glColor4f(1, .5, 0, 1);
		glutWireSphere(2000, 20, 10);
	}
	
	if(debugLevel > 4) {
		glPushMatrix();
		glColor4f(1, 1, 1, 1);
		glTranslatef(avg.x, avg.y, avg.z);
		if(debugLevel > 15) {
			glPushMatrix();
			glScalef(1, -1, 1);
			glutWireTeapot(400);
			glPopMatrix();
		} else
			glutWireCube(400);
		glPopMatrix();
	}
	
	if(debugLevel > 6) {
		glColor4f(1, 0, 1, .5);
		glBegin(GL_LINES);
		for(int i = 0; i < 16; i++) {
			Particle& cur = Particle::particles[i];
			ofxVec3f& pos = cur.position;
			glVertex3f(avg.x, avg.y, avg.z);
			glVertex3f(pos.x, pos.y, pos.z);
		}
		glEnd();
	}
	
	if(debugLevel > 7) {
		glColor4f(0, 1, 1, .5);
		glBegin(GL_LINES);
		for(int i = 0; i < 512; i++) {
			Particle& cur = Particle::particles[i];
			ofxVec3f& pos = cur.position;
			glVertex3f(pos.x, pos.y, pos.z);
		}
		glEnd();
	}
	
	if(debugLevel > 8) {
		glColor4f(1, 1, 0, 1);
		glBegin(GL_LINES);
		for(int i = 0; i < 5000; i++) {
			Particle& cur = Particle::particles[i];
			ofxVec3f& pos = cur.position;
			ofxVec3f vel = cur.velocity * 60;
			glVertex3f(pos.x, pos.y, pos.z);
			glVertex3f(pos.x + vel.x, pos.y + vel.y, pos.z + vel.z);
		}
		glEnd();
	}
	
	dofShader.begin();
	dofShader.setUniform("focusDistance", focusDistance);
	if(debugLevel > 5)
		dofShader.setUniform("aperture", 0.f);
	else
		dofShader.setUniform("aperture", aperture);
	
	glColor4f(1, 1, 1, pointBrightness);
	Particle::drawAll();

	dofShader.end();

	ofPopMatrix();
	
	chroma.end();
	chroma.clearAlpha();
	
	drawWithAberration();
	
	if(debugLevel > 0) {
		ofSetColor(255, 255, 255, 255);
		ofDrawBitmapString(ofToString((int) ofGetFrameRate()), 10, 20);
	}
}

void CloudsAreLooming::drawWithAberration() {
	float scaleFactor;
	
	// red
	glColor3f(1. / 2, 0, 0);
	glPushMatrix();
	scaleFactor = 1 - aberration;
	glTranslatef(chroma.getWidth() / 2, chroma.getHeight() / 2, 0);
	glScalef(scaleFactor, scaleFactor, 1);
	chroma.draw(-chroma.getWidth() / 2, -chroma.getHeight() / 2);
	glPopMatrix();
	
	// yellow
	glColor3f(1. / 2, 1. / 3, 0);
	glPushMatrix();
	scaleFactor = 1 - aberration / 2;
	glTranslatef(chroma.getWidth() / 2, chroma.getHeight() / 2, 0);
	glScalef(scaleFactor, scaleFactor, 1);
	chroma.draw(-chroma.getWidth() / 2, -chroma.getHeight() / 2);
	glPopMatrix();
	
	// green
	glColor3f(0, 1. / 3, 0);
	chroma.draw(0, 0);
	
	// cyan
	glColor3f(0, 1. / 3, 1. / 2);
	glPushMatrix();
	scaleFactor = 1 + aberration / 2;
	glTranslatef(chroma.getWidth() / 2, chroma.getHeight() / 2, 0);
	glScalef(scaleFactor, scaleFactor, 1);
	chroma.draw(-chroma.getWidth() / 2, -chroma.getHeight() / 2);
	glPopMatrix();
	
	// blue
	glColor3f(0, 0, 1. / 2);
	glPushMatrix();
	scaleFactor = 1 + aberration;
	glTranslatef(chroma.getWidth() / 2, chroma.getHeight() / 2, 0);
	glScalef(scaleFactor, scaleFactor, 1);
	chroma.draw(-chroma.getWidth() / 2, -chroma.getHeight() / 2);
	glPopMatrix();
}

void CloudsAreLooming::midi(ofxMidiEventArgs &args) {
	float x = args.byteTwo / 127.;
	bool tap = args.status == 144 && args.byteTwo != 0;
	switch(args.byteOne) {
		// row 1
		case 1: case 21: Particle::speed = ofLerp(0, 200, x); break;
		case 2: case 22: Particle::spread = ofLerp(10, 200, x); break;
		case 3: case 23: Particle::viscosity = ofLerp(0, 1, x); break;
		case 4: case 24: Particle::independence = ofLerp(0, 1, x); break;
			
		// row 2
		case 5: case 25: Particle::neighborhood = ofLerp(1, 1440, x); break;
		case 6: case 26: focusDistance = ofLerp(100, 3200, x); break;
		case 7: case 27: aperture = ofLerp(0, .05, x); break;
		case 8: case 28: aberration = ofLerp(0, .1, x); break;
			
		// sliders
		case 17: rotationVelocity.x = ofLerp(-100, 100, x); break;
		case 18: rotationVelocity.y = ofLerp(-100, 100, x); break;
			
		// row 3
		case 9: case 19: turbulence = ofLerp(0, 1000, x); break;
		case 10: case 20: pointBrightness = ofLerp(0, 6, x); break;
		case 11: fov = ofLerp(60, 180, x); break;
		case 12: cameraDistance = ofLerp(1600, 1, x); break;
		
		// row 4
		case 13: debugLevel = ofLerp(0, 16, x); break;
		case 14: if(tap) cameraMode = !cameraMode; break;
		case 15: fov = ofLerp(60, 0, x); break;
		case 16: cameraDistance = ofLerp(1600, 3200, x); break;
	}
}

void CloudsAreLooming::keyPressed(int key) {
	switch(key) {
		case ' ': resetCloud(); break;
		case 'q': zeroGoal(); break;
		case 'w': sphereGoal(6, 3); break;
		case 'e': sphereGoal(10, 5); break;
		case 'r': sphereGoal(30, 15); break;
		case 't': sphereGoal(300, 150); break;
	}
}

void CloudsAreLooming::zeroGoal() {
	vector<Particle>& all = Particle::particles;
	for(int i = 0; i < all.size(); i++)
		all[i].goal.set(0, 0, 0);
}

void CloudsAreLooming::sphereGoal(int slices, int stacks) {
	float radius = 900;
	vector<Particle>& all = Particle::particles;
	for(int i = 0; i < all.size(); i++) {
		int theta = (int) ofRandom(0, slices);
		int phi = (int) ofRandom(0, stacks);
		
		float atheta = (theta * TWO_PI / slices) - PI;
		float aphi = (phi * PI / stacks);
		ofxVec3f start(cos(atheta) * sin(aphi), sin(atheta) * sin(aphi), cos(aphi));
		start *= radius;
		
		int lr = 0;
		int ud = 0;
		switch((int) ofRandom(0, 2)) {
			case 0: lr = +1; break;
			case 1: ud = +1; break;
		}
		float btheta = ((theta + lr) * TWO_PI / slices) - PI;
		float bphi = ((phi + ud) * PI / stacks);
		ofxVec3f end(cos(btheta) * sin(bphi), sin(btheta) * sin(bphi), cos(bphi));
		end *= radius;
		
		float alongLine = (asinf(ofRandom(-1, 1)) + HALF_PI) / PI;
		all[i].goal.set(start.interpolate(end, alongLine));
	}
}
