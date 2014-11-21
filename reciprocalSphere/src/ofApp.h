#pragma once

#include "ofMain.h"
#include "ofxGui.h"
#include "scaData.h"
// #include "mtzData.h"
#include "ofxOsc.h"
#include <map>

typedef struct {

    int h;
    int k;
    int l;
    int mirrorno;
    float intensity;
    float longitude;
    float latitude;
    float phase;

    bool updated;   // if not, that means the dot disappeared, the sound should stop
    bool fresh;     // dot just appeared, the sound should be instantiated

} activeDot;


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


		ofVec3f ewaldO;			// origin of Ewald sphere
		float ewaldSphereRadius;
		bool onEwaldSphere(int h, int k, int l, float mirrorRotZ);

		/* ************ drawing related ************ */
		ofMesh foo;
		ofSpherePrimitive sphere;
		ofVbo vboSphere;
		ofVboMesh vboSphereMesh;

		void changeSphereResolution(int & sphereResolution);
		void changeWaveLength(float & waveLength);

		int visibleNodes;


		int drawMode;	// 0 ... ofDrawSphere
						// 1 ... ofVbo
						// 2 ... ofVboMesh

		ofVec3f rotateHKL;

        /* **************** SOUND **************** */

        bool drawLongLat;

        std::map<int, activeDot> soundElements;

        ofxOscReceiver receiver;
		ofxOscSender localSender;

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
		bool sphereFill;
		bool sphereColor;

		ofParameter<bool> ewaldSphere;
		bool displayEwald;
		ofParameter<float> ewaldMargin;
		ofParameter<float> waveLength;
		ofParameter<float> rotateCrystal;
		ofParameter<float> tiltCrystal;
        ofParameter<bool> sendOSC;


};
