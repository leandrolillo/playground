/*
 * CubeMapResource.h
 *
 *  Created on: Mar 30, 2021
 *      Author: leandro
 */

#pragma once

#include "TextureResource.h"

class CubeMapResource : public TextureResource {
public:
	CubeMapResource(unsigned int id) : TextureResource(id, MimeTypes::CUBEMAP)
	{

	}
};
