/*
 * CubeMapResource.h
 *
 *  Created on: Mar 30, 2021
 *      Author: leandro
 */

#ifndef SRC_VIDEO_RESOURCES_CUBEMAPRESOURCE_H_
#define SRC_VIDEO_RESOURCES_CUBEMAPRESOURCE_H_

#include <resources/TextureResource.h>


class CubeMapResource : public TextureResource {
public:
	CubeMapResource(unsigned int id) : TextureResource(id, MimeTypes::CUBEMAP)
	{

	}
};




#endif /* SRC_VIDEO_RESOURCES_CUBEMAPRESOURCE_H_ */
