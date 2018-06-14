#pragma once

#include <string>
#include <glad/glad.h>

class Shader
{
public:
	Shader() { }
	~Shader() { Destroy(); }

	std::string LoadShaderFile(std::string strFile);
	void Initialize(std::string strVertexFile, std::string strFragmentFile);

	// This returns an ID for a variable in our shader, to be used with a Set*() function
	GLint GetVariable(std::string strVariable);

	// Below are functions to set an integer, a set of floats or a matrix (float[16])
	void SetInt(GLint id, int newValue) { glUniform1i(id, newValue); }
	void SetFloat(GLint id, GLfloat newValue) { glUniform1f(id, newValue); }
	void SetFloat2(GLint id, GLfloat v0, GLfloat v1) { glUniform2f(id, v0, v1); }
	void SetFloat3(GLint id, GLfloat v0, GLfloat v1, GLfloat v2) { glUniform3f(id, v0, v1, v2); }
	void SetFloat4(GLint id, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3) { glUniform4f(id, v0, v1, v2, v3); }

	void SetUniform(const GLchar* uniformName, const glm::vec3& v)
	{
		glUniform3fv(GetVariable(uniformName), 1, &v[0]);
	}

	// This allows us to pass in 4x4 matrix (float array of 16) to a shader by the Id received from GetVariable()
	void SetMatrix4(GLint id, GLsizei count, GLboolean transpose, const GLfloat *value)
	{
		glUniformMatrix4fv(id, count, transpose, value);
	}

	void SetMatrix3(GLint id, GLsizei count, GLboolean transpose, const GLfloat *value)
	{
		glUniformMatrix3fv(id, count, transpose, value);
	}

	void TurnOn() { glUseProgram(ShaderProgramId); }
	void TurnOff() { glUseProgram(0); }

	void Destroy();

private:
	GLuint VertexShaderId, FragmentShaderId, ShaderProgramId;
};