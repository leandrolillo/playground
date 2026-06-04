/*
 * VertexBufferAdapter.h
 *
 *  Created on: 07/02/2013
 *      Author: Lean
 */

#pragma once
#include <set>

#include "OpenGLResourceAdapter.h"
#include "OpenGLUtilities.h"

class VertexArrayResourceAdapter: public OpenGLResourceAdapter {
public:
  VertexArrayResourceAdapter(ResourceManager &resourceManager) : OpenGLResourceAdapter(resourceManager) {
    logger = LoggerFactory::getLogger("video/VertexArrayResourceAdapter");
    this->produces(MimeTypes::VERTEXARRAY);
  }

  virtual void dispose(Resource *resource) const override {
    OpenGLUtilities::disposeVertexArray((VertexArrayResource*) resource);
  }

protected:
  /**
   * Temporarily returns a single VERTEXARRAY but it also loads all other meshes in the file, leaving them available in the resource manager caché
   */
  virtual std::vector<Resource*> doLoad(ResourceLoadRequest &request) const override {
    std::vector<Resource*> response;

    GeometryResource *geometry= (GeometryResource *) this->getResourceManager().load(
        ResourceLoadRequest(request).acceptMimeType(MimeTypes::GEOMETRY).withAdditionalLabels(std::set<String> {
            ResourceManager::EphemeralLabel })
            );
    if (geometry == null) {
      logger->error("Could not load geometry [%s] with mimetype [%s]", request.getFilePath().c_str(), MimeTypes::GEOMETRY.c_str());
    } else {
        response.push_back(OpenGLUtilities::generateVertexArray(geometry)); //make sure we add it to resource manager to avoid leaks
    }

    return response;
  }
};
