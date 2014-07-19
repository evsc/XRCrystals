#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){

	cout << "listening for osc messages on port " << PORT << "\n";
	receiver.setup(PORT);

}

//--------------------------------------------------------------
void ofApp::update(){

	// check for waiting messages
	while(receiver.hasWaitingMessages()){

		// get the next message
		ofxOscMessage m;
		receiver.getNextMessage(&m);
        
		// check for mouse moved message
		if(m.getAddress() == "/head"){
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

	ofBackground(30,30,30);

	ofSetColor(255);
	ofDrawBitmapString(ofToString(head.x, 0)+" "+ofToString(head.y, 0)+" "+ofToString(head.z, 0), 12, 24);


	ofPoint kRoom = ofPoint(640,480,2000); 	// kinect room dimensions

	float m = 0.5;		// scale incoming head position, for better display
	float zm = 0.15;

	ofNoFill();

	// draw top down
	ofPushMatrix();
	ofTranslate(10,60);
	ofRect(0,0,kRoom.x*m,kRoom.z*zm);
	ofFill();
	ofCircle(head.x*m, kRoom.z*zm - head.z*zm,10);

	// draw kinect position
	ofRect(kRoom.x*m*0.5, kRoom.z*zm, 10,5);


	ofNoFill();

	// draw frontal
	ofTranslate(kRoom.x*m + 20,0);
	ofRect(0,0,kRoom.x*m,kRoom.y*m);
	ofFill();
	ofCircle(head.x*m, head.y*m,10);

	ofPopMatrix();

}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

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
