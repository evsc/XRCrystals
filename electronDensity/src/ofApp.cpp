#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){


	createGUI();
	resetSettings();
	loadSettings();

	data.parseFile("lysozyme.na4");

	// inverse of unit cell dimensions
	uc = ofVec3f( 1.f/data.cell_dim.x, 1.f/data.cell_dim.y, 1.f/data.cell_dim.z );
	uc.normalize();


	cam.setDistance(100);
	cam.enableOrtho();
	cam.setFov(60);
	cam.setPosition(-ofGetWidth()/2,-ofGetHeight()/2,100);

	// set so nothing gets clipped off
	cam.setFarClip(2000);
	cam.setNearClip(-1000);


	ofSetSphereResolution(6);


	// contour.SetFieldFcn(contourFunction);

	// set limits
	double pLimits[]={0,1,2,3,4};
    cout << "contour: set limits" << endl;
    contour.SetLimits(pLimits);

	// set iso contour values
	int n = 20;
    vector<double> vIso(n); 
	for (unsigned int i=0;i<vIso.size();i++) {
		vIso[i]=(i-vIso.size()/2.0)*0.1;
	}
    // setting iso-lines
    cout << "contour: set planes" << endl;
    // contour.SetPlanes(vIso); // crashes

    // generate contour lines, based on contourFunction
    cout << "contour: generate" << endl;
    contour.Generate();

    cout << "contour: primary grid \t" << contour.GetColFir() << "\t" << contour.GetRowFir() << endl;
    cout << "contour: secondary grid \t" << contour.GetColSec() << "\t" << contour.GetRowSec() << endl;


}

// double contourFunction(double x, double y) {
// 	return 0.5*(cos(x+3.14/4)+sin(y+3.14/4));
// }

//--------------------------------------------------------------
void ofApp::update(){


	updatedHead = false;

	// check for waiting messages
	while(receiver.hasWaitingMessages()){

		ofxOscMessage m;
		receiver.getNextMessage(&m);
        
		if(m.getAddress() == oscAddress.toString()){

			head.x = m.getArgAsFloat(0);
			head.y = m.getArgAsFloat(1);
			head.z = m.getArgAsFloat(2);

			updatedHead = true;
		}
	}

}

