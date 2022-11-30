#include "Shader.h"

using namespace std;

bool Shader::validateShaderSourceByShaderIndex(int shaderIndex) {
	int success;
	char infoLog[512];
	glGetShaderiv(shaderIndex, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(shaderIndex, 512, NULL, infoLog);
		cout << "ERROR::SHADER::VERTEX\n" << infoLog << endl;
		return false;
	}
	return true;
}

unsigned int Shader::readAndCompileShaderByFilename(const char* filename, unsigned int shaderType) {
	//read and compile the vertex shader
	const char* shaderSource = Utility::textFileRead(filename);
	unsigned int shader = glCreateShader(shaderType);
	glShaderSource(shader, 1, &shaderSource, NULL);
	glCompileShader(shader);
	if (validateShaderSourceByShaderIndex(shader)) {
		return shader;
	}
	return -1;
}

unsigned int Shader::loadShaderSourceByFilename(const char* vertexShaderPath, const char* fragmentShaderPath) {
	//read and compile the vertex shader 
	unsigned int vertexShader = readAndCompileShaderByFilename(vertexShaderPath, GL_VERTEX_SHADER);

	//read and compile the fragment shader
	unsigned int fragmentShader = readAndCompileShaderByFilename(fragmentShaderPath, GL_FRAGMENT_SHADER);

	unsigned int shaderProgram;
	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	bool success = validateShaderSourceByShaderIndex(shaderProgram);

	//Once linked them into program object, we no longer need them anymore.
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	return shaderProgram;
}

int Shader::getVariableLocation(const char* name) const{
	int location = glGetUniformLocation(ID, name);
	if (location == -1)
		printf("ERROR: query %s failed!\n",name);
	return location;
}


Shader::Shader(const char* vertexPath, const char* fragmentPath) {
	ID = loadShaderSourceByFilename(vertexPath, fragmentPath);
}

//use/activate shader
void Shader::use() {
	glUseProgram(ID);
}
void Shader::setValue(const char* name, bool value) const{
	glUniform1i(getVariableLocation(name), (int)value);
}

void Shader::setValue(const char* name, int value) const {
	glUniform1i(getVariableLocation(name), (int)value);

}
void Shader::setValue(const char* name, float value) const {
	glUniform1f(getVariableLocation(name), value);
}

void Shader::setValue(const char* name, float value_x, float value_y, float value_z) const {
	glUniform3f(getVariableLocation(name),value_x,value_y,value_z);
}
void Shader::setValue(const char* name, float value_x, float value_y, float value_z, float value_w) const {
	glUniform4f(getVariableLocation(name),value_x,value_y,value_z,value_w);
}

void Shader::setValue(const char* name, glm::mat4& matrix) const {
	glUniformMatrix4fv(getVariableLocation(name), 1, GL_FALSE, glm::value_ptr(matrix));
	//the second argument tells OpenGL how many matrices we'd like to send
}