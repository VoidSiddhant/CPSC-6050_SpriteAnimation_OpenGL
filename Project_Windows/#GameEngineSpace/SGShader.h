#ifndef _SG_SHADER_H
#define _SG_SHADER_H

#include<SGUtility.h>
#include<map>
class SGShader
{
public:
	SGShader(const char* vertexFile, const char* fragmentFile);
	SGShader(const SGShader& shader);
	~SGShader();
	void Use();
	void SetMatrix4(const char* id, SGMat4 mat4);
	void SetUniform3f(const char* id, SGVector3 inVector);
	void SetUniform1f(const char* id, float value);
	void SetUniformBool(const char* id, bool value);
	void ActiveTexture();
	void AddTexture(const GLenum& target, const unsigned int& texture,
					const char* textureID, const int& inShaderIndex);
private:
	unsigned int shaderProgram;
	std::map<GLenum, unsigned int> mapTextures;
	const char* vertexFile, *fragmentFile;
};

#endif // !_SG_SHADER_H