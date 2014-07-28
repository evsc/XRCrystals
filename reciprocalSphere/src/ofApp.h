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




		/* **************** DATA **************** */
		scaData sca;			// object to load scalepack data

		ofVec3f hklDim;			// maximum HKL grid points
		ofVec3f spaceDim;		// HKL coordinates times unit cell dimension
		ofPoint spaceO;			// origin of space on screen




		/* ************ drawing related ************ */
		ofMesh foo;
		ofSpherePrimitive sphere;
		ofVbo vboSphere;
		ofVboMesh vboSphereMesh;

		void changeSphereResolution(int & sphereResolution);

		int visibleNodes;


		int drawMode;	// 0 ... ofDrawSphere
						// 1 ... ofVbo
						// 2 ... ofVboMesh



		/* **************** CAMERA **************** */
		bool useEasyCam;
		bool drawAxis;
		ofEasyCam cam;
		ofCamera camera[1];
		int camToView;


		ofVec3f viewRotation;
		float scale;



		/* **************** GUI **************** */
		ofxPanel gui;
		bool showGUI;

		ofParameter<float> zoom;
		ofParameter<bool> mirror;
		ofParameter<float> minIntensity;
		ofParameter<float> nodeScale;
		ofParameter<string> dataFile;
		ofParameter<int> drawMaxH;
		ofParameter<int> drawMaxK;
		ofParameter<int> drawMaxL;
		ofParameter<int> sphereResolution;
		ofParameter<float> sphereBrightness;
		ofParameter<float> sphereAlpha;
		ofParameter<bool> sphereFill;



		
};
