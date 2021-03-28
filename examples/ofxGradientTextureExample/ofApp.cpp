#include "ofApp.h"

ofxGradientTexture gt;
//--------------------------------------------------------------
void ofApp::setup(){
	gt.setup(ofGetWidth(), ofGetHeight());

	gt.addColor(ofColor(0));
	gt.addColor(ofColor::fromHex(0xb01d1e));
	gt.addColor(ofColor::fromHex(0x006be4));
	gt.addColor(ofColor::fromHex(0xF16826));
	gt.addColor(ofColor::fromHex(0x7e63b4));
	gt.addColor(ofColor::fromHex(0xead35f));
	gt.addColor(ofColor::fromHex(0x00bbad));
	gt.addColor(ofColor(255));
	gt.addColor(ofColor(0));
}

//--------------------------------------------------------------
void ofApp::update() {

}

//--------------------------------------------------------------
void ofApp::draw() {
	//Now this texture can be passed to a shader for exampling
	gt.getTexture().draw(0, 0);
}
