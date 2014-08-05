

#include "na4Data.h"

na4Data::na4Data() {

	grid[0] = 10;
	grid[1] = 10;
	grid[2] = 10;

	lim[0] = 9;
	lim[1] = 9;
	lim[2] = 9;

	spacegroup = 1;

	mode = 1;

	cell_dim = ofVec3f(1,1,1);
	cell_rot = ofVec3f(90,90,90);

}

na4Data::~na4Data() {

	// delete pointer array
	// 	for ( int i=0; i<num_rows; i++ )  
	// {  
	//   delete[] array_of_arrays[i];  
	// }  
	// delete[] array_of_arrays; 
	// }


}



void na4Data::parseFile( string fileName ) {

	cout << "na4Data parser" << endl << "----------------------" << endl;
	cout << "file\t\t<" << fileName << ">" << endl;

	ofBuffer file = ofBufferFromFile(fileName);
	// cout << file.getText();


	int cnt = 0;
	string line;


	// parse HEADER:

	line = file.getFirstLine();
	// 1 MAPTONA4 HEADER

	line = file.getNextLine();
	// 2 TITLE

	line = file.getNextLine();
	// 3 From clipper Xmap  

	line = file.getNextLine();
	// 4 AXIS           Z       X       Y

	line = file.getNextLine();
	// 5 GRID         108     108      56
	grid[0] = ofToInt(line.substr(8,8));
	grid[1] = ofToInt(line.substr(16,8));
	grid[2] = ofToInt(line.substr(24,8));


	line = file.getNextLine();
	// 6 XYZLIM         0      54       0      53       0      28
	lim[0] = ofToInt(line.substr(16,8));
	lim[1] = ofToInt(line.substr(32,8));
	lim[2] = ofToInt(line.substr(48,8));


	line = file.getNextLine();
	// 7 SPACEGROUP            96
	spacegroup = ofToInt(line.substr(16,8));


	line = file.getNextLine();
	// 8 MODE           2
	mode = ofToInt(line.substr(8,8));


	line = file.getNextLine();
	// 9 CELL        78.733    78.733    36.859    90.000    90.000    90.000
	cell_dim = ofVec3f( ofToFloat(line.substr(8,10)), ofToFloat(line.substr(18,10)), ofToFloat(line.substr(28,10)) );
	cell_rot = ofVec3f(  ofToFloat(line.substr(38,10)), ofToFloat(line.substr(48,10)), ofToFloat(line.substr(58,10)) );


	line = file.getNextLine();
	// 10 RHOLIM      -1.61241         2.74221        0.114427E-03    0.454045    
	line = file.getNextLine();
	// 11 INDXR4     0  21
	line = file.getNextLine();
	// 12 END HEADER



	// .. .empty
	// SECTION       0   ... up to grid[1] sections
	// .. empty
	// 2kRw1qlB0JU-00RL0jB50viB02Wtz0QZyWH7 ... up to grid[2] letters*4
	// ... up to grid[0] lines


	line = file.getNextLine();
	// 


	// map = new float[ grid[0] * grid[1] * grid[2] ];

	map = new float**[lim[0]];  
	for ( int i=0; i<lim[0]; i++ ) {  
	  map[i] = new float*[lim[1]];  
	  for ( int j=0; j<lim[1]; j++ ) {
	  	map[i][j] = new float[lim[2]];
	  }
	}   

	map[0][0][0] = 3.3;

	int section = 0;

	while (section <= grid[1]) {
		line = file.getNextLine();
		// SECTION       0
		section = ofToInt(line.substr(8,7));
		
		line = file.getNextLine();
		// .. empty
		cout << "section \t" << section << endl;

		int row = 0;
		while (true) {
			// first line
			line = file.getNextLine();

			// second line
			line += file.getNextLine();
			
			// cout << "get line " << line.length() << " in row " << row << endl;
			// map[0][section][0] = 1.f;

			row++;
			
			// break when empty line
			if (line.length() < 5) {
				cout << "line break " << endl;
				break;
			}
		}

		if (file.isLastLine()) break;

	}


	cout << "grid \t\t" << grid[0] << "\t" << grid[1] << "\t" << grid[2] << endl;
	cout << "limit \t\t" << lim[0] << "\t" << lim[1] << "\t" << lim[2] << endl;
	cout << "spacegroupt \t" << spacegroup << endl;
	cout << "mode \t\t" << mode << endl;
	cout << "cell dim \t" << cell_dim.x << "\t" << cell_dim.y << "\t" << cell_dim.z << endl;
	cout << "cell rot \t" << cell_rot.x << "\t" << cell_rot.y << "\t" << cell_rot.z << endl;
	cout << "sections \t" << section << endl;

}