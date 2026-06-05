/*
 * MeshResourceAdapter.h
 *
 *  Created on: Oct 13, 2022
 *      Author: leandro
 */

#pragma once

#include "OpenGLResourceAdapter.h"
#include "GeometryResource.h"
#include "MeshResource.h"
#include "OpenGLUtilities.h"

class MeshResourceAdapter: public OpenGLResourceAdapter {
public:
  MeshResourceAdapter(ResourceManager &resourceManager) : OpenGLResourceAdapter(resourceManager) {
    logger = LoggerFactory::getLogger("video/MeshResourceAdapter");
    this->produces(MimeTypes::MESH);
  }

protected:
  /**
   * Load all objects including the mesh collection
   */
  virtual std::vector<Resource*> doLoad(ResourceLoadRequest &request) const override {
    std::vector<Resource*> response;

    GeometryResource *geometry = (GeometryResource *) this->getResourceManager().load(
        ResourceLoadRequest(request).acceptMimeType(MimeTypes::GEOMETRY).withAdditionalLabels(std::set<String> {
            ResourceManager::EphemeralLabel })
            );
    if (geometry == null) {
      logger->error("Could not load geometry [%s] with mimetype [%s]", request.getFilePath().c_str(), MimeTypes::GEOMETRY.c_str());
    } else {
        response.push_back(buildMesh(geometry, request, response));
    }

    return response;
  }

private:
  MeshResource* buildMesh(const GeometryResource *geometry, ResourceLoadRequest &request, std::vector<Resource*> &response) const {
    MeshResource *resource = new MeshResource();
    resource->setName(geometry->getName());
    resource->setVertexArray(OpenGLUtilities::generateVertexArray(geometry));
    response.push_back(resource->getVertexArray()); //make sure to add generated resources to resource manager or they're leaked

    if (geometry->getMaterial() != null) {
      resource->setMaterial(geometry->getMaterial());

      if (!geometry->getMaterial()->getAmbientTexture().empty()) {
        TextureResource *texture = (TextureResource*) getResourceManager().load(geometry->getMaterial()->getDiffuseTexture(),
            MimeTypes::TEXTURE, request.getLabels(), request.getOptions());
        if (texture != null) {
          resource->setTexture(texture);
        } else {
          logger->warn("Could not load texture [%s]", geometry->getMaterial()->getDiffuseTexture().c_str());
        }
      }

      if (!geometry->getMaterial()->getDiffuseTexture().empty()) {
        TextureResource *texture = (TextureResource*) getResourceManager().load(geometry->getMaterial()->getDiffuseTexture(),
            MimeTypes::TEXTURE, request.getLabels(), request.getOptions());
        if (texture != null) {
          resource->setTexture(texture);
        } else {
          logger->warn("Could not load texture [%s]", geometry->getMaterial()->getDiffuseTexture().c_str());
        }
      }

      if (!geometry->getMaterial()->getBumpTexture().empty()) {
        TextureResource *texture = (TextureResource*) getResourceManager().load(geometry->getMaterial()->getBumpTexture(),
            MimeTypes::TEXTURE, request.getLabels(), request.getOptions());
        if (texture != null) {
          resource->setNormalMap(texture);
        } else {
          logger->warn("Could not load texture [%s]", geometry->getMaterial()->getBumpTexture().c_str());
        }
      }

      if (!geometry->getMaterial()->getSpecularTexture().empty()) {
        TextureResource *texture = (TextureResource*) getResourceManager().load(geometry->getMaterial()->getSpecularTexture(),
            MimeTypes::TEXTURE, request.getLabels(), request.getOptions());
        if (texture != null) {
          resource->setSpecularMap(texture);
        } else {
          logger->warn("Could not load texture [%s]", geometry->getMaterial()->getSpecularTexture().c_str());
        }
      }
    }

    return resource;
  }
};
