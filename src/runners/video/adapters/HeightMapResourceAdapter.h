/*
 * HeightMapResourceAdapter.h
 *
 *  Created on: Apr 2, 2021
 *      Author: leandro
 */

#pragma once

#include<Math3d.h>
#include "ResourceAdapter.h"
#include "ImageResource.h"
#include "HeightMapResource.h"

class HeightMapResourceAdapter: public ResourceAdapter {
public:
  HeightMapResourceAdapter() {
    logger = LoggerFactory::getLogger("video/HeightMapResourceAdapter");
    this->produces(MimeTypes::HEIGHTMAP);
  }

protected:
  virtual std::vector<Resource *> doLoad(ResourceLoadRequest &request) const override {
    std::vector<Resource *> response;
    ImageResource *image = (ImageResource*) this->getResourceManager().load(ResourceLoadRequest(request).acceptMimeType(MimeTypes::IMAGE));
    if (image != null) {
      response.push_back(new HeightMapResource(image, vector(2, 80, 2)));
    } else {
      logger->error("Could not load image [%s] - skipping heightmap creation", request.getFilePath().c_str());
    }

    return response;
  }
};
