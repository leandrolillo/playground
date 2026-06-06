#pragma once

#include "Resource.h"

class TextureResource : public Resource
{
	public:
		TextureResource(unsigned int id) : Resource(id, MimeTypes::TEXTURE)
		{

		}

		TextureResource(unsigned int id, const String &mimeType) : Resource(id, mimeType)
		{

		}
};
