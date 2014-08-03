#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){

	createGUI();
	resetSettings();
	loadSettings();

	font.loadFont("mono.ttf", 36);

	cout << "listening for osc messages on port " << oscPort << "\n";
	receiver.setup(ofToInt(oscPort));


	// cameras setup

	cam.setDistance(100);
	cam.enableOrtho();
	cam.setFov(80);
	cam.setPosition(-ofGetWidth()/2,-ofGetHeight()/2,100);

	// set so nothing gets clipped off
	cam.setFarClip(2000);
	cam.setNearClip(-1000);


	viewRotation = ofVec3f(45,-45,0);
}

//--------------------------------------------------------------
void ofApp::update(){

	// check for waiting messages
	while(receiver.hasWaitingMessages()){

		// get the next message
		ofxOscMessage m;
		receiver.getNextMessage(&m);
        
		// check for mouse moved message
		if(m.getAddress() == oscAddress.toString()){
			// both the arguments are int32's
			head.x = m.getArgAsFloat(0);
			head.y = m.getArgAsFloat(1);
			head.z = m.getArgAsFloat(2);

			// cout << "head position at " << head.x << " | " << head.y << " | " << head.z << endl;
		}
	}

}

//--------------------------------------------------------------
void ofApp::draw(){


	cam.begin();	

	ofBackground(30,30,30);

	ofPushMatrix();
	float sc = 0.15 * (ofGetWidth()/900.0);
	ofScale(sc,sc,sc);

	ofRotateX(viewRotation.x);
	ofRotateY(viewRotation.y);
	ofRotateZ(viewRotation.z);

	ofSetColor(255);
	ofNoFill();

	ofVec3f boxO = ofVec3f(0,0,-room.z/2);
	ofDrawBox(room.x, room.y, room.z);

	// ofSetColor(255,100);

	ofPushMatrix();
	ofTranslate(boxO);

	// draw point-planes
	drawPlane( ofPoint(head.x,-room.y/2,0), ofPoint(head.x,room.y/2,0), ofPoint(head.x, room.y/2, room.z), ofPoint(head.x, -room.y/2, room.z ));
	drawPlane( ofPoint(-room.x/2,head.y,0), ofPoint(room.x/2,head.y,0), ofPoint(room.x/2, head.y, room.z), ofPoint(-room.x/2, head.y, room.z ));
	drawPlane( ofPoint(-room.x/2,-room.y/2,head.z), ofPoint(room.x/2,-room.y/2,head.z), ofPoint(room.x/2, room.y/2, head.z), ofPoint(-room.x/2,room.y/2, head.z));

	// draw head
	ofFill();
	ofDrawBox(head, 150);

	// draw kinect
	ofDrawBox(0,0,0, 250,50,80);


	ofPopMatrix();
	ofPopMatrix();

	cam.end();


	if (showGUI) {
		gui.draw();

		font.drawString("x: "+ofToString(head.x,2), 30,250);
		font.drawString("y: "+ofToString(head.y,2), 30,300);
		font.drawString("z: "+ofToString(head.z,2), 30,350);

	}


}


void ofApp::drawPlane(ofPoint p1, ofPoint p2, ofPoint p3, ofPoint p4) {

	ofVec3f dot;
	float step;
	float minStep = 0.02f;

	step = max(minStep,30.f/p1.distance( p2 ));
	for (float i=0; i<=1; i+=step) {
		dot = p1.getInterpolated( p2, i );
		ofRect(dot,1,1);
	}
	step = max(minStep,30.f/p2.distance( p3 ));
	for (float i=0; i<=1; i+=step) {
		dot = p2.getInterpolated( p3, i );
		ofRect(dot,1,1);
	}
	step = max(minStep,30.f/p3.distance( p4 ));
	for (float i=0; i<=1; i+=step) {
		dot = p3.getInterpolated( p4, i );
		ofRect(dot,1,1);
	}
	step = max(minStep,30.f/p4.distance( p1 ));
	for (float i=0; i<=1; i+=step) {
		dot = p4.getInterpolated( p1, i );
		ofRect(dot,1,1);
	}
}


void ofApp::createGUI() {

	// style the GUI
	ofxGuiSetDefaultWidth(300);
	ofxGuiSetDefaultHeight(25);
	ofxGuiSetBackgroundColor(ofColor(50));
	ofxGuiSetHeaderColor(ofColor(100));
	ofxGuiSetTextColor(ofColor(200,255,0));
	ofxGuiSetTextPadding(5);
	ofxGuiSetFont("mono.ttf", 12, true, true);

	roomX.addListener(this,&ofApp::changeRoomX);
	roomY.addListener(this,&ofApp::changeRoomY);
	roomZ.addListener(this,&ofApp::changeRoomZ);

	gui.setup("room tracking");
	gui.add(roomX.set( "room x", 640, 0, 3000 ));
	gui.add(roomY.set( "room y", 480, 0, 3000 ));
	gui.add(roomZ.set( "room z", 2000, 0, 3000 ));
	gui.add(oscPort.set("osc port", "8000"));
	gui.add(oscAddress.set("osc address", "/head"));

}

void ofApp::resetSettings() {
	showGUI = true;
	oscPort = "8000";
	roomX = 640;
	roomY = 480;
	roomZ = 2000;
	room = ofVec3f(roomX,roomY,roomZ);
}

void ofApp::loadSettings() {
	gui.loadFromFile("settings.xml");

	room = ofVec3f(roomX,roomY,roomZ);
}

void ofApp::changeRoomX(float & roomX) {
	room = ofVec3f(roomX,roomY,roomZ);
}
void ofApp::changeRoomY(float & roomY) {
	room = ofVec3f(roomX,roomY,roomZ);
}
void ofApp::changeRoomZ(float & roomZ) {
	room = ofVec3f(roomX,roomY,roomZ);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

	if (key == 'g') {
		showGUI = !showGUI;
	}

	else if (key == 'p') {
		// save a screenshot
        ofImage img;
        img.grabScreen(0,0,ofGetWidth(), ofGetHeight());
        string fileName = "screenshots/room_"+ofGetTimestampString()+".png";
        img.saveImage(fileName);
        cout << "saved screenshot " << fileName.c_str() << endl;
	}

	else if (key == 'r') {
		cam.reset();
		cam.enableOrtho();
		cam.setPosition(-ofGetWidth()/2,-ofGetHeight()/2,100);
	}


	if(key == 'f') {
		ofToggleFullscreen();
		cam.reset();
		cam.enableOrtho();
		cam.setPosition(-ofGetWidth()/2,-ofGetHeight()/2,100);
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
