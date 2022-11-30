#pragma once
#include <string>
#include <json/json.h>
#include <iostream>
#include <fstream>
class JsonManipulation
{
public:
private:
	Json::Value jsonData;
	Json::Reader reader;
	std::string jsonFilePath="";
private:
	void updateJsonData();
public:

	JsonManipulation(const std::string& jsonFilePath);

	virtual ~JsonManipulation();
	Json::Value& getJsonData(const std::string& jsonFilePath = "");
};

