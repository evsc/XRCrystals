

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
}

scaData::~scaData() {
}

void scaData::parseFile(string fileName) {

	cout << "scaData parser" << endl << "----------------------" << endl;
	cout << "file\t\t<" << fileName << ">" << endl;

	ofBuffer file = ofBufferFromFile(fileName);
	// cout << file.getText();

	string line1 = file.getFirstLine();
	string line2 = file.getNextLine();
	string line3 = file.getNextLine();

	// cout << "line 1 = " << line1 << endl;
	// cout << "line 2 = " << line2 << endl;
	cout << "unit cell\t" << "|" << line3 << endl;

	int cnt = 0;

	while (!file.isLastLine()) {

		string line = file.getNextLine();
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

	cout << "data objects\t" << cnt << endl;
	cout << "H \t\tmin: " << minH << "\t\tmax: " << maxH << endl;
	cout << "K \t\tmin: " << minK << "\t\tmax: " << maxK << endl;
	cout << "L \t\tmin: " << minL << "\t\tmax: " << maxL << endl;
	cout << "Intensity \tmin: " << minIntensity << "\tmax: " << maxIntensity << endl;
	cout << "Std Dev \tmin: " << minSD << "\t\tmax: " << maxSD << endl;
	cout << "----------------------" << endl;

}