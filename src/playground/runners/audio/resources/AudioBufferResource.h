/*
 * BufferResource.h
 *
 *  Created on: 14/01/2013
 *      Author: Lean
 */

#pragma once

#include "Resource.h"

class AudioBufferResource : public Resource
{
	public:
		AudioBufferResource(unsigned int id) : Resource(id, MimeTypes::AUDIOBUFFER) {
		}
};
