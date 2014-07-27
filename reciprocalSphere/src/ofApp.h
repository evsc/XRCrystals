#pragma once

#include "ofMain.h"
#include "ofxGui.h"
#include "scaData.h"
// #include "mtzData.h"

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);

		void resetSettings();
		void loadSettings();


		scaData sca;

		ofVec3f hklDim;			// maximum HKL grid points
		ofVec3f spaceDim;		// HKL coordinates times unit cell dimension
		ofPoint spaceO;			// origin of space on screen

		float scale;

		bool useEasyCam;
		bool drawAxis;

		ofEasyCam cam;

		ofCamera camera[1];
		int camToView;
		ofVec3f viewRotation;


		ofxPanel gui;

		ofParameter<float> zoom;
		ofParameter<bool> mirror;
		ofParameter<float> minIntensity;
		ofParameter<float> drawDots;
		ofParameter<string> dataFile;
		ofParameter<int> drawMaxH;
		ofParameter<int> drawMaxK;
		ofParameter<int> drawMaxL;
		ofParameter<int> sphereResolution;
		ofParameter<float> sphereBrightness;
		ofParameter<float> sphereAlpha;
		ofParameter<bool> sphereFill;


		void changeSphereResolution(int & sphereResolution);
		
};
