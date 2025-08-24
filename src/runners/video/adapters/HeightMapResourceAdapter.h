/*
 * HeightMapResourceAdapter.h
 *
 *  Created on: Apr 2, 2021
 *      Author: leandro
 */

#ifndef SRC_OPENGL_ADAPTERS_HEIGHTMAPRESOURCEADAPTER_H_
#define SRC_OPENGL_ADAPTERS_HEIGHTMAPRESOURCEADAPTER_H_

#include<Math3d.h>
#include "ResourceAdapter.h"
#include "ImageResource.h"
#include "HeightMapResource.h"

class HeightMapResourceAdapter : public ResourceAdapter {
public:
    HeightMapResourceAdapter() {
        logger = LoggerFactory::getLogger("video/HeightMapResourceAdapter");
        this->produces(MimeTypes::HEIGHTMAP);
    }

    virtual void load(ResourceLoadRequest &request, ResourceLoadResponse &response) const override {
        ImageResource *image = (ImageResource *)this->getResourceManager()->load(ResourceLoadRequest(request).acceptMimeType(MimeTypes::IMAGE));
        if(image == null) {
        	logger->error("Could not load image [%s] - skipping heightmap creation", request.getFilePath().c_str());
        }

        response.addResource(new HeightMapResource(image, vector(2, 80, 2)));
    }
private:



};



#endif /* SRC_OPENGL_ADAPTERS_HEIGHTMAPRESOURCEADAPTER_H_ */
