

#include "na4Data.h"

na4Data::na4Data() {

	grid[0] = 10;
	grid[1] = 10;
	grid[2] = 10;

	sections = 1;
	rows = 1;
	cols = 1;

	spacegroup = 1;

	mode = 1;

	cell_dim = ofVec3f(1,1,1);
	cell_rot = ofVec3f(90,90,90);


	scaleValue = 0.00001;
	minValue = 0;
	maxValue = 0;

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
	rows = ofToInt(line.substr(16,8))+1;
	sections = ofToInt(line.substr(32,8))+1;
	cols = ofToInt(line.substr(48,8))+1;

	cout << "rows \t" << rows << endl;
	cout << "sections \t" << sections << endl;
	cout << "cols \t" << cols << endl;


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


	// structure of upcoming map data:

	// .. empty
	// SECTION       0   ... up to grid[1] sections
	// .. empty
	// 2kRw1qlB0JU-00RL0jB50viB02Wtz0QZyWH7 ... up to grid[2] letters*4
	// ... up to grid[0] lines


	line = file.getNextLine();
	// .. empty


	// now we know the size of the density map, we can create our data container
	map = new float**[sections];
	for ( int i=0; i<sections; i++ ) {
	  map[i] = new float*[rows];
	  for ( int j=0; j<rows; j++ ) {
	  	map[i][j] = new float[cols];
	  }
	}

	int section = 0;

	// loop over all sections
	while (true) {

		line = file.getNextLine();
		// SECTION       0
		section = ofToInt(line.substr(8,7));
		// cout << "section \t" << section << endl;

		line = file.getNextLine();
		// .. empty

		// cout << "section " << section << endl;


		int row = 0;
		while (true) {
            // cout << "row " << row << endl;
			// first line
			line = file.getNextLine();

			if (line.length() < 5) {
				// break when empty line, because that means section is done
				break;
			} else {
				// second line completes the row data
				line += file.getNextLine();
			}

			unsigned int cp = 0;
			int col = 0;
			while (cp <= line.length()-4) {
				// take 4 characters
				string c4 = line.substr(cp,4);
				int v = char4ToInt(c4);
				float f = intToReal(v);
				// if (section == 0 && row == 0) cout << "col\t" << col << "\t" << c4 << "\t" << f << endl;
				map[section][row][col] = f;

				minValue = min(minValue, f);
				maxValue = max(maxValue, f);

				col++;
				cp+=4;
			}

			// cout << "section " << section << " line.length " << line.length() << " in row " << row << endl;
			row++;

		}

		if (file.isLastLine()) break;

	}


	cout << "grid \t\t" << grid[0] << "\t" << grid[1] << "\t" << grid[2] << endl;
	cout << "limit \t\t" << endl;
	cout << "\t[0] sections\t" << sections << endl;
	cout << "\t[1] rows\t" << rows << endl;
	cout << "\t[2] cols\t" << cols << endl;
	cout << "spacegroup \t" << spacegroup << endl;
	cout << "mode \t\t" << mode << endl;
	cout << "cell dim \t" << cell_dim.x << "\t" << cell_dim.y << "\t" << cell_dim.z << endl;
	cout << "cell rot \t" << cell_rot.x << "\t" << cell_rot.y << "\t" << cell_rot.z << endl;
	cout << "loaded \t\t" << section << " sections" << endl;
	cout << "scaleValue \t" << scaleValue << endl;
	cout << "minValue \t" << minValue << endl;
	cout << "maxValue \t" << maxValue << endl;

}


/* ***** convert 4-character string into interger ****
   according to mapping algorithm in <maptona4.f> (ccp4 program suite) */
int na4Data::char4ToInt( string c4 ) {

	string chrtab = "0123456789-+ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";

	char c;
	int index;
	int n = 0;

	// integers go up to:
	// 2**24 -1 = 16777215;

	// cout << "char4ToInt ( " << c4 << " ) \t\t";

	for (int i=0; i<4; i++) {
		c = c4[i];
		index = chrtab.find_first_of(c);
		if (index == 0) {
			// illegal??, gives a negative numbers when added up
			// cout << "0" << endl;
			// return 0;
			n = 64*n;
		} else {
			n = 64*n + index -1;
			// cout << n << " ";
		}
	}

	// cout << endl;

	return n;

}


/* ***** convert INTEGER in the range 0 to 2**24-1 to REAL.
		Negative numbers are 2's complement.
   according to mapping algorithm in <maptona4.f> (ccp4 program suite) */

float na4Data::intToReal( int v ) {

	if (v < pow(2,23.0)) {
		return v * scaleValue;
	} else {
		return (v - pow(2,24.0)) * scaleValue;
	}

}



      // IF (INDXR4(1).NE.0) THEN							if i1 != 0
      //   REALA4 = ISIGN(N, INDXR4(1)) * .5**INDXR4(2)			define sign, scale
      // ELSEIF (N.LT.2**23) THEN							else if  n < 2**23
      //   REALA4 = N * .5**INDXR4(2)							n * scale
      // else 												else
      //   REALA4 = (N - 2**24) * .5**INDXR4(2)					(n -2**24) * scale
      // ENDIF
      // END

// SIGN(A,B) returns the value of A with the sign of B.
