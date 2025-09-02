#pragma once

#include "Resource.h"

class ShaderResource : public Resource
{
	public:
		ShaderResource(unsigned int id) : Resource(id, "" /*MimeTypes::VERTEXSHADER*/) // we are using the same resource for multiple mimetypes, so let resource manager populate it.
		{

		}
};
