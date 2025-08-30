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
	VertexShaderResourceAdapter() {
		logger = LoggerFactory::getLogger("video/VertexShaderResourceAdapter");
		this->produces(MimeTypes::VERTEXSHADER);
		this->accepts(MimeTypes::GLSL);
	}
};
