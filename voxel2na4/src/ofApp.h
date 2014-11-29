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

		string fileName;
		string templateName;

        int grid[3];    // size of our overall voxel grid
        int offset[3];	// placement of model offset in grid
        float scalePrimitive[3];
        float*** space;   // the actual voxel grid

        float minX;
        float maxX;
        float minY;
        float maxY;
        float minZ;
        float maxZ;

        float cutOffX;
	    float cutOffY;
	    float cutOffZ;
	    float gridStep;

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
