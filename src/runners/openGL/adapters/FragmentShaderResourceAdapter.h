/*
 * FragmentShaderResourceAdapter.h
 *
 *  Created on: Oct 24, 2022
 *      Author: leandro
 */

#pragma once

#include "ShaderResourceAdapter.h"

class FragmentShaderResourceAdapter : public ShaderResourceAdapter {
public:
	FragmentShaderResourceAdapter() {
		logger = LoggerFactory::getLogger("video/FragmentShaderResourceAdapter");
		this->produces(MimeTypes::FRAGMENTSHADER);
	}
};
