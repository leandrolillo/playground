/*
 * ShaderProgramResource.h
 *
 *  Created on: 23/02/2013
 *      Author: Lean
 */

#ifndef SHADERPROGRAMRESOURCE_H_
#define SHADERPROGRAMRESOURCE_H_


#include "ShaderResource.h"

class ShaderProgramResource : public Resource
{
	private:
		std::vector<ShaderResource *>shaders;
	public:
		ShaderProgramResource(unsigned int id) : Resource(id, MimeTypes::SHADERPROGRAM)
		{

		}

		std::vector<ShaderResource*>& getShaders() {
			return shaders;
		}

		void setShaders(const std::vector<ShaderResource*>& shaders) {
			this->shaders = shaders;
		}
};



#endif /* SHADERPROGRAMRESOURCE_H_ */
