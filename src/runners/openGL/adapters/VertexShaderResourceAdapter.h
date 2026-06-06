/*
 * VertexShaderResourceAdapter.h
 *
 *  Created on: Oct 24, 2022
 *      Author: leandro
 */

#pragma once

#include "ShaderResourceAdapter.h"

class VertexShaderResourceAdapter : public ShaderResourceAdapter {
public:
	VertexShaderResourceAdapter(ResourceManager &resourceManager) : ShaderResourceAdapter(resourceManager) {
		logger = LoggerFactory::getLogger("video/VertexShaderResourceAdapter");
		this->produces(MimeTypes::VERTEXSHADER);
		this->accepts(MimeTypes::GLSL);
	}
};
