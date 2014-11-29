#include "ofApp.h"
// #include <stdio.h>

//--------------------------------------------------------------
void ofApp::setup(){


    valSolid = 32;
    valEmpty = -32;

    grid[0] = 104;   // 62
    grid[1] = 234;   // 39
    grid[2] = 162;   // 31

    offset[0] = 20;
    offset[1] = 100;
    offset[2] = 50;

    // first let's clear all of space
    space = new float**[grid[0]];
	for ( int i=0; i<grid[0]; i++ ) {
	  space[i] = new float*[grid[1]];
	  for ( int j=0; j<grid[1]; j++ ) {
	  	space[i][j] = new float[grid[2]];
	  }
	}

    for (int i=0; i<grid[0]; i++) {
        for (int j=0; j<grid[1]; j++) {
            for (int k=0; k<grid[2]; k++) {
                space[i][j][k] = valEmpty;
            }
        }
    }

    parseVoxelFile("Teapot02_4MEZ_151_93_73.ASE");
    // saveOutNa4("Teapot02_4MEZ_151_93_73.na4");

    scale = 8.0f;
    drawAxis = true;
    cam.setDistance(100);
    cam.enableOrtho();
    cam.setFov(60);
    cam.setPosition(-ofGetWindowWidth()/2,-ofGetWindowHeight()/2,100);
    cam.setFarClip(2000);
    cam.setNearClip(-1000);





}

//--------------------------------------------------------------
void ofApp::update(){


}

//--------------------------------------------------------------
void ofApp::draw(){
    // draw voxels


    cam.begin();
    ofBackground(255);


    ofPushMatrix();
    // ofTranslate(ofGetWindowWidth()/2, ofGetWindowHeight()/2,0);
    ofScale(scale,scale,scale);
    ofTranslate(-grid[0]/2, -grid[1]/2, -grid[2]/2);


    for (int i=0; i<grid[0]; i++) {
        for (int j=0; j<grid[1]; j++) {
            for (int k=0; k<grid[2]; k++) {

                if(space[i][j][k] > 0) {
                    // draw voxel

                    ofNoFill();
                    ofSetColor(0);
                    ofDrawBox(i,j,k,1);

                    ofFill();
                    ofSetColor(100);
                    ofDrawBox(i,j,k,0.9);
                }
            }
        }
    }




    ofPopMatrix();
    cam.end();

    ofNoFill();
    ofSetColor(0);
    ofDrawBitmapString(ofToString(ofGetFrameRate(),0) + " FPS", ofGetWidth()-105, ofGetHeight()-30);

}

// 2.2834
// 4.8058
// 7.3283
// 9.8507
// gridstep = 2.5224
// minX    -50.5362         maxX   55.4067
// minY    -34.6221         maxY   33.484
// minZ    2.2834           maxZ   55.2548

