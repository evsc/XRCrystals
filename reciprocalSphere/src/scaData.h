// parse .sca data 
// Scalepak format, reflection data


// header
// [0]
// [1]
// [2] unit cell dimension X Y Z | unit cell angle X Y Z | space group

// body
// [3] H K L intensity standard-dev
// .
// .



#pragma once

#include "ofMain.h"


// different formatting styles in .sca files
enum scaFormat {
	SCA_FIRST,
	SCA_MAIN,
	SCA_TNT,
	SCA_REFMAC
};


typedef struct {

    int h;
    int k;
    int l;
    float intensity;
    float sd;	// standard deviation
    float phase;

} scaItem;




class scaData {

public:

	scaData();
	~scaData();

	vector < scaItem > data;

	int minH, maxH;
	int minK, maxK;
	int minL, maxL;

	float minIntensity, maxIntensity;
	float minSD, maxSD;

	ofVec3f unitCellDimension;
	ofVec3f unitCellRotation;

	string spaceGroup;

	void parseFile( string fileName, scaFormat format = SCA_MAIN );


};