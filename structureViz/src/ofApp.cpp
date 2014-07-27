#include "ofApp.h"


//--------------------------------------------------------------
void ofApp::setup(){

	resetSettings();
	loadSettings();

	// LOAD AND PARSE A scalepack file
	// sca.parseFile("hfpps.sca");
	sca.parseFile(dataFile);

	// cout << "length data vector = " << sca.data.size() << endl;
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

	ofBackground(100);
	ofPushMatrix();
	// ofTranslate(spaceO.x, spaceO.y, spaceO.z);
	ofScale(scale,scale,scale);
	// ofTranslate(-spaceDim.x/2, -spaceDim.y/2, -spaceDim.z/2);


	if (drawAxis) ofDrawAxis(100);

	// draw space
	ofNoFill(); ofColor(255);
	// ofDrawBox(-spaceDim.x/2,-spaceDim.y/2,-spaceDim.z/2, spaceDim.x, spaceDim.y, spaceDim.z);


	float uc_h = sca.unitCellDimension.x;
	float uc_k = sca.unitCellDimension.y;
	float uc_l = sca.unitCellDimension.z;

	// mirror HKL group, so it forms full cirlce
	for (int i=0; i<8; i++) {
		ofPushMatrix();
		ofRotateZ(i * (360.0/8.0));

		// draw points
		for (vector<scaItem>::iterator it = sca.data.begin() ; it != sca.data.end(); ++it) {
			if ((*it).intensity > minIntensity) {
				ofDrawSphere((*it).h*uc_h, (*it).k*uc_k, (*it).l*uc_l, drawDots*(*it).intensity);
			}
		}	

		// mirrored version, to complete sphere
		for (vector<scaItem>::iterator it = sca.data.begin() ; it != sca.data.end(); ++it) {
			if ((*it).intensity > minIntensity) {
				ofDrawSphere((*it).h*uc_h, (*it).k*uc_k, -(*it).l*uc_l, drawDots*(*it).intensity);
			}
		}	
		ofPopMatrix();
	}

	ofPopMatrix();


	cam.end();


	// if (useEasyCam) cam.end();
	// else camera[camToView].end();


	gui.draw();

}



void ofApp::resetSettings() {

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
	gui.add(minIntensity.set( "minimum intensity to draw", 500000, 0, 1000000 ));
	gui.add(drawDots.set( "intensity draw factor", 0.000002, 0, 0.00002 ));
	gui.add(dataFile.set("Daniels-lysozyme.sca"));

}

void ofApp::loadSettings() {

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
	else if (key == 'm') {
		scale+=1;
	}
	else if (key == 'n') {
		scale-=1;
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