//--------------------------------------------------------------
void ofApp::draw(){

	int visibleNodes = 0;

	ofBackground(0);

	if (draw3D) {

		// draw 3d version of full grid

		cam.begin();
		ofPushMatrix();

		float zoomV = pow(10,zoom) * 10;
		ofScale(zoomV,zoomV,zoomV);

		ofRotateX(viewRotation.x);
		ofRotateY(viewRotation.y);
		ofRotateZ(viewRotation.z);

		// draw one grid plane first

		ofFill();
		ofSetColor(255*drawBrightness, 255*drawAlpha);


		ofTranslate( -(data.sections * uc.x )/2.f, -(data.rows * uc.y )/2.f, -(data.cols * uc.z )/2.f);

		for (int section=0; section<data.sections; section++) {
			for (int row=0; row<data.rows; row++) {
				for (int col=0; col<data.cols; col++) {

					if (drawSection==-1 || drawSection==section) {
						if (drawRow==-1 || drawRow==row) {
							if (drawCol==-1 || drawCol==col) {
								float oV = data.map[section][row][col];
								float sV = oV * pow(10,nodeScale) * 0.01;
								if (oV>0 && oV>filter) {
									ofDrawSphere( section * uc.x, row * uc.y, col * uc.z, sV );
									visibleNodes++;
								}
							}
						}
					}


				}
			}
		}

		ofPopMatrix();
		cam.end();


		
	} else {


		// draw flat plane

		// ofFill();


		ofPushMatrix();
		ofTranslate( ofGetWidth()/2, ofGetHeight()/2, 0);

		// float zoomV = pow(10,zoom) * 10;
		// ofScale(zoomV,zoomV,zoomV);

		// ofTranslate( -(data.sections * uc.x )/2.f, -(data.rows * uc.y )/2.f, 0);

		// int col = drawCol;
		// if (col < 0 || col >= data.cols) col = 0;
		// for (int section=0; section<data.sections; section++) {
		// 	for (int row=0; row<data.rows; row++) {
		// 		float oV = data.map[section][row][col];
		// 		float sV = oV * pow(10,nodeScale) * 0.01;
		// 		ofSetColor( ofMap( oV, data.minValue, data.maxValue, 0, 255) );
		// 		ofRect(uc.x * section, uc.y * row, 0, uc.x, uc.y );
		// 	}
		// }


		// draw contour
		ofNoFill();
		ofSetColor(255);
		contour.Generate();


	    // Retreiving info
	    // CLineStripList* pStripList;

	    // getting 0-th iso-curve
	    // pStripList = contour.GetLines(0); 

	    // iterating liststrip vertices
	    // CLineStrip::iterator pos;
	    // for (pos = pStripList->begin(); 
	    //     pos != pStripList->end() ; pos++)
	    // {
	    //     CLineStrip pStrip = (*pos);

	    //     if (pStrip->empty()) {

	    //     }
	    // }





		ofPopMatrix();

	}


	if (showGUI) {

		gui.draw();

		ofFill();
		ofSetColor(ofColor(255));
		stringstream keyInstructions;
		keyInstructions << " r  ... reset camera" << endl;
		if (draw3D) {
			keyInstructions << " x  ... view from x axis " << endl;
			keyInstructions << " y  ... view from y axis " << endl;
			keyInstructions << " z  ... view from z axis " << endl;
		}
		keyInstructions << " f ... toggle fullscreen " << endl;
		keyInstructions << " p  ... save screenshot " << endl;

		ofDrawBitmapString(keyInstructions.str(), 20, 400);
	}



	ofDrawBitmapString(ofToString(ofGetFrameRate(),0) + " FPS", ofGetWidth()-105, ofGetHeight()-30);
	if (draw3D)	ofDrawBitmapString(ofToString(visibleNodes) + " Nodes", ofGetWidth()-105, ofGetHeight()-50);
	


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


	gui.setup("density map");
	gui.add(draw3D.set("draw 3d", false));
	gui.add(zoom.set( "zoom", 0, -1.f, 1.f ));
	gui.add(nodeScale.set( "node scale", 0, -1.f, 1.f ));
	gui.add(filter.set( "filter", 0, 0, 100 ));

	gui.add(drawAlpha.set( "draw alpha", 0.5, 0, 1 ));
	gui.add(drawBrightness.set( "draw brightness", 0.5, 0, 1 ));

	gui.add(drawSection.set( "draw section", -1, -1, 100));
	gui.add(drawRow.set( "draw row", -1, -1, 100));
	gui.add(drawCol.set( "draw col", -1, -1, 100));

	gui.add(oscPort.set("osc port", "8000"));
	gui.add(oscAddress.set("osc address", "/head"));

}


void ofApp::resetSettings() {

	showGUI = true;
	filter = 10;
	zoom = 0;
	nodeScale = 0;

	drawAlpha = 0.5;
	drawBrightness = 1.f;

	drawSection = -1;
	drawRow = -1;
	drawCol = -1;

	viewRotation = ofVec3f(0,0,0);

	uc = ofVec3f(1,1,1);

}


void ofApp::loadSettings() {

	gui.loadFromFile("settings.xml");

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
        string fileName = "screenshots/map_"+ofGetTimestampString()+".png";
        img.saveImage(fileName);
        cout << "saved screenshot " << fileName.c_str() << endl;
	}

	else if (key == 'r') {
		viewRotation = ofVec3f(0,0,0);
		cam.reset();
		cam.enableOrtho();
		cam.setPosition(-ofGetWidth()/2,-ofGetHeight()/2,100);
	}

	else if(key == 'f') {
		ofToggleFullscreen();
		cam.reset();
		cam.enableOrtho();
		cam.setPosition(-ofGetWidth()/2,-ofGetHeight()/2,100);
	}

	else if (key == 'x') {
		cam.reset();
		viewRotation = ofVec3f(0,0,0);
		viewRotation.x = 90;
		cam.enableOrtho();
		cam.setPosition(-ofGetWidth()/2,-ofGetHeight()/2,100);
	}
	else if (key == 'y') {
		cam.reset();
		viewRotation = ofVec3f(0,0,0);
		viewRotation.y = -90;
		cam.enableOrtho();
		cam.setPosition(-ofGetWidth()/2,-ofGetHeight()/2,100);
	}
	else if (key == 'z') {
		cam.reset();
		viewRotation = ofVec3f(0,0,0);
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



