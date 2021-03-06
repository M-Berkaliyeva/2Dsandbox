#ifndef RESOURCE_MANAGER_H
#define RESOURCE_MANAGER_H

#include <map>

#define GLEW_STATIC
#include <GL/glew.h>

#include "TileSheet.h"
#include "Shader.h"
#include "tinyxml.h"


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
	{
		TILES_TILESHEET,
		PLAYER_TILESHEET,
		ITEMS_TILESHEET,
		MOUNTAINS_BACKGROUND_TEXTURE,
		CLOUDS_BG_BACKGROUND_TEXTURE,
		CLOUDS_FG_BACKGROUND_TEXTURE
	};

	static enum TileType
	{//TODO: will probably need more than just these
		EMPTY_TILE,
		GRASS_TILE,
		DIRT_TILE,
		SAND_TILE,
		SNOW_TILE,
		STONE_TILE,

		TILES_COUNT
	};

	static enum ShaderName
	{
		TEXTURED_SPRITE_SHADER,
		TILE_SHADER,
		GAMEOBJECT_SHADER,
		SKY_BACKGROUND_SHADER,
		OVERLAY_BACKGROUND_SHADER,
		WATER_SHADER//i wish lol
	};
	static struct TexCoords
	{
		GLfloat u;
		GLfloat v;
		GLfloat width;
		GLfloat height;
	};
	// Resource storage
	static std::map<ShaderName, Shader*>		Shaders;
	static std::map<TextureName, TileSheet*>	TileSheets;
	static std::map<TileType, TexCoords>		TilesLookupTable;
	// Loads (and generates) a shader program from file loading vertex, fragment (and geometry) shader's source code. If gShaderFile is not nullptr, it also loads a geometry shader
	static void	LoadShader(const GLchar *vShaderFile, const GLchar *fShaderFile, const GLchar *gShaderFile, ShaderName name);
	// Retrieves a stored shader
	static Shader* GetShader(ShaderName name);
	// Loads (and generates) a tilesheet from file
	static void LoadTextures(const GLchar *file, GLboolean alpha, TextureName name, glm::ivec2 tileSize);
	// Retrieves a stored tilesheet
	static TileSheet* GetTexture(TextureName name);
	// Loads a spritesheet params (texture name, x, y, width and height) from file
	static void LoadSpritesheetParams(const GLchar *file, TextureName name);
	// Retrieves parameters of texture in spritesheet
	static TexCoords GetTileUVCoords(TextureName sName, TileType tName);
	// Properly de-allocates all loaded resources
	static void      Clear();
private:
	// Private constructor, that is we do not want any actual resource manager objects. Its members and functions should be publicly available (static).
	ResourceManager() { }
	// Loads and generates a shader from file
	static Shader* loadShaderFromFile(const GLchar *vShaderFile, const GLchar *fShaderFile, const GLchar *gShaderFile = nullptr);
	// Loads a single texture from file
	static TileSheet* loadTextureFromFile(const GLchar *file, GLboolean alpha, glm::ivec2 tileSize);
	// Loads a texture coodinate data
	static void loadTilesLookupTableFromFile(const GLchar *file);
};

#endif