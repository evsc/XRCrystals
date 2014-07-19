

#include "scaData.h"

scaData::scaData() {
}

scaData::~scaData() {
}

void scaData::parseFile(string fileName) {

	ofBuffer file = ofBufferFromFile(fileName);
	cout << file.getText();

}