#include "Texture.h"

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


Texture::Texture()
{
}


Texture::~Texture()
{
}

void Texture::init(std::string name)
{
	texFilename = name;

	FIBITMAP* image = FreeImage_Load(FreeImage_GetFileType(texFilename.c_str(), 0), texFilename.c_str());		// Loads in raw image
	if (image != nullptr) {
		FIBITMAP* image32Bit = FreeImage_ConvertTo32Bits(image);	// Converts image to a 32 bit image
		FreeImage_Unload(image);	// Removes image from RAM

		glGenTextures(1, &texID);
		glBindTexture(GL_TEXTURE_2D, texID);	// Bind texture to RAM

		glTexImage2D(GL_TEXTURE_2D, 0, GL_SRGB_ALPHA, FreeImage_GetWidth(image32Bit), FreeImage_GetHeight(image32Bit), 0, GL_BGRA, GL_UNSIGNED_BYTE, (void*)FreeImage_GetBits(image32Bit));		// Upload texture bytes

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);	// Set min filter to linear (default is mipmap linear)

		FreeImage_Unload(image32Bit);		// Removes image32Bit from RAM

		glBindTexture(GL_TEXTURE_2D, 0);	// Unbind texture
	}

	else {
		printf("Error loading texture");
	}
}

// Applies the texture to the model
void Texture::apply()
{
	glBindTexture(GL_TEXTURE_2D, texID);
}

// Removes the texture
void Texture::remove()
{
	glDeleteTextures(1, &texID);
}
