#ifndef RESOURCE_MANAGER_H
#define RESOURCE_MANAGER_H

#include <map>

#define GLEW_STATIC
#include <GL/glew.h>

#include "texture.h"
#include "shader.h"


/************************SINGLETON****************************** 
	A static singleton ResourceManager class that hosts several
	functions to load Textures and Shaders. Each loaded texture
	and/or shader is also stored in a map for future reference 
	by string handles. All functions and resources are static 
	and no public constructor is defined.
****************************************************************/
class ResourceManager
{
public:
	static enum TextureName
	{//TODO: will probably need more than just these
		BACKGROUND_TEX,
		BLOCK_TEX,
		BLOCK_SOLID_TEX,
		GRASS_TEX,
		DIRT_TEX,
		STONE_TEX,
		TILES_SPRITESHEET
	};
	static enum ShaderName
	{//TODO: definitely needs some good renaming
		SPRITE_SHADER,
		WATER_SHADER,//i wish
		SHADER_THREE
	};
	// Resource storage
	static std::map<ShaderName, Shader>    Shaders;
	static std::map<TextureName, Texture2D> Textures;
	// Loads (and generates) a shader program from file loading vertex, fragment (and geometry) shader's source code. If gShaderFile is not nullptr, it also loads a geometry shader
	static Shader   LoadShader(const GLchar *vShaderFile, const GLchar *fShaderFile, const GLchar *gShaderFile, ShaderName name);
	// Retrieves a stored shader
	static Shader   GetShader(ShaderName name);
	// Loads (and generates) a texture from file
	static Texture2D LoadTexture(const GLchar *file, GLboolean alpha, TextureName name);
	// Retrieves a stored texture
	static Texture2D GetTexture(TextureName name);
	// Properly de-allocates all loaded resources
	static void      Clear();
private:
	// Private constructor, that is we do not want any actual resource manager objects. Its members and functions should be publicly available (static).
	ResourceManager() { }
	// Loads and generates a shader from file
	static Shader    loadShaderFromFile(const GLchar *vShaderFile, const GLchar *fShaderFile, const GLchar *gShaderFile = nullptr);
	// Loads a single texture from file
	static Texture2D loadTextureFromFile(const GLchar *file, GLboolean alpha);
};

#endif