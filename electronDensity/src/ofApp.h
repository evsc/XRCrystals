#pragma once

#include "ofMain.h"
#include "na4Data.h"
// #include "edContour.h"
// #include "ListContour.h"
#include "GLContour.h"
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
		ofVec3f uc;		// unit cell dimension, inverse



		
		
	    // edContour contour;
	    CGLContour contourCol;
	    CGLContour contourSection;
	    // CListContour contour;

	    static double getElectronDensityCol(ofApp * mother, double x, double y);
	    static double getElectronDensitySection(ofApp * mother, double x, double y);


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

		ofParameter<bool> draw3D;

		ofParameter<float> zoom;
		ofParameter<float> nodeScale;
		ofParameter<float> filter;

		ofParameter<float> drawSection;
		ofParameter<int> drawRow;
		ofParameter<float> drawCol;

		ofParameter<int> contourSize;
		ofParameter<int> isoPlanes;
		ofParameter<float> isoPlaneDist;
		ofParameter<int> gridSize;
		ofParameter<bool> interpolateGrid;

		ofParameter<float> drawAlpha;
		ofParameter<float> drawBrightness;

		ofParameter<string> oscPort;
		ofParameter<string> oscAddress;


		void changeContourSize(int & contourSize);
		void changeGridSize(int & gridSize);
		void changeIsoPlanes(int & isoPlanes);
		void changeIsoPlaneDist(float & isoPlaneDist);


};



double contourFunction(double x, double y);