/*
 * ShaderResource.h
 *
 *  Created on: 23/02/2013
 *      Author: Lean
 */

#ifndef SHADERRESOURCE_H_
#define SHADERRESOURCE_H_


#include "../../Resource.h"

class ShaderResource : public Resource
{
	public:
		ShaderResource(unsigned int id) : Resource(id, "" /*MimeTypes::VERTEXSHADER*/) // we are using the same resource for multiple mimetypes, so let resource manager populate it.
		{

		}
};
#endif /* SHADERRESOURCE_H_ */
