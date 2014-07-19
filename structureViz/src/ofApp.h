#pragma once

#include "ofMain.h"
#include "scaData.h"

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


		scaData sca;

		ofPoint spaceDim;
		ofPoint spaceO;	// origin 

		float scale;

		bool useEasyCam;
		bool drawAxis;

		ofEasyCam cam;

		ofCamera camera[1];
		int camToView;
		
};
