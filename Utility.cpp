#include "Utility.h"

char* Utility::textFileRead(const char* fn) {
	FILE* fp;
	char* content = NULL;
	int cnt = 0;
	if (fn != NULL) {
		fopen_s(&fp, fn, "rt");
		if (fp != NULL) {
			fseek(fp, 0, SEEK_END);
			cnt = ftell(fp);
			rewind(fp);
			if (cnt > 0) {
				content = (char*)malloc(sizeof(char) * (cnt + 1));
				cnt = fread(content, sizeof(char), cnt, fp);
				content[cnt] = '\0';
			}
			fclose(fp);
		}
	}
	return content;
}

bool Utility::dirExists(const std::string& dirName_in)
{
	DWORD ftyp = GetFileAttributesA(dirName_in.c_str());
	if (ftyp == INVALID_FILE_ATTRIBUTES)
	{
		DWORD dwError = GetLastError();
		if (dwError == ERROR_FILE_NOT_FOUND)
		{
			return false;// file not found
		}
		else if (dwError == ERROR_PATH_NOT_FOUND)
		{
			return false;// path not found
		}
		else if (dwError == ERROR_ACCESS_DENIED)
		{
			return false;// file or directory exists, but access is denied
		}
		else
		{
			return false;// some other error has occured
		}
	}
	if (ftyp & FILE_ATTRIBUTE_DIRECTORY)
		return true;   // this is a directory!

	return false;    // this is not a directory!
}
bool Utility::fileExists(const std::string& fileName_in)
{
	DWORD ftyp = GetFileAttributesA(fileName_in.c_str());
	if (ftyp == INVALID_FILE_ATTRIBUTES)
	{
		DWORD dwError = GetLastError();
		if (dwError == ERROR_FILE_NOT_FOUND)
		{
			return false;// file not found
		}
		else if (dwError == ERROR_PATH_NOT_FOUND)
		{
			return false;// path not found
		}
		else if (dwError == ERROR_ACCESS_DENIED)
		{
			return false;// file or directory exists, but access is denied
		}
		else
		{
			return false;// some other error has occured
		}
	}

	if (ftyp & FILE_ATTRIBUTE_DIRECTORY)
		return false;   // this is a directory!

	return true;    // this is not a directory!
}

float* Utility::floatArrayRead(const std::string& filePath, int& arrayLength) {
	float* floatArray;
	FILE* pFile;
	pFile = fopen(filePath.c_str(), "r");
	int p = 0;
	if (pFile == NULL)
	{
		printf("Open filefailure!");
	}
	else {
		rewind(pFile);
		if (fscanf(pFile, "%d", &arrayLength) == EOF) {
			std::cout << "Array length missing!" << std::endl;
			return NULL;
		}
		floatArray = new float[arrayLength];
		for (int i = 0; i < arrayLength; ++i) {
			if (fscanf(pFile, "%f,", &floatArray[p++]) == EOF) {
				std::cout << "Array format error!" << std::endl;
				return NULL;
			}
		}
	}
	fclose(pFile);
	return floatArray;
}

int* Utility::intArrayRead(const std::string& filePath, int& arrayLength) {
	int* intArray;
	FILE* pFile;
	pFile = fopen(filePath.c_str(), "r");
	int p = 0;
	if (pFile == NULL)
	{
		printf("Open filefailure!");
	}
	else {
		rewind(pFile);
		if (fscanf(pFile, "%d", &arrayLength) == EOF) {
			std::cout << "Array length missing!" << std::endl;
			return NULL;
		}
		intArray = new int[arrayLength];
		for (int i = 0; i < arrayLength; ++i) {
			if (fscanf(pFile, "%d,", &intArray[p++]) == EOF) {
				std::cout << "Array format error!" << std::endl;
				return NULL;
			}
		}
	}
	fclose(pFile);
	return intArray;
}


