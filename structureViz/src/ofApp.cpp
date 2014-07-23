#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){


	// sca.parseFile("hfpps.sca");
	sca.parseFile("daniel.sca");

	// cout << "length data vector = " << sca.data.size() << endl;
	spaceDim = ofPoint(sca.maxH-sca.minH, sca.maxK-sca.minK, sca.maxL-sca.minL);
	spaceO = ofPoint(ofGetWidth()/2,ofGetHeight()/2,0);

	scale = 20.0;

	// spaceDim*=scale;
	cout << "sphere resolution = " << ofGetSphereResolution() << endl;
	ofSetSphereResolution(3);

	cout << "getNearClip : " << cam.getNearClip() << endl;
	cout << "getFarClip : " << cam.getFarClip() << endl;


	useEasyCam = true;
	drawAxis = true;

	cam.setDistance(100);
	cam.enableOrtho();
	// cam.setFov(60);
	cam.setPosition(-ofGetWidth()/2, -ofGetHeight()/2, 0);
	// cam.setFarClip(2000000);
	// cam.setNearClip(-1000000);

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
	ofTranslate(-spaceDim.x/2, -spaceDim.y/2, -spaceDim.z/2);


	if (drawAxis) ofDrawAxis(100);

	// draw space
	ofNoFill(); ofColor(255);
	// ofDrawBox(-spaceDim.x/2,-spaceDim.y/2,-spaceDim.z/2, spaceDim.x, spaceDim.y, spaceDim.z);

	// draw points
	for (vector<scaItem>::iterator it = sca.data.begin() ; it != sca.data.end(); ++it) {
		ofDrawSphere((*it).h, (*it).k, (*it).l, 0.0000002*(*it).intensity);
	}	
	// ofDrawSphere(spaceO.x, spaceO.y, spaceO.z, 5);
	ofPopMatrix();


	cam.end();

	// if (useEasyCam) cam.end();
	// else camera[camToView].end();

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
		cam.setPosition(-ofGetWidth()/2, -ofGetHeight()/2, 100);
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
