#pragma once
#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>


class shader
{
public:
	unsigned int ID;	//stores program ID
	shader(const char* vertexPath, const char* fragmentPath);	//constructor for reading and building of the shader
	void use();	//used to activate the shader

	//uniform utilities
	void setBool(const std::string& name, bool value) const;
	void setInt(const std::string& name, int value) const;
	void setFloat(const std::string& name, float value) const;
};

#endif // !SHADER_H

