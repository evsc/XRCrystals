// parse .na4 data 
// produced form electron-density map file
// with program maptona4 of ccp4 suite


#pragma once

#include "ofMain.h"


class na4Data {

public:

	na4Data();
	~na4Data();

	void parseFile( string fileName );

	int char4ToInt(string c4);
	float intToReal(int v);


	int grid[3];
	int sections;
	int cols;
	int rows;

	int spacegroup;

	int mode;

	ofVec3f cell_dim;
	ofVec3f cell_rot;


	float*** map;


};
