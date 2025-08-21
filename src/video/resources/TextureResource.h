/*
 * TextureResource.h
 *
 *  Created on: 28/01/2013
 *      Author: Lean
 */

#ifndef TEXTURERESOURCE_H_
#define TEXTURERESOURCE_H_

#include <Resource.h>

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


#endif /* TEXTURERESOURCE_H_ */
