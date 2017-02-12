#include "objLoader.h"
#include <cstdio>
#include <vector>
#include <cstring>


bool loadOBJ(const char* path, 
				std::vector<glm::vec3>& out_vertices,
				std::vector<glm::vec2>& out_uvs,
				std::vector<glm::vec3>& out_normals)
{
	printf("load obj file %s...\n", path);

	std::vector<unsigned int> vertexIndices, uvIndices, normalIndices;
	std::vector<glm::vec3> tmp_vertices;
	std::vector<glm::vec2> tmp_uvs;
	std::vector<glm::vec3> tmp_normals;

	FILE* fp = fopen(path, "r");
	if(fp == NULL)
	{
		printf("Impossible to open the file!\n");
		return false;
	}

	char lineHeader[128];
	while(1)
	{
		int res = fscanf(fp, "%s", lineHeader);
		if(res == EOF)
		{
			break;
		}

		//parse lineHeader
		if(strcmp(lineHeader, "v") == 0)
		{
			glm::vec3 vertex;
			fscanf(fp, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z);
			tmp_vertices.push_back(vertex);
		}
		else if(strcmp(lineHeader, "vt") == 0)
		{
			glm::vec2 uv;
			fscanf(fp, "%f %f\n", &uv.x, &uv.y);
			uv.y = -uv.y;
			tmp_uvs.push_back(uv);
		}
		else if(strcmp(lineHeader, "vn") == 0)
		{
			glm::vec3 normal;
			fscanf(fp, "%f %f %f\n", &normal.x, &normal.y, &normal.z);
			tmp_normals.push_back(normal);
		}
		else if(strcmp(lineHeader, "f") == 0)
		{
			unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
			int matches = fscanf(fp, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &vertexIndex[0], &uvIndex[0], &normalIndex[0], &vertexIndex[1], &uvIndex[1], &normalIndex[1], &vertexIndex[2], &uvIndex[2], &normalIndex[2]);
			if(matches != 9)
			{
				printf("cannot parse the f part!\n");
				return false;
			}

			vertexIndices.push_back(vertexIndex[0]);
			vertexIndices.push_back(vertexIndex[1]);
			vertexIndices.push_back(vertexIndex[2]);

			uvIndices.push_back(uvIndex[0]);
			uvIndices.push_back(uvIndex[1]);
			uvIndices.push_back(uvIndex[2]);

			normalIndices.push_back(normalIndex[0]);
			normalIndices.push_back(normalIndex[1]);
			normalIndices.push_back(normalIndex[2]);


		}
		else
		{
			//eat up the comment
			char comment[1000];
			fgets(comment, 1000, fp);
		}

	}

	//for each triangle
	for(int i = 0; i < vertexIndices.size(); i++)
	{
		//get the indices of its attributes
		unsigned int vertexindex = vertexIndices[i];
		unsigned int uvindex = uvIndices[i];
		unsigned int normalindex = normalIndices[i];

		glm::vec3 vertex = tmp_vertices[vertexindex - 1];
		glm::vec2 uv = tmp_uvs[uvindex - 1];
		glm::vec3 normal = tmp_normals[normalindex - 1];

		//put the attributes in buffer
		out_vertices.push_back(vertex);
		out_uvs.push_back(uv);
		out_normals.push_back(normal);

	}
	
	return true;

}