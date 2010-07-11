#pragma once

#include "ofMain.h"
#include <fstream>

class ofxShader {
public:
	ofxShader();
	~ofxShader();

	void setup(string shaderName);
	void setup(string vertexName, string fragmentName);
	void setupInline(string vertexShaderSource, string fragmentShaderSource);
	void unload();
	void logError();

	void begin();
	void end();

	// set a texture reference
	void setTexture(const char* name, ofImage& img, int textureLocation);

	// set a single uniform vector
	void setUniform(const char* name, int v1);
	void setUniform(const char* name, int v1, int v2);
	void setUniform(const char* name, int v1, int v2, int v3);
	void setUniform(const char* name, int v1, int v2, int v3, int v4);

	void setUniform(const char* name, float v1);
	void setUniform(const char* name, float v1, float v2);
	void setUniform(const char* name, float v1, float v2, float v3);
	void setUniform(const char* name, float v1, float v2, float v3, float v4);

	// set an array of uniform vectors
	void setUniform1v(const char* name, int* v, int count = 1);
	void setUniform2v(const char* name, int* v, int count = 1);
	void setUniform3v(const char* name, int* v, int count = 1);
	void setUniform4v(const char* name, int* v, int count = 1);

	void setUniform1v(const char* name, float* v, int count = 1);
	void setUniform2v(const char* name, float* v, int count = 1);
	void setUniform3v(const char* name, float* v, int count = 1);
	void setUniform4v(const char* name, float* v, int count = 1);

	// set attributes that vary per vertex
	void setAttribute(const char* name, short v1);
	void setAttribute(const char* name, short v1, short v2);
	void setAttribute(const char* name, short v1, short v2, short v3);
	void setAttribute(const char* name, short v1, short v2, short v3, short v4);

	void setAttribute(const char* name, float v1);
	void setAttribute(const char* name, float v1, float v2);
	void setAttribute(const char* name, float v1, float v2, float v3);
	void setAttribute(const char* name, float v1, float v2, float v3, float v4);

	void setAttribute(const char* name, double v1);
	void setAttribute(const char* name, double v1, double v2);
	void setAttribute(const char* name, double v1, double v2, double v3);
	void setAttribute(const char* name, double v1, double v2, double v3, double v4);

	void setAttributeLocation(const char* name, int index);
	int getAttributeLocation(const char* name);

	void printActiveUniforms();
	void printActiveAttributes();

	GLhandleARB vertexShader;
	GLhandleARB fragmentShader;
	GLhandleARB shader;

protected:
	string loadTextFile(string filename);
	GLint getLoc(const char* name);

	bool bLoaded;
};
