#pragma once
#include <cstdio>
#include <atomic>
#include <string>
#include <io.h>
#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include <windows.h>
#include <fstream>

#include <unordered_map>


class Utility
{
public:
	static char* textFileRead(const char* fn);
	static bool dirExists(const std::string& dirName_in);
	static bool fileExists(const std::string& fileName_in);
	static float* floatArrayRead(const std::string& filePath, int& arrayLength);
	static int* intArrayRead(const std::string& filePath, int& arrayLength);
};

