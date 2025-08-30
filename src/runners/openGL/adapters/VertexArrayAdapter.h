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
#include "GeometryCollection.h"

class VertexArrayResourceAdapter: public OpenGLResourceAdapter {
public:
  VertexArrayResourceAdapter() {
    logger = LoggerFactory::getLogger("video/VertexArrayResourceAdapter");
    this->produces(MimeTypes::VERTEXARRAY);
  }

  virtual void dispose(Resource *resource) const override {
    OpenGLUtilites::disposeVertexArray((VertexArrayResource*) resource);
  }

protected:
  /**
   * Temporarily returns a single VERTEXARRAY but it also loads all other meshes in the file, leaving them available in the resource manager caché
   */
  virtual std::vector<Resource*> doLoad(ResourceLoadRequest &request) const override {
    std::vector<Resource*> response;

    GeometryCollection *geometryCollection = (GeometryCollection*) this->getResourceManager().load(
        ResourceLoadRequest(request).acceptMimeType(MimeTypes::GEOMETRYCOLLECTION).withAdditionalLabels(std::set<String> {
            ResourceManager::EphemeralLabel })
            );
    if (geometryCollection == null || geometryCollection->getObjects().empty()) {
      logger->error("Could not load geometry from %s with mimetype %s", request.getFilePath().c_str(),
          MimeTypes::GEOMETRYCOLLECTION.c_str());
      return response;
    }

    for (auto &geometry : geometryCollection->getObjects()) {
      response.push_back(OpenGLUtilites::generateVertexBuffer(geometry.second)); //make sure we add it to resource manager to avoid leaks
    }

    return response;
  }

};
