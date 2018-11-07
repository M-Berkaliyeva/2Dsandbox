#pragma once

#include <glm/glm.hpp>
#include "Texture.h"

class TileSheet {
public:
	void init(const Texture2D& texture, const glm::ivec2& tilesheetDims) {
		this->texture = texture;
		this->dims = tilesheetDims;
	}

	glm::vec4 getUVs(int index)	{
		int xTile = index % dims.x;
		int yTile = index / dims.x;

		glm::vec4 uvs;
		uvs.x = xTile / (float)dims.x;
		uvs.y = yTile / (float)dims.y;
		uvs.z = 1.0f / dims.x;
		uvs.w = 1.0f / dims.y;

		return uvs;
	}

	Texture2D texture;
	glm::ivec2 dims;
};
