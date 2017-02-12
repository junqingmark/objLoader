#include "shader.h"
#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <cstdio>


GLuint LoadShaders(const char * vertex_file_path,const char * fragment_file_path)
{
	//create shaders
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

	//read vertex shader from file
	std::string vertexShaderCode;
	std::ifstream vertexShaderStream(vertex_file_path, std::ios::in);
	if(vertexShaderStream.is_open())
	{
		std::string line = "";
		while(std::getline(vertexShaderStream, line))
		{
			vertexShaderCode += "\n" + line;
		}

		vertexShaderStream.close();
	}
	else
	{
		std::cerr << "fail to open " << vertex_file_path << std::endl;
		return 0;
	}

	//read fragment shader from file
	std::string fragmentShaderCode;
	std::ifstream fragmentShaderStream(fragment_file_path, std::ios::in);
	if(fragmentShaderStream.is_open())
	{
		std::string line = "";
		while(std::getline(fragmentShaderStream, line))
		{
			fragmentShaderCode += "\n" + line ;
		}

		fragmentShaderStream.close();
	}
	else
	{
		std::cerr << "fail to open " << fragment_file_path << std::endl;
		return 0;
	}

	GLint result = GL_FALSE;
	int infoLogLength;

	//compile vertex shader
	printf("Compiling shader: %s\n", vertex_file_path);
	const char* vertexPointer = vertexShaderCode.c_str();
	glShaderSource(vertexShader, 1, &vertexPointer, NULL);
	glCompileShader(vertexShader);

	//check vertex shader
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &result);
	glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &infoLogLength);
	if(infoLogLength > 0)
	{
		std::vector<char> vertexShaderErrorMessage(infoLogLength + 1);
		glGetShaderInfoLog(vertexShader, infoLogLength, NULL, &vertexShaderErrorMessage[0]);
		printf("%s\n", &vertexShaderErrorMessage[0]);
	}

	//compile fragment shader
	printf("Compiling shader: %s\n", fragment_file_path);
	const char* fragmentPointer = fragmentShaderCode.c_str();
	glShaderSource(fragmentShader, 1, &fragmentPointer, NULL);
	glCompileShader(fragmentShader);

	//check vertex shader
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &result);
	glGetShaderiv(fragmentShader, GL_INFO_LOG_LENGTH, &infoLogLength);
	if(infoLogLength > 0)
	{
		std::vector<char> fragmentShaderErrorMessage(infoLogLength + 1);
		glGetShaderInfoLog(fragmentShader, infoLogLength, NULL, &fragmentShaderErrorMessage[0]);
		printf("%s\n", &fragmentShaderErrorMessage[0]);
	}

	//link the program
	printf("link program\n");
	GLuint program = glCreateProgram();
	glAttachShader(program, vertexShader);
	glAttachShader(program, fragmentShader);
	glLinkProgram(program);

	//check link status
	glGetShaderiv(program, GL_LINK_STATUS, &result);
	glGetShaderiv(program, GL_INFO_LOG_LENGTH, &infoLogLength);
	if(infoLogLength > 0)
	{
		std::vector<char> linkErrorMessage(infoLogLength + 1);
		glGetShaderInfoLog(program, infoLogLength, NULL, &linkErrorMessage[0]);
		printf("%s\n", &linkErrorMessage[0]);
	}

	return program;



}