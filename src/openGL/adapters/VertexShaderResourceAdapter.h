/*
 * VertexShaderResourceAdapter.h
 *
 *  Created on: Oct 24, 2022
 *      Author: leandro
 */

#ifndef SRC_OPENGL_ADAPTERS_VERTEXSHADERRESOURCEADAPTER_H_
#define SRC_OPENGL_ADAPTERS_VERTEXSHADERRESOURCEADAPTER_H_

#include "ShaderResourceAdapter.h"

class VertexShaderResourceAdapter : public ShaderResourceAdapter {
public:
	VertexShaderResourceAdapter() {
		logger = LoggerFactory::getLogger("video/VertexShaderResourceAdapter");
		this->produces(MimeTypes::VERTEXSHADER);
		this->accepts(MimeTypes::GLSL);
	}
};


#endif /* SRC_OPENGL_ADAPTERS_VERTEXSHADERRESOURCEADAPTER_H_ */
