/*
 * BufferResource.h
 *
 *  Created on: 14/01/2013
 *      Author: Lean
 */

#ifndef BUFFERRESOURCE_H_
#define BUFFERRESOURCE_H_

#include "../../Resource.h"

class BufferResource : public Resource
{
	public:
		BufferResource(unsigned int id) : Resource(id, MimeTypes::AUDIOBUFFER) {
		}
};



#endif /* BUFFERRESOURCE_H_ */