void ofApp::parseVoxelFile(string voxelfile){

    cout << "let's parse those voxels from " << voxelfile << endl;

    ofBuffer file = ofBufferFromFile(voxelfile);

    int cnt = 0;
    string line;

    float minX = 1000;
    float maxX = -1000;
    float minY = 1000;
    float maxY = -1000;
    float minZ = 1000;
    float maxZ = -1000;

    // take off the minX values so that smallest value starts at 0
//    float cutOffX = -50.5362;
//    float cutOffY = -34.6221;
//    float cutOffZ = 2.2834;
//    float gridStep = 2.5224;
    float cutOffX = 0;
    float cutOffY = 0;
    float cutOffZ = 0;
    float gridStep = 1;

    while (!file.isLastLine()) {
        line = file.getNextLine();
        cnt++;
        while (line.substr(0,1) == "\t") line = line.substr(1);


        if (line.substr(0,15) == "*MESH_NUMVERTEX") {
            int numVertices = ofToInt(line.substr(16,4));
            cout << "number vertices = " << numVertices << endl;
        }

        if (line.substr(0,12) == "*MESH_VERTEX") {
            string line2 = line.substr(16);

            vector< string > result;
            result = ofSplitString(line2, "\t");
            // cout << line2 << "\t" << result.size() << endl;
            if (result.size() > 3) {
                int id = ofToInt(result[0]);
                int x = int((ofToFloat(result[1]) - cutOffX) / gridStep);
                int y = int((ofToFloat(result[2]) - cutOffY) / gridStep);
                int z = int((ofToFloat(result[3]) - cutOffZ) / gridStep);
                minX = min(minX, float(x));
				maxX = max(maxX, float(x));
				minY = min(minY, float(y));
				maxY = max(maxY, float(y));
				minZ = min(minZ, float(z));
				maxZ = max(maxZ, float(z));
                // cout << ofToString(x) << "\t" << ofToString(y) << "\t" << ofToString(z) << endl;

                int i = x+offset[0];
                int j = y+offset[1];
                int k = z+offset[2];
                if (i<grid[0] && j<grid[1] && k<grid[2]) {
                    space[i][j][k] = valSolid;
                }
            }
        }

        // if (cnt > 34 && cnt < 45) cout << cnt << "\t" << line << endl;
    }

    cout << "minX \t" << minX << "\t maxX \t" << maxX << endl;
    cout << "minY \t" << minY << "\t maxY \t" << maxY << endl;
    cout << "minZ \t" << minZ << "\t maxZ \t" << maxZ << endl;
}


void ofApp::saveOutNa4(string na4file){

    cout << "--------" << endl;
    cout << "save out voxel data in .na4 format" << endl;

    if (!ofFile::doesFileExist(na4file)) {
        outputFile = ofFile(ofToDataPath(na4file), ofFile::ReadWrite);
        outputFile.create();
    }
    outputFile.open(na4file, ofFile::ReadWrite);


    // write header
    outputFile << "MAPTONA4 HEADER" << "\n";
    outputFile << "TITLE" << "\n";
    outputFile << na4file << "\n";
    outputFile << "AXIS           Z       X       Y" << "\n";
    outputFile << "GRID         104     162     234" << "\n";

    outputFile << "XYZLIM  ";
    outputFile << ofToString(0, 8, ' ') << ofToString(grid[0]-1, 8, ' ');
    outputFile << ofToString(0, 8, ' ') << ofToString(grid[2]-1, 8, ' ');
    outputFile << ofToString(0, 8, ' ') << ofToString(grid[1]-1, 8, ' ');
    outputFile << "\n";

    outputFile << "SPACEGROUP             1" << "\n";
    outputFile << "MODE           2" << "\n";
    outputFile << "CELL        34.333    55.071    77.681    90.000    90.000    90.000" << "\n";
    outputFile << "RHOLIM      -1.61241         2.74221        0.114427E-03    0.454045    " << "\n";
    outputFile << "INDXR4     0  19" << "\n";
    outputFile << "END HEADER" << "\n";
    outputFile << "\n";


    // write sections (=z axis?)

    for (int k=0; k<grid[2]; k++) { //
        outputFile << "SECTION     " << ofToString(k, 3, ' ') << "\n";
        outputFile << "\n";
        for (int i=0; i<grid[0]; i++) {
            // lines
            for (int j=0; j<grid[1]; j++) {
                // 4 characters per number
                string v = "wrmw";
                if (space[i][j][k]>0) v = "1qlB";
                outputFile << v;
                if ( ((j+1)%20==0) ) outputFile << "\n";
            }
            outputFile << "\n";
        }
        outputFile << "\n";
    }

    outputFile.close();
    cout << "file closed" << endl;
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
    if (key=='p') {
        // save a screenshot
        ofImage img;
        img.grabScreen(0,0,ofGetWidth(), ofGetHeight());
        string fileName = "screenshots/voxel2na4_"+ofGetTimestampString()+".png";
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
