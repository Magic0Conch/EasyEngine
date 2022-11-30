//#ifndef SHADER_H
//#define SHADER_H
//#endif 
#pragma once
#include<glad/glad.h>
#include "Utility.h"
#include <iostream>
#include <string>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
class Shader
{
public:
	//shaderProgram ID
	unsigned int ID;

	//read and builds the shader	
	Shader(const char*, const char*);

	//use/activate shader
	void use();
	void setValue(const char*, bool value) const;
	void setValue(const char*, int value) const;
	void setValue(const char*, float value) const;
	void setValue(const char*, float value_x,float value_y,float value_z) const;
	void setValue(const char*, float,float,float,float) const;
	void setValue(const char*, glm::mat4&) const;

private:
	int getVariableLocation(const char*) const;
	static bool validateShaderSourceByShaderIndex(int shaderIndex);
	static unsigned int readAndCompileShaderByFilename(const char* filename, unsigned int shaderType);
public:
	static unsigned int loadShaderSourceByFilename(const char* vertexShaderPath, const char* fragmentShaderPath);

};

