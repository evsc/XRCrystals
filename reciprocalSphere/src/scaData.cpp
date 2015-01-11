

#include "scaData.h"

scaData::scaData() {

	minH = 0;
	maxH = 0;
	minK = 0;
	maxK = 0;
	minL = 0;
	maxL = 0;

	minIntensity = 0;
	maxIntensity = 0;
	minSD = 0;
	maxSD = 0;

	unitCellDimension = ofVec3f(1,1,1);
	unitCellRotation = ofVec3f(0,0,0);
	spaceGroup = 1;
}

scaData::~scaData() {
}

void scaData::parseFile(string fileName, scaFormat format) {

	cout << "scaData parser" << endl << "----------------------" << endl;
	cout << "file\t\t<" << fileName << ">" << endl;

	ofBuffer file = ofBufferFromFile(fileName);
	// cout << file.getText();


	int cnt = 0;
	string line;

	switch (format) {
		case SCA_FIRST: 
			// nothing of consequence in the first two lines
			line = file.getFirstLine();
			line = file.getNextLine();

			// get Unit Cell, Space Group data in 3rd line
			line = file.getNextLine();
			unitCellDimension = ofVec3f(ofToFloat(line.substr(0,10)), ofToFloat(line.substr(10,10)), ofToFloat(line.substr(20,10)));
			unitCellRotation = ofVec3f(ofToFloat(line.substr(30,10)), ofToFloat(line.substr(40,10)), ofToFloat(line.substr(50,10)));
			spaceGroup = ofToInt(line.substr(61,10));

			cout << "format \t\tSCA_FIRST" << endl;
			cout << "unit cell" << endl;
			cout << "\t dimension\t" << unitCellDimension.x << " " << unitCellDimension.y << " " << unitCellDimension.z << endl;
			cout << "\t rotation\t" << unitCellRotation.x << " " << unitCellRotation.y << " " << unitCellRotation.z << endl;
			cout << "\t space group \t" << spaceGroup << endl;

			while (!file.isLastLine()) {

				line = file.getNextLine();
				cnt++;

				scaItem item;
				item.h = ofToInt(line.substr(0,4));
				item.k = ofToInt(line.substr(4,4));
				item.l = ofToInt(line.substr(8,4));
				item.intensity = ofToFloat(line.substr(12,8));
				item.sd = ofToFloat(line.substr(20,8));

				// cout << item.h << "\t" << item.k << "\t" << item.l << "\t" << item.intensity << "\t" << item.sd << endl;
			
				minH = min(minH, item.h);
				maxH = max(maxH, item.h);
				minK = min(minK, item.k);
				maxK = max(maxK, item.k);
				minL = min(minL, item.l);
				maxL = max(maxL, item.l);

				minIntensity = min(minIntensity, item.intensity);
				maxIntensity = max(maxIntensity, item.intensity);
				minSD = min(minSD, item.sd);
				maxSD = max(maxSD, item.sd);


				data.push_back(item);
			}
			break;

		case SCA_MAIN:

			cout << "format \t\tSCA_MAIN" << endl;

			unitCellDimension = ofVec3f(1,1,1);
			unitCellRotation = ofVec3f(90,90,90);

			while (!file.isLastLine()) {

				line = file.getNextLine();
				cnt++;

				scaItem item;
				item.h = ofToInt(line.substr(3,4));
				item.k = ofToInt(line.substr(7,4));
				item.l = ofToInt(line.substr(11,4));
				item.intensity = ofToFloat(line.substr(19,8));
				item.sd = ofToFloat(line.substr(31,8));
				item.phase = ofToFloat(line.substr(43,7));

				// cout << item.h << "\t" << item.k << "\t" << item.l << "\t" << item.intensity << "\t" << item.phase << endl;
			
				minH = min(minH, item.h);
				maxH = max(maxH, item.h);
				minK = min(minK, item.k);
				maxK = max(maxK, item.k);
				minL = min(minL, item.l);
				maxL = max(maxL, item.l);

				minIntensity = min(minIntensity, item.intensity);
				maxIntensity = max(maxIntensity, item.intensity);
				minSD = min(minSD, item.sd);
				maxSD = max(maxSD, item.sd);


				data.push_back(item);
			}
			break;
	}











	cout << "data objects\t" << cnt << endl;
	cout << "H \t\tmin: " << minH << "\t\tmax: " << maxH << endl;
	cout << "K \t\tmin: " << minK << "\t\tmax: " << maxK << endl;
	cout << "L \t\tmin: " << minL << "\t\tmax: " << maxL << endl;
	cout << "Intensity \tmin: " << minIntensity << "\tmax: " << maxIntensity << endl;
	cout << "Std Dev \tmin: " << minSD << "\t\tmax: " << maxSD << endl;
	cout << "----------------------" << endl;

}