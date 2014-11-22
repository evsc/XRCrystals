#pragma once

#include "ofMain.h"


typedef struct {
    ofVec3f pos;
    float intensity;
} voxel;

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


        int grid[3];    // size of our overall voxel grid
        float*** space;   // the actual voxel grid

        std::vector< voxel > voxels;

        float valSolid;
        float valEmpty;

		void parseVoxelFile(string voxelfile);
		void saveOutNa4(string na4file);


		bool drawAxis;
		ofEasyCam cam;
		float scale;

		ofFile outputFile;

};
