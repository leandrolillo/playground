/*
 * CubeMapResource.h
 *
 *  Created on: Mar 30, 2021
 *      Author: leandro
 */

#ifndef SRC_RUNNERS_VIDEO_RESOURCES_CUBEMAPRESOURCE_H_
#define SRC_RUNNERS_VIDEO_RESOURCES_CUBEMAPRESOURCE_H_

#include "TextureResource.h"


class CubeMapResource : public TextureResource {
public:
	CubeMapResource(unsigned int id) : TextureResource(id, MimeTypes::CUBEMAP)
	{

	}
};




#endif /* SRC_RUNNERS_VIDEO_RESOURCES_CUBEMAPRESOURCE_H_ */
