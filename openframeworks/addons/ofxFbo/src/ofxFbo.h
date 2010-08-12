#pragma once

#include "ofAppRunner.h"
#include "ofGraphics.h"

/*
	add multisampling support
	add all the checks for warnings and errors (just before begin())
	add OpenGLes support
*/

class ofxFbo {
public:
	ofxFbo();
	void setup(int width, int height, bool useDepth = true, bool useStencil = false);
	~ofxFbo();

	void setFov(float fov);
	void setBackground(float r, float g, float b, float a = 255);

	void attach(ofTexture& target, int position = 0);
	void detach(int position = 0);

	void begin();
	void end();

	void draw(float x, float y);
	void draw(float x, float y, float width, float height);

protected:
	int width, height;
	float fov;
	GLuint fboId, depthId, stencilId;
	ofTexture internalColor;
	vector<ofTexture*> attachments;

	int levels;
	void push();
	void pop();
	ofTexture* getAttachment();
	void checkAttachment();
	void setupScreenForFbo();
	void setupScreenForWindow();
};
