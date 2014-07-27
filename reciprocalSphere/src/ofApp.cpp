#include "ofApp.h"


//--------------------------------------------------------------
void ofApp::setup(){

	resetSettings();
	loadSettings();

	// LOAD AND PARSE A scalepack file
	sca.parseFile(dataFile);

	// FIGURE out how big to draw
	hklDim = ofVec3f(sca.maxH-sca.minH, sca.maxK-sca.minK, sca.maxL-sca.minL);

	spaceDim = ofVec3f(sca.unitCellDimension.x*hklDim.x,sca.unitCellDimension.y*hklDim.y);
	float maxRadius = max(spaceDim.x, spaceDim.y);
	maxRadius = max (maxRadius, spaceDim.z);
	cout << "biggest dimension = " << maxRadius << endl;	

	scale = float(ofGetWidth()) / (maxRadius*2.3);
	cout << "ideal multiplication factor = " << scale << endl;
	spaceO = ofPoint(-ofGetWidth()/2,-ofGetHeight()/2,100);

	// cout << "sphere resolution = " << ofGetSphereResolution() << endl;
	ofSetSphereResolution(3);

	// cout << "getNearClip : " << cam.getNearClip() << endl;
	// cout << "getFarClip : " << cam.getFarClip() << endl;

	useEasyCam = true;
	drawAxis = true;

	cam.setDistance(100);
	cam.enableOrtho();
	cam.setFov(60);
	// cam.setPosition(-ofGetWidth()/2, -ofGetHeight()/2, 100);
	cam.setPosition(spaceO.x, spaceO.y, spaceO.z);

	// set so nothing gets clipped off
	cam.setFarClip(2000);
	cam.setNearClip(-1000);

	camToView = 0;
	camera[0].resetTransform();
	camera[0].setFov(60);
	camera[0].setPosition(-ofGetWidth()/2, -ofGetHeight()/2, 0);
	camera[0].enableOrtho();
}

//--------------------------------------------------------------
void ofApp::update(){
	// camera[0].setPosition(mouseX, mouseY, 0);
	// cam.setFarClip(mouseX);
	// cam.setNearClip(mouseY);
}

//--------------------------------------------------------------
void ofApp::draw(){

	cam.begin();	

	// if (useEasyCam) cam.begin();	
	// else camera[camToView].begin();

	ofBackground(100,115,115);
	ofPushMatrix();
	ofScale(scale*zoom,scale*zoom,scale*zoom);


	if (drawAxis) ofDrawAxis(100);

	// draw space
	ofNoFill(); ofColor(255);
	// ofDrawBox(-spaceDim.x/2,-spaceDim.y/2,-spaceDim.z/2, spaceDim.x, spaceDim.y, spaceDim.z);


	float uc_h = sca.unitCellDimension.x;
	float uc_k = sca.unitCellDimension.y;
	float uc_l = sca.unitCellDimension.z;

	ofRotateX(viewRotation.x);
	ofRotateY(viewRotation.y);
	ofRotateZ(viewRotation.z);

	// mirror HKL group, so it forms full cirlce
	for (int i=0; i<4; i++) {
		ofPushMatrix();
		ofRotateZ(i * (360.0/4.0));

		for (int i=0; i<4; i++) {

			// on second time around, we'll mirror the points to complete the sphere
			float flipz = 1.0;
			float flipx = 1.0;
			if (i>1) flipx = -1.0;
			if (i%2==1) flipz = -1.0;

			// draw points
			for (vector<scaItem>::iterator it = sca.data.begin() ; it != sca.data.end(); ++it) {
				if ((*it).intensity > minIntensity) {
					if ((*it).h <= drawMaxH && (*it).k <= drawMaxK && (*it).l <= drawMaxL) {
						// avoid doubling of the mirror-plane
						if (((*it).h != (*it).k) || (i<2)) {
							ofDrawSphere((*it).h*uc_h*flipx, (*it).k*uc_k, (*it).l*uc_l*flipz, drawDots*(*it).intensity);
						}
					}
				}
			}
			if (!mirror) break;
		}

		ofPopMatrix();
		if (!mirror) break;
	}

	ofPopMatrix();


	cam.end();


	// if (useEasyCam) cam.end();
	// else camera[camToView].end();


	gui.draw();



	ofFill();
	ofColor(ofColor(255));
	stringstream keyInstructions;
	keyInstructions << "r ... reset camera" << endl;
	keyInstructions << "a ... draw axis " << endl;
	keyInstructions << "x ... view from x axis " << endl;
	keyInstructions << "y ... view from y axis " << endl;
	keyInstructions << "z ... view from z axis " << endl;


	ofDrawBitmapString(keyInstructions.str(), 20, 500);

}



void ofApp::resetSettings() {

	viewRotation = ofVec3f(0,0,0);

	// style the GUI
	ofxGuiSetDefaultWidth(400);
	ofxGuiSetDefaultHeight(50);
	ofxGuiSetBackgroundColor(ofColor(50));
	ofxGuiSetHeaderColor(ofColor::red);
	ofxGuiSetBorderColor(ofColor::black);
	ofxGuiSetTextColor(ofColor(200,255,0));
	ofxGuiSetTextPadding(12);
	ofxGuiSetFont("mono.ttf", 14, true, true);


	// GUI setup
	gui.setup("HKL diffraction space");
	gui.add(zoom.set( "zoom", 1, 0.5, 7 ));
	gui.add(mirror.set( "mirror", true));
	gui.add(minIntensity.set( "minimum intensity to draw", 500000, 0, 1000000 ));
	gui.add(drawDots.set( "intensity draw factor", 0.000002, 0, 0.00005 ));
	gui.add(dataFile.set("data file", "Daniels-lysozyme.sca"));

	gui.add(drawMaxH.set("draw max H index", 10, 1, 50));
	gui.add(drawMaxK.set("draw max K index", 10, 1, 50));
	gui.add(drawMaxL.set("draw max L index", 10, 1, 50));

}

void ofApp::loadSettings() {
	gui.loadFromFile("settings.xml");
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
	// if (key == 'c') {
	// 	useEasyCam = !useEasyCam;
	// }
	if (key == 'o') {
		bool state = cam.getOrtho();
		if (!state) {
			cam.enableOrtho();
			cam.setPosition(-ofGetWidth()/2, -ofGetHeight()/2, 100);
		} else {
			cam.disableOrtho();
			cam.setPosition(0, 0, 200);
		}
	}
	else if (key == 'a') {
		drawAxis = !drawAxis;
	}

	else if (key == 'r') {
		cam.reset();
		cam.enableOrtho();
		cam.setPosition(spaceO.x, spaceO.y, spaceO.z);
	}

	else if (key == 'x') {
		cam.reset();
		cam.setPosition(spaceO.x, spaceO.y, spaceO.z);
		viewRotation = ofVec3f(0,0,0);
		viewRotation.x = 90;
	}
	else if (key == 'y') {
		cam.reset();
		cam.setPosition(spaceO.x, spaceO.y, spaceO.z);
		viewRotation = ofVec3f(0,0,0);
		viewRotation.y = 90;
	}
	else if (key == 'z') {
		cam.reset();
		cam.setPosition(spaceO.x, spaceO.y, spaceO.z);
		viewRotation = ofVec3f(0,0,0);
		viewRotation.z = 90;
	}
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
