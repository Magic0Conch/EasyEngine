#include "include/JsonManipulation.h"

void JsonManipulation::updateJsonData() {
	std::ifstream in(jsonFilePath, std::ios::binary);
	if (!in.is_open()) {
		std::cout << "Can't find the file path: " << jsonFilePath << std::endl;
	}
	if (reader.parse(in, jsonData)) {
		in.close();
	}
	else {
		std::cout << "Json from the file path: " << jsonFilePath << " format error!" << std::endl;
	}
}
JsonManipulation::JsonManipulation(const std::string& jsonFilePath) {
	this->jsonFilePath = jsonFilePath;
	updateJsonData();
}

JsonManipulation::~JsonManipulation() {

}
Json::Value& JsonManipulation::getJsonData(const std::string& jsonFilePath) {
	if (jsonFilePath == "") {
		return jsonData;
	}
	this->jsonFilePath = jsonFilePath;
	updateJsonData();
	return jsonData;
}