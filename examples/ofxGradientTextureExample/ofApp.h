#pragma once

#include "ofMain.h"
#include "ofxTSAO.h"

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		ofxGradientTexture gt;
};
