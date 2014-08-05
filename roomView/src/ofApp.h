#pragma once

#include "ofMain.h"
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

		ofxOscReceiver receiver;

		bool updatedHead;
		ofPoint head;
		ofVec3f room;

		ofxPanel gui;
		bool showGUI;

		ofTrueTypeFont font;

		void changeRoomX(float & roomX);
		void changeRoomY(float & roomY);
		void changeRoomZ(float & roomZ);


		ofParameter<float> roomX;
		ofParameter<float> roomY;
		ofParameter<float> roomZ;

		ofParameter<string> oscPort;
		ofParameter<string> oscAddress;


		ofEasyCam cam;
		ofVec3f viewRotation;


		void drawPlane(ofPoint p1, ofPoint p2, ofPoint p3, ofPoint p4);
};
