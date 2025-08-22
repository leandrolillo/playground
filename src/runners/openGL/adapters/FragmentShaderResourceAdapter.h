/*
 * FragmentShaderResourceAdapter.h
 *
 *  Created on: Oct 24, 2022
 *      Author: leandro
 */

#ifndef SRC_RUNNERS_OPENGL_ADAPTERS_FRAGMENTSHADERRESOURCEADAPTER_H_
#define SRC_RUNNERS_OPENGL_ADAPTERS_FRAGMENTSHADERRESOURCEADAPTER_H_


#include "../../runners/openGL/adapters/ShaderResourceAdapter.h"

class FragmentShaderResourceAdapter : public ShaderResourceAdapter {
public:
	FragmentShaderResourceAdapter() {
		logger = LoggerFactory::getLogger("video/FragmentShaderResourceAdapter");
		this->produces(MimeTypes::FRAGMENTSHADER);
	}
};


#endif /* SRC_RUNNERS_OPENGL_ADAPTERS_FRAGMENTSHADERRESOURCEADAPTER_H_ */
