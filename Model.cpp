#include "Model.h"

#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include <glm\glm.hpp>
#include <glm\gtx\transform.hpp>
#include <glm\gtx\euler_angles.hpp>
#include <FreeImage.h>

#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <sstream>

Model::Model()
{
	vertArr = 0;
	vertCount = 0;
}


Model::~Model()
{
}

// Sets model's buffer
bool Model::buffer(std::string objFile)
{
	// Set object and texture files
	objFilename = objFile;

	// Store information from obj files
	std::vector<glm::vec3> locs;
	std::vector<glm::vec2> uvs;
	std::vector<glm::vec3> normals;
	std::vector<VertInd> vertInds;

	std::ifstream inStream;
	std::string fileData;

	inStream.open(objFile);

	// Reads obj file
	if (inStream.is_open()) {
		while (std::getline(inStream, fileData)) {
			std::istringstream sStream(fileData);
			std::string label;
			sStream >> label;

			// Vertex locations
			if (label == "v") {
				glm::vec3 loc;

				sStream >> loc.x >> loc.y >> loc.z;

				locs.push_back(loc);

				glm::vec3 absVert = abs(loc);
				maxXYZ = glm::max(maxXYZ, absVert);
				minXYZ = glm::min(minXYZ, absVert);
			}

			// UVs
			else if (label == "vt") {
				glm::vec2 uv;

				sStream >> uv.x >> uv.y;

				uvs.push_back(uv);
			}

			// Vertex normals
			else if (label == "vn") {
				glm::vec3 norm;

				sStream >> norm.x >> norm.y >> norm.z;

				normals.push_back(norm);
			}

			// Faces/Vertex indices
			else if (label == "f") {
				for (int i = 0; i < 3; i++) {
					GLuint ind1;
					GLuint ind2;
					GLuint ind3;
					char slash;

					sStream >> ind1 >> slash >> ind2 >> slash >> ind3;

					ind1--;
					ind2--;
					ind3--;

					VertInd indices = { ind1, ind2, ind3 };

					vertInds.push_back(indices);
				}
			}


		}
		inStream.close();
	}

	vertCount = vertInds.size();

	// Collects all of the vertex buffer data
	std::vector<Vertex> vertBufData(vertCount);
	for (unsigned int i = 0; i < vertCount; i++) {
		vertBufData[i] = {
			locs[vertInds[i].locInd],
			uvs[vertInds[i].uvInd],
			normals[vertInds[i].normInd],
		};
	}

	// Begin Attribute Setup

	glGenVertexArrays(1, &vertArr);
	glGenBuffers(1, &vertBuf);

	glBindVertexArray(vertArr);
	glBindBuffer(GL_ARRAY_BUFFER, vertBuf);

	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * vertCount, &vertBufData[0], GL_STATIC_DRAW);

	// Model location
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);

	// UVs
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(glm::vec3)));

	// Fragment normals
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(glm::vec3) + sizeof(glm::vec2)));

	// End Attribute Setup

	return false;
}

// Renders model (called every frame in game loop)
void Model::render()
{
	glBindVertexArray(vertArr);
	glDrawArrays(GL_TRIANGLES, 0, vertCount);
}

void Model::remove()
{
	glDeleteVertexArrays(1, &vertArr);
	glDeleteBuffers(1, &vertBuf);

	vertArr = 0;
	vertBuf = 0;
	vertCount = 0;
}
