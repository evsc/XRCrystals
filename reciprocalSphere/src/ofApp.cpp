#include "ofApp.h"


//--------------------------------------------------------------
void ofApp::setup(){

	// ofSetVerticalSync(true);

	resetSettings();
	loadSettings();


	// // sphere.setRadius( 100 );
	// // sphere.setResolution( 4 );
	foo = ofMesh::sphere(1, 12);
	// // foo = sphere.getMesh();
	// ofMesh cone;
	// cone = ofMesh::cone(100.0, 200.0);
	// foo.append(cone);
	// foo.save("sphere.ply");

	// vboSphere.setMesh(foo, GL_STATIC_DRAW);
	// vboSphereMesh.setMesh(foo, GL_STATIC_DRAW);
	vboSphereMesh = foo;

	// LOAD AND PARSE A scalepack file
	// sca.parseFile(dataFile, SCA_FIRST);
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

	ewaldSphereRadius = 0.1/waveLength;
	ewaldO = ofVec3f(0,0,ewaldSphereRadius);
	cout << "ewaldSphereRadius = " << ewaldSphereRadius << endl;

	// cout << "getNearClip : " << cam.getNearClip() << endl;
	// cout << "getFarClip : " << cam.getFarClip() << endl;


	useEasyCam = true;
	drawAxis = true;

	cam.setDistance(100);
	cam.enableOrtho();
	cam.setFov(60);
	cam.setPosition(spaceO.x, spaceO.y, spaceO.z);

	// set so nothing gets clipped off
	cam.setFarClip(2000);
	cam.setNearClip(-1000);

	camToView = 0;
	camera[0].resetTransform();
	camera[0].setFov(60);
	camera[0].setPosition(-ofGetWidth()/2, -ofGetHeight()/2, 0);
	camera[0].enableOrtho();


	// glEnable(GL_DEPTH_TEST);
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


	float uc_h = sca.unitCellDimension.x;
	float uc_k = sca.unitCellDimension.y;
	float uc_l = sca.unitCellDimension.z;

	ofRotateX(viewRotation.x);
	ofRotateY(viewRotation.y);
	ofRotateZ(viewRotation.z);

	if (drawAxis) ofDrawAxis(100);

	if (displayEwald) {
		ofNoFill();
		ofSetColor(0);
		ofDrawSphere(ewaldO.x,ewaldO.y,ewaldO.z,ewaldSphereRadius);
	}

	// ofRotateX(rotateHKL.x);
	// ofRotateY(rotateHKL.y);
	// ofRotateZ(rotateHKL.z);

	rotateHKL.y += rotateCrystal;

	// let's count the nodes drawn, to compare
	visibleNodes = 0;
    //soundElements.clear();
    std::map<int,activeDot>::iterator it = soundElements.begin();
    while (it != soundElements.end()) {
        activeDot* dot = &it->second;
        dot->updated = false;
        it++;
    }

	ofSetColor(255*sphereBrightness, 255*sphereAlpha);

	float mirrorRotZ = 0;

	// mirror HKL group, so it forms full cirlce
	for (int m=0; m<4; m++) {
		ofPushMatrix();
		mirrorRotZ = m * (360.0/4.0);
		// ofRotateZ(mirrorRotZ);

		for (int i=0; i<4; i++) {

			// on second time around, we'll mirror the points to complete the sphere
			float flipz = 1.0;
			float flipx = 1.0;
			if (i>1) flipx = -1.0;
			if (i%2==1) flipz = -1.0;

			int mirrorno = m*4 + i;

			// draw points
			for (vector<scaItem>::iterator it = sca.data.begin() ; it != sca.data.end(); ++it) {


				// Intensity check
				if ((*it).intensity > minIntensity*sca.maxIntensity) {

					// HKL drawing filter
					if ((*it).h <= drawMaxH && (*it).k <= drawMaxK && (*it).l <= drawMaxL) {

						ofVec3f hkl = ofVec3f((*it).h*flipx*uc_h,(*it).k*uc_k,(*it).l*flipz*uc_l);
						hkl.rotate(mirrorRotZ, ofVec3f(0, 0, 1));
						hkl.rotate(rotateHKL.y, ofVec3f(0, 1, 0));
						hkl.rotate(tiltCrystal, ofVec3f(1, 0, 0));

						// avoid doubling of the mirror-plane
						if ((hkl.x != hkl.y) || (i<2)) {

                            // check for intersection with ewald sphere
							if ( !ewaldSphere || onEwaldSphere(hkl.x,hkl.y,hkl.z, mirrorRotZ) ) {

                                // cout << "hkl \t" << (*it).h << "\t" << (*it).k << "\t" << (*it).l << endl;

                                float latitude = hkl.y;
                                float longitude = ((atan2(hkl.x-ewaldO.x, ewaldO.z-hkl.z)) *180 / PI);
                                // ... both change over time, if crystal is tiled

                                // register dot as currently visible, for sound trigger
                                // identifier= (*it).h  (*it).k  (*it).l


                                // first look if a object with that coordinates is already registered
                                int coordinates = mirrorno*1000000 + ((*it).h+50)*10000 + ((*it).k+50)*100 + (*it).l+50;
                                std::map<int,activeDot>::iterator iter = soundElements.find(coordinates);
                                bool newDot = false;

                                if( iter != soundElements.end() ) {
                                    // found it, only need to update it
                                } else {
                                    // create new
                                    newDot = true;
                                    // cout << "new sound to be created \t" << coordinates << endl;
                                    // these values we only set once
                                    soundElements[coordinates].h = (*it).h;
                                    soundElements[coordinates].k = (*it).k;
                                    soundElements[coordinates].l = (*it).l;
                                    soundElements[coordinates].mirrorno = mirrorno;
                                    soundElements[coordinates].longitude = longitude;
                                    soundElements[coordinates].latitude = latitude;
                                    soundElements[coordinates].intensity = (*it).intensity;
                                    soundElements[coordinates].fresh = true;
                                    soundElements[coordinates].phase = (*it).phase;
                                }
                                activeDot* dot = &soundElements.find(coordinates)->second;
                                dot->updated = true;

								if (sphereFill) ofFill();
								else ofNoFill();

								if (sphereColor) {
									ofColor c = ofColor(255*sphereBrightness,0,0,255*sphereAlpha);
									c.setHue( (*it).phase * (255.0/360.0) );
									ofSetColor( c );
								}

								if (drawMode==0) {

									// draw directly with simple OF function
									ofDrawSphere(hkl.x,hkl.y,hkl.z, nodeScale*(*it).intensity);
                                    ofFill();
                                    ofSetColor(ofColor(255));
                                    ofDrawBitmapString(ofToString(latitude,0)+"\n"+ofToString(longitude,1),hkl.x,hkl.y,hkl.z);
								} else {



									// include push/pop translates in every drawing step
									ofPushMatrix();
									ofTranslate(hkl.x,hkl.y,hkl.z);

									float sc = nodeScale*(*it).intensity;
									ofScale(sc,sc,sc);

									if (drawMode==1) {
										if (sphereFill) foo.drawFaces();
										else foo.drawWireframe();
									}
									else if (drawMode==2) {
										if (sphereFill) vboSphereMesh.draw(OF_MESH_FILL);
										else vboSphereMesh.draw(OF_MESH_WIREFRAME);
									}

									ofPopMatrix();

								}

								visibleNodes++;
							}
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


	// sound related
	// start sound for fresh dots
	// and stop sounds and delete non-active dots
    std::map<int,activeDot>::iterator it2 = soundElements.begin();
    while (it2 != soundElements.end()) {
        activeDot* dot = &it2->second;
        int coord = (dot->mirrorno)*1000000 + (dot->h+50)*10000 + (dot->k+50)*100 + dot->l+50;
        if ( dot->fresh ) {
            dot->fresh = false;
            cout << "fresh sound \t" << coord << endl;
        }
        if( !dot->updated ) {
            cout << "this sound needs to go away \t" << coord << endl;
            soundElements.erase(it2++);
        } else {
            ++it2;
        }
    }



	// if (useEasyCam) cam.end();
	// else camera[camToView].end();


	if (showGUI) {

		gui.draw();


		ofFill();
		ofSetColor(ofColor(255));
		stringstream keyInstructions;
		keyInstructions << " a  ... draw axis " << endl;
		keyInstructions << " f  ... fill nodes" << endl;
		keyInstructions << " c  ... color nodes with phase" << endl;
		keyInstructions << " e  ... draw Ewald sphere " << endl;
		keyInstructions << " r  ... reset camera" << endl;
		keyInstructions << " x  ... view from x axis " << endl;
		keyInstructions << " y  ... view from y axis " << endl;
		keyInstructions << " z  ... view from z axis " << endl;
		keyInstructions << " m  ... drawing mode (";
		switch (drawMode) {
			case 0: keyInstructions << "ofDrawSphere)" << endl;
			break;
			case 1: keyInstructions << "mesh)" << endl;
			break;
			case 2: keyInstructions << "ofVboMesh)" << endl;
			break;
		}
		keyInstructions << "' ' ... toggle fullscreen " << endl;
		keyInstructions << " p  ... save screenshot " << endl;


		ofDrawBitmapString(keyInstructions.str(), 20, 550);

		// draw phase color diagram
		ofFill();
		int x = 10;
		int y = 510;
		int h = 20;
		int w = 400;
		int deg = 10;
		float step = float(w)/(360.0/float(deg));
		for (int i=0; i<360; i+=deg) {
			ofColor c = ofColor(255*sphereBrightness,0,0,255);
			c.setHue( i * (255.0/360.0) );
			ofSetColor( c );
			ofRect(x+(i/deg)*step,y,step,h);
		}
	}

	ofDrawBitmapString(ofToString(ofGetFrameRate(),0) + " FPS", ofGetWidth()-105, ofGetHeight()-30);
	ofDrawBitmapString(ofToString(visibleNodes) + " Nodes", ofGetWidth()-105, ofGetHeight()-50);
	ofDrawBitmapString(ofToString(soundElements.size()) + " Sounds", ofGetWidth()-105, ofGetHeight()-70);

}

/* 	determine if an HKL point lies on the Ewald Sphere
	which means, that it satisfies the Bragg equation */
bool ofApp::onEwaldSphere(int h, int k, int l, float mirrorRotZ) {


	// distance between HKL grid-point needs to be close to ewaldSphere radius
	ofVec3f hkl = ofVec3f(h,k,l);
	// hkl.rotate(mirrorRotZ, ofVec3f(0, 0, 1));
	// hkl.rotate(rotateHKL.y, ofVec3f(0, 1, 0));

	// anything behind the HKL-origin is outside the sphere right away
	if (hkl.z < 0) return false;

	// anything in front of the crystal is out right away
	// crystal position = ewaldO position
	if (hkl.z > ewaldO.z) return false;

	// if ((h == 3 || h == -3) && (k == 3 || k == -3) && (l == 3 || l == -3) ) {
	// 	cout << "point(" << h << "," << k << "," << l << ") becomes " << hkl.x << " " << hkl.y << " " << hkl.z << endl;
	// }


	float distance = ewaldO.distance( hkl );
	// if (h == 3 && k == 3 && l == 3 ) {
	// 	cout << "ewaldO = " << ewaldO.x << " " << ewaldO.y << " " << ewaldO.z << "    distance = " << distance << endl;
	// }

	// if (distance < ewaldSphereRadius) return true;

	if (distance > ewaldSphereRadius - ewaldMargin && distance < ewaldSphereRadius + ewaldMargin) {
		return true;
	}
	// cout << "point(" << h << "," << k << "," << l << ") distance = " << distance << endl;


	return false;
}


void ofApp::resetSettings() {

	drawMode = 0;
	showGUI = true;
	sphereFill = true;
	sphereColor = true;

	viewRotation = ofVec3f(0,0,0);
	rotateHKL = ofVec3f(0,0,0);

	// style the GUI
	ofxGuiSetDefaultWidth(400);
	ofxGuiSetDefaultHeight(28);
	ofxGuiSetBackgroundColor(ofColor(50));
	ofxGuiSetHeaderColor(ofColor::red);
	ofxGuiSetBorderColor(ofColor::black);
	ofxGuiSetTextColor(ofColor(200,255,0));
	ofxGuiSetTextPadding(10);
	ofxGuiSetFont("mono.ttf", 12, true, true);


	// GUI listeners
	sphereResolution.addListener(this,&ofApp::changeSphereResolution);
	waveLength.addListener(this,&ofApp::changeWaveLength);


	// GUI setup
	gui.setup("HKL diffraction space");
	gui.add(dataFile.set("data file", "Daniels-lysozyme.sca"));
	gui.add(zoom.set( "zoom", 1, 0.5, 15 ));
	gui.add(mirror.set( "mirror", true));
	gui.add(minIntensity.set( "intensity filter", 0.5, 0, 1 ));
	gui.add(nodeScale.set( "node display scaling", 0.000002, 0, 0.0002 ));

	gui.add(drawMaxH.set("draw max H index", 10, 1, 50));
	gui.add(drawMaxK.set("draw max K index", 10, 1, 50));
	gui.add(drawMaxL.set("draw max L index", 10, 1, 50));
	gui.add(sphereResolution.set("sphere resolution", 3, 2, 20));
	gui.add(sphereBrightness.set("sphere brightness", 0.5, 0, 1));
	gui.add(sphereAlpha.set("sphere alpha", 0.5, 0, 1));
	// gui.add(sphereFill.set("fill sphere", false));
	// gui.add(sphereColor.set("phase color", false));

	gui.add(ewaldSphere.set("apply Ewald sphere", false));
	gui.add(ewaldMargin.set("Ewald margin", 0.5, 0, 1));
	gui.add(waveLength.set("xray wave length", 0.005, 0, 0.03));
	gui.add(rotateCrystal.set("rotate crystal", 0.005, 0, 0.5));
	gui.add(tiltCrystal.set("tilt crystal", 0.0, 0, 180));

}

void ofApp::changeSphereResolution(int & sphereResolution){
	ofSetSphereResolution(sphereResolution);
}

void ofApp::changeWaveLength(float & waveLength){
	ewaldSphereRadius = 0.1/waveLength;
	ewaldO = ofVec3f(0,0,ewaldSphereRadius);
}

void ofApp::loadSettings() {
	gui.loadFromFile("settings.xml");
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
	if (key == 'c') {
		sphereColor = !sphereColor;
	}
	if (key == 'f') {
		sphereFill = !sphereFill;
	}
	if (key == 'o') {
		bool state = cam.getOrtho();
		if (!state) {
			cam.enableOrtho();
			// cam.setPosition(-ofGetWidth()/2, -ofGetHeight()/2, 100);
		} else {
			cam.disableOrtho();
			// cam.setPosition(0, 0, 200);
		}
	}
	else if (key == 'a') {
		drawAxis = !drawAxis;
	}
	else if (key == 'e') {
		displayEwald = !displayEwald;
	}
	else if (key == 'g') {
		showGUI = !showGUI;
	}
	else if (key == 'r') {
		rotateHKL = ofVec3f(0,0,0);
		viewRotation = ofVec3f(0,0,0);
		cam.reset();
		cam.enableOrtho();
		cam.setPosition(spaceO.x, spaceO.y, spaceO.z);
	}

	else if (key == 'x') {
		cam.reset();
		viewRotation = ofVec3f(0,0,0);
		viewRotation.x = 90;
		cam.setPosition(spaceO.x, spaceO.y, spaceO.z);
	}
	else if (key == 'y') {
		cam.reset();
		viewRotation = ofVec3f(0,0,0);
		viewRotation.y = -90;
		cam.setPosition(spaceO.x, spaceO.y, spaceO.z);
	}
	else if (key == 'z') {
		cam.reset();
		viewRotation = ofVec3f(0,0,0);
		cam.setPosition(spaceO.x, spaceO.y, spaceO.z);
	}
	else if (key == 'm') {
		drawMode++;
		if (drawMode > 2) drawMode=0;
		cout << "drawMode: " << drawMode << endl;
	}

	if(key == ' ') {
		ofToggleFullscreen();
	}

	if (key=='p') {
		// save a screenshot
        ofImage img;
        img.grabScreen(0,0,ofGetWidth(), ofGetHeight());
        int lastIndex = dataFile.toString().find_last_of(".");
		string rawFileName = dataFile.toString().substr(0, lastIndex);
        string fileName = "screenshots/" + rawFileName + "_"+ofGetTimestampString()+".png";
        img.saveImage(fileName);
        cout << "saved screenshot " << fileName.c_str() << endl;
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
