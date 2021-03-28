#pragma once

#include "ofMain.h"
#include "ofxGradientTexture.h"


namespace TSAO {
	//COLOR
	inline float cosColor(float t, float a, float b, float c, float d) {
		return (a + b * cos(6.28318*(c*t + d))) * 255.;
	}

	inline ofColor cosColor(float t, glm::vec3 a, glm::vec3 b, glm::vec3 c, glm::vec3 d) {
		float red = (a.x + b.x * cos(6.28318*(c.x*t + d.x))) * 255.;
		float green = (a.y + b.y * cos(6.28318*(c.y*t + d.y))) * 255.;
		float blue = (a.z + b.z * cos(6.28318*(c.z*t + d.z))) * 255.;
		return ofColor(red, green, blue);
	}

	//VECTORS
	inline ofVec2f polar2vec(float theta, float radius = 1.) {
		return ofVec2f(cos(theta)*radius, sin(theta)*radius);
	}

	//LOGGING
	inline void printFrameRate(int period) {
		if (ofGetFrameNum() % period == 0) {
			cout << ofGetFrameRate() << endl;
		}
	}

	inline void printFrameRate() {
		cout << ofGetFrameRate() << endl;
	}
	inline void drawFrameRate(int x = 20, int y= 20) {
		ofDrawBitmapString(ofToString(ofGetFrameRate()), x, y);
	}

	//CONSTANTS
	const float TAU = 6.283185307179586;

	//SHADERS
	inline void setDefaultVertexShader(ofShader & shader) {
		shader.setupShaderFromSource(GL_VERTEX_SHADER, R"(
			#version 460
			uniform mat4 modelViewProjectionMatrix;
			in vec4 position;
			in vec4 normal;
			in vec2 texcoord;

			out vec2 varyingtexcoord;
			void main() {
				gl_Position = modelViewProjectionMatrix * position;
				varyingtexcoord = texcoord;
			})");
	}

	inline string getSobelShaderString() {
		string shader = R"(
			#version 460

			uniform  sampler2DRect src;
			uniform int mode;
			// 0 hue, 1 sat, 2 val
			vec3 rgb2hsv(vec3 c) {
				vec4 K = vec4(0.0, -1.0 / 3.0, 2.0 / 3.0, -1.0);
				vec4 p = mix(vec4(c.bg, K.wz), vec4(c.gb, K.xy), step(c.b, c.g));
				vec4 q = mix(vec4(p.xyw, c.r), vec4(c.r, p.yzx), step(p.x, c.r));
				float d = q.x - min(q.w, q.y);
				float e = 1.0e-10;
				return vec3(abs(q.z + (q.w - q.y) / (6.0 * d + e)), d / (q.x + e), q.x);
			}
			vec3 hsv2rgb(vec3 c) {
				vec4 K = vec4(1.0, 2.0 / 3.0, 1.0 / 3.0, 3.0);
				vec3 p = abs(fract(c.xxx + K.xyz) * 6.0 - K.www);
				return c.z * mix(K.xxx, clamp(p - K.xxx, 0.0, 1.0), c.y);
			}
			void main(){

			vec2 uv	= gl_FragCoord.xy;
			vec2 offset[9];
			offset[0] = vec2(-1.,-1.);
			offset[1] = vec2(0.,-1.);
			offset[2] = vec2(1.,-1.);
			offset[3] = vec2(-1.,0.);
			offset[4] = vec2(0.,0.);
			offset[5] = vec2(1.,0.);
			offset[6] = vec2(-1.,1.);
			offset[7] = vec2(0.,1.);
			offset[8] = vec2(1.,1.);

			int xKernel[9];
			xKernel [0] = 1;
			xKernel [1] = 0;
			xKernel [2] = -1;
			xKernel [3] = 2;
			xKernel [4] = 0;
			xKernel [5] = -2;
			xKernel [6] = 1;
			xKernel [7] = 0;
			xKernel [8] = -1;
			int yKernel[9];
			yKernel[0] = 1;
			yKernel[1] = 2;
			yKernel[2] = 1;
			yKernel[3] = 0;
			yKernel[4] = 0;
			yKernel[5] = 0;
			yKernel[6] = -1;
			yKernel[7] = -2;
			yKernel[8] = -1;

			float gx = 0;
			float gy = 0;
			float disp = 20;
			for(int i = 0; i<9; i++){
				//GET GRADIENT OF SATURATION
				if(mode == 0){
					gx += rgb2hsv(texture(src,uv+offset[i]*disp).rgb).r * xKernel[i];
					gy += rgb2hsv(texture(src,uv+offset[i]*disp).rgb).r * yKernel[i];
				} else if (mode == 1){
					gx += rgb2hsv(texture(src,uv+offset[i]*disp).rgb).g * xKernel[i];
					gy += rgb2hsv(texture(src,uv+offset[i]*disp).rgb).g * yKernel[i];
				} else if (mode == 2){
					gx += rgb2hsv(texture(src,uv+offset[i]*disp).rgb).b * xKernel[i];
					gy += rgb2hsv(texture(src,uv+offset[i]*disp).rgb).b * yKernel[i];
				}
			}
			float g = sqrt(gx*gx + gy*gy);

			gl_FragColor = vec4(gx,gy,g,1.);
		})";
		return shader;
	}
}

