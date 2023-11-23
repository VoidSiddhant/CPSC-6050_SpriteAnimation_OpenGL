#include "SGShader.h"

using namespace std;

SGShader::SGShader(const char* vertexFile, const char* fragmentFile)
{
	this->vertexFile = vertexFile;
	this->fragmentFile = fragmentFile;

	string vertexCode, fragmentCode;
	ifstream vShaderFile, fShaderFile;

	try {
		stringstream vStream, fStream;

		vShaderFile.open(vertexFile);
		fShaderFile.open(fragmentFile);
		vStream << vShaderFile.rdbuf();
		fStream << fShaderFile.rdbuf();

		vShaderFile.clear();
		vShaderFile.close();
		fShaderFile.clear();
		fShaderFile.close();

		vertexCode = vStream.str();
		fragmentCode = fStream.str();

		vStream.clear();
		fStream.clear();
		
	}
	catch (ifstream::failure e)
	{
		cout << "ERROR READING SHADER FILES : " << endl;
	}

	// SETUP SHADER 

	// Create Vertex Shader with ID
	unsigned int vertexShader, vertexShaderLight;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	//Set and Compile the ID
	const char* vc = vertexCode.c_str();
	glShaderSource(vertexShader, 1, &vc, NULL);
	glCompileShader(vertexShader);

	//Fragment shader ID
	unsigned int fragmentShader, fragmentShaderLight;
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	//Set and Compile ID
	const char* fc = fragmentCode.c_str();
	glShaderSource(fragmentShader, 1, &fc, NULL);
	glCompileShader(fragmentShader);

	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);


	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}

SGShader::SGShader(const SGShader& shader)
{
	this->vertexFile = shader.vertexFile;
	this->fragmentFile = shader.fragmentFile;

	string vertexCode, fragmentCode;
	ifstream vShaderFile, fShaderFile;

	try {
		stringstream vStream, fStream;

		vShaderFile.open(shader.vertexFile);
		fShaderFile.open(shader.fragmentFile);
		vStream << vShaderFile.rdbuf();
		fStream << fShaderFile.rdbuf();

		vShaderFile.clear();
		vShaderFile.close();
		fShaderFile.clear();
		fShaderFile.close();

		vertexCode = vStream.str();
		fragmentCode = fStream.str();

		vStream.clear();
		fStream.clear();

	}
	catch (ifstream::failure e)
	{
		cout << "ERROR READING SHADER FILES : " << endl;
	}

	// SETUP SHADER 

	// Create Vertex Shader with ID
	unsigned int vertexShader, vertexShaderLight;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	//Set and Compile the ID
	const char* vc = vertexCode.c_str();
	glShaderSource(vertexShader, 1, &vc, NULL);
	glCompileShader(vertexShader);

	//Fragment shader ID
	unsigned int fragmentShader, fragmentShaderLight;
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	//Set and Compile ID
	const char* fc = fragmentCode.c_str();
	glShaderSource(fragmentShader, 1, &fc, NULL);
	glCompileShader(fragmentShader);

	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);


	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	auto iter = shader.mapTextures.begin();
	while (iter != shader.mapTextures.end())
	{
		mapTextures.insert(std::make_pair(iter->first, iter->second));
		iter++;
	}
}

void SGShader::SetMatrix4(const char* id, SGMat4 mat4)
{
	glUniformMatrix4fv(glGetUniformLocation(shaderProgram, id),1, GL_FALSE, glm::value_ptr(mat4));
}

void SGShader::SetUniform3f(const char* id, SGVector3 inVector)
{
	glUniform3f(glGetUniformLocation(shaderProgram, id), inVector.x, inVector.y, inVector.z);
}

void SGShader::SetUniform1f(const char* id, float value)
{
	glUniform1f(glGetUniformLocation(shaderProgram, id), value);
}

void SGShader::SetUniformBool(const char* id, bool value)
{
	glUniform1i(glGetUniformLocation(shaderProgram, id), value);
}

void SGShader::ActiveTexture()
{
	auto iter = mapTextures.begin();
	while (iter != mapTextures.end())
	{
		glActiveTexture(iter->first);
		glBindTexture(GL_TEXTURE_2D, iter->second);
		iter++;
	}
}

void SGShader::AddTexture(const GLenum& target, const unsigned int& texture, const char* textureID, const int& inShaderIndex)
{
	mapTextures.insert(std::make_pair(target, texture));
	this->Use();
	glUniform1i(glGetUniformLocation(shaderProgram, textureID), inShaderIndex);
}

void SGShader::Use()
{
	glUseProgram(shaderProgram);
}

SGShader::~SGShader()
{
	glDeleteProgram(shaderProgram);

}