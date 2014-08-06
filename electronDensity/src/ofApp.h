#pragma once

#include "ofMain.h"
#include "na4Data.h"
#include "ofxGui.h"
#include "ofxOsc.h"

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


		void createGUI();
		void resetSettings();
		void loadSettings();




		/* **************** DATA **************** */
		na4Data data;




		/* **************** OSC **************** */
		ofxOscReceiver receiver;

		bool updatedHead;
		ofPoint head;


		/* **************** CAMERA **************** */
		ofEasyCam cam;

		ofVec3f viewRotation;

		




		/* **************** GUI **************** */
		ofxPanel gui;
		bool showGUI;

		ofParameter<float> zoom;
		ofParameter<float> nodeScale;
		ofParameter<float> filter;

		ofParameter<int> drawSection;
		ofParameter<int> drawRow;
		ofParameter<int> drawCol;

		ofParameter<float> drawAlpha;
		ofParameter<float> drawBrightness;

		ofParameter<string> oscPort;
		ofParameter<string> oscAddress;


};
