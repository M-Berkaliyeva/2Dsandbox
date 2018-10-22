#include "ResourceManager.h"

#include <iostream>
#include <sstream>
#include <fstream>

#include <SOIL.h>

// Instantiate static variables
std::map<ResourceManager::SpritesheetName, Texture2D>					ResourceManager::Spritesheets;
std::map<ResourceManager::TileType, ResourceManager::TexParams>			ResourceManager::TilesLookupTable;
std::map<ResourceManager::ShaderName, Shader>							ResourceManager::Shaders;

Shader ResourceManager::LoadShader(const GLchar *vShaderFile, const GLchar *fShaderFile, const GLchar *gShaderFile, ShaderName name)
{
	Shaders[name] = loadShaderFromFile(vShaderFile, fShaderFile, gShaderFile);
	return Shaders[name];
}

Shader ResourceManager::GetShader(ShaderName name)
{
	return Shaders[name];
}

Texture2D ResourceManager::LoadSpritesheet(const GLchar *file, GLboolean alpha, SpritesheetName name)
{
	Spritesheets[name] = loadTextureFromFile(file, alpha);
	return Spritesheets[name];
}

Texture2D ResourceManager::GetSpritesheet(SpritesheetName name)
{
	return Spritesheets[name];
}

void ResourceManager::LoadSpritesheetParams(const GLchar * file, SpritesheetName name)
{
	// Depending on different spritesheets load and store texture params (for player and enemy spritesheets params will also include animation data)
	switch (name)
	{
	case TILES_SPRITESHEET:
		loadTilesLookupTableFromFile(file);
		break;
	}
	//case PLAYER_SPRITESHEET:
	//case ENEMIES_SPRITESHEET:
	//case ITEMS_SPRITESHEET:
	//.....
	return;
}

ResourceManager::TexParams ResourceManager::GetTexParams(SpritesheetName sName, TileType tName)
{
	switch (sName)
	{
	case TILES_SPRITESHEET:
		return TilesLookupTable[tName];
	}
	//case PLAYER_SPRITESHEET:
	//case ENEMIES_SPRITESHEET:
	//case ITEMS_SPRITESHEET:
	//......
}

void ResourceManager::Clear()
{
	// (Properly) delete all shaders	
	for (auto iter : Shaders)
		glDeleteProgram(iter.second.ID);
	// (Properly) delete all textures
	for (auto iter : Spritesheets)
		glDeleteTextures(1, &iter.second.ID);
}

Shader ResourceManager::loadShaderFromFile(const GLchar *vShaderFile, const GLchar *fShaderFile, const GLchar *gShaderFile)
{
	// 1. Retrieve the vertex/fragment source code from filePath
	std::string vertexCode;
	std::string fragmentCode;
	std::string geometryCode;
	try
	{
		// Open files
		std::ifstream vertexShaderFile(vShaderFile);
		std::ifstream fragmentShaderFile(fShaderFile);
		std::stringstream vShaderStream, fShaderStream;
		// Read file's buffer contents into streams
		vShaderStream << vertexShaderFile.rdbuf();
		fShaderStream << fragmentShaderFile.rdbuf();
		// close file handlers
		vertexShaderFile.close();
		fragmentShaderFile.close();
		// Convert stream into string
		vertexCode = vShaderStream.str();
		fragmentCode = fShaderStream.str();
		// If geometry shader path is present, also load a geometry shader
		if (gShaderFile != nullptr)
		{
			std::ifstream geometryShaderFile(gShaderFile);
			std::stringstream gShaderStream;
			gShaderStream << geometryShaderFile.rdbuf();
			geometryShaderFile.close();
			geometryCode = gShaderStream.str();
		}
	}
	catch (std::exception e)
	{
		std::cout << "ERROR::SHADER: Failed to read shader files" << std::endl;
	}
	const GLchar *vShaderCode = vertexCode.c_str();
	const GLchar *fShaderCode = fragmentCode.c_str();
	const GLchar *gShaderCode = geometryCode.c_str();
	// 2. Now create shader object from source code
	Shader shader;
	shader.Compile(vShaderCode, fShaderCode, gShaderFile != nullptr ? gShaderCode : nullptr);
	return shader;
}

Texture2D ResourceManager::loadTextureFromFile(const GLchar *file, GLboolean alpha)
{
	// Create Texture object
	Texture2D texture;
	if (alpha)
	{
		texture.Internal_Format = GL_RGBA;
		texture.Image_Format = GL_RGBA;
	}
	// Load image
	int width, height;
	unsigned char* image = SOIL_load_image(file, &width, &height, 0, texture.Image_Format == GL_RGBA ? SOIL_LOAD_RGBA : SOIL_LOAD_RGB);
	// Now generate texture
	texture.Generate(width, height, image);
	//texture.GenerateTextureArray(width, height, image);
	// And finally free image data
	SOIL_free_image_data(image);
	return texture;
}

// Reading xml file and storing tile uvs in the map lookup table	
void ResourceManager::loadTilesLookupTableFromFile(const GLchar * file)
{
	TiXmlDocument levelFile(file);

	// Can't load file
	if (!levelFile.LoadFile())
	{
		std::cout << "Loading file \"" << file << "\" failed." << std::endl;
		return;
	}

	TiXmlElement *texAtlas;
	texAtlas = levelFile.FirstChildElement("TextureAtlas");

	float spWidth = atof(texAtlas->Attribute("spWidth"));
	float spHeight = atof(texAtlas->Attribute("spHeight"));

	TiXmlElement *tileParam;
	tileParam = texAtlas->FirstChildElement("SubTexture");
	
	TexParams currTileParam;	 
	for (int i = 1; i < TILES_COUNT; i++)//while(tileParam)
	{
		// THIS IS TEMP HACKED WAY TO DO IT!!! THINK MORE!!!!
		currTileParam.u = atof(tileParam->Attribute("x")) / spWidth;//DO NOT USE HARD CODED VALS!!!!!!! THIS IS TEMP to test!
		currTileParam.v = atof(tileParam->Attribute("y")) / spHeight;//DO NOT USE HARD CODED VALS!!!!!!! THIS IS TEMP to test!
		currTileParam.width = atof(tileParam->Attribute("width")) / spWidth;// TEMP TO MAKE IT NORMALIZED
		currTileParam.height = atof(tileParam->Attribute("height")) / spWidth;// TEMP TO MAKE IT NORMALIZED

		TilesLookupTable[(TileType)i] = currTileParam;//right now just stores different params for same name!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!1

		// Now move on and read next subtexture 
		tileParam = tileParam->NextSiblingElement("SubTexture");
	}
	return;
}

