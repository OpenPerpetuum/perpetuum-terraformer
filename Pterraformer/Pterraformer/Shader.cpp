#include "stdafx.h"
#include "Shader.h"

#include <cstdio>
#include <fstream>
#include <iostream>

std::string Shader::LoadShaderFile(std::string strFile)
{
	// Open the file passed in
	std::ifstream fin(strFile.c_str());

	// Make sure we opened the file correctly
	if (!fin)
		return "";

	std::string strLine = "";
	std::string strText = "";

	// Go through and store each line in the text file within a "string" object
	while (getline(fin, strLine))
	{
		strText = strText + "\n" + strLine;
	}

	// Close our file
	fin.close();

	// Return the text file's data
	return strText;
}


// This function loads a vertex and fragment shader file and creates them
void Shader::Initialize(std::string strVertexFile, std::string strFragmentFile)
{
	std::string strVShader, strFShader;

	if (!strVertexFile.length() || !strFragmentFile.length())
		return;

	if (VertexShaderId || FragmentShaderId || ShaderProgramId)
		Destroy();
	
	VertexShaderId = glCreateShader(GL_VERTEX_SHADER);
	FragmentShaderId = glCreateShader(GL_FRAGMENT_SHADER);

	strVShader = LoadShaderFile(strVertexFile.c_str());
	strFShader = LoadShaderFile(strFragmentFile.c_str());

	const char *szVShader = strVShader.c_str();
	const char *szFShader = strFShader.c_str();

	glShaderSource(VertexShaderId, 1, &szVShader, nullptr);
	glShaderSource(FragmentShaderId, 1, &szFShader, nullptr);

	glCompileShader(VertexShaderId);
	glCompileShader(FragmentShaderId);
	
	int success = true;
	char infoLog[512];

	glGetShaderiv(VertexShaderId, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(VertexShaderId, 512, nullptr, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	glGetShaderiv(FragmentShaderId, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(FragmentShaderId, 512, nullptr, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	// Next we create a program object to represent our shaders
	ShaderProgramId = glCreateProgram();

	// We attach each shader we just loaded to our program object
	glAttachShader(ShaderProgramId, VertexShaderId);
	glAttachShader(ShaderProgramId, FragmentShaderId);

	// Our last init function is to link our program object with OpenGL
	glLinkProgram(ShaderProgramId);
}

// This function returns a variable Id for a shader variable.  This is used
// when we need to pass in variables into a shader like numbers and matrices.
GLint Shader::GetVariable(std::string strVariable)
{
	if (!ShaderProgramId)
		return -1;

	return glGetUniformLocation(ShaderProgramId, strVariable.c_str());
}


void Shader::Destroy()
{
	if (VertexShaderId)
	{
		glDetachShader(ShaderProgramId, VertexShaderId);
		glDeleteShader(VertexShaderId);
		VertexShaderId = 0;
	}

	if (FragmentShaderId)
	{
		glDetachShader(ShaderProgramId, FragmentShaderId);
		glDeleteShader(FragmentShaderId);
		FragmentShaderId = 0;
	}

	if (ShaderProgramId)
	{
		glDeleteShader(ShaderProgramId);
		ShaderProgramId = 0;
	}
}

