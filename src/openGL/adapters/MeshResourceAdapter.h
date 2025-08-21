/*
 * MeshResourceAdapter.h
 *
 *  Created on: Oct 13, 2022
 *      Author: leandro
 */

#ifndef SRC_OPENGL_ADAPTERS_MESHRESOURCEADAPTER_H_
#define SRC_OPENGL_ADAPTERS_MESHRESOURCEADAPTER_H_

#include <adapters/OpenGLResourceAdapter.h>
#include <MeshResource.h>

class MeshResourceAdapter: public OpenGLResourceAdapter {
public:
	MeshResourceAdapter() {
		logger = LoggerFactory::getLogger("video/MeshResourceAdapter");
		this->produces(MimeTypes::MESHCOLLECTION);
		this->produces(MimeTypes::MESH);
	}

	/**
	 * Load all objects including the mesh collection
	 */
	virtual void load(ResourceLoadRequest &request, ResourceLoadResponse &response) const override {
	    String geometryMimeType;

		GeometryCollection *geometryCollection = (GeometryCollection*) this->getResourceManager()->load(
				ResourceLoadRequest(request).acceptMimeType(MimeTypes::GEOMETRYCOLLECTION).withAdditionalLabels(std::set<String> { ResourceManager::EphemeralLabel })
		);
		if (geometryCollection == null || geometryCollection->getObjects().empty()) {
		    logger->error("Could not load geometry from %s with mimetype %s", request.getFilePath().c_str(), geometryMimeType.c_str());
		} else {
			for(auto &geometry : geometryCollection->getObjects()) {
				response.addResource(buildMesh(geometry.second, request, response));
			}

			response.addResource(geometryCollection);
		}
	}

	MeshResource * buildMesh(const GeometryResource *geometry, ResourceLoadRequest &request, ResourceLoadResponse &response) const {
		MeshResource *resource = new MeshResource();
        resource->setUri(geometry->getUri());

		resource->setVertexArray((VertexArrayResource *)response.addResource(OpenGLUtilites::generateVertexBuffer(geometry))); //make sure to add generated resources to resource manager or they're leaked
		if(geometry->getMaterial() != null) {
			resource->setMaterial(geometry->getMaterial());

			if(!geometry->getMaterial()->getAmbientTexture().empty()) {
				TextureResource *texture = (TextureResource *)getResourceManager()->load(geometry->getMaterial()->getDiffuseTexture(), MimeTypes::TEXTURE, request.getLabels(), request.getOptions());
				if(texture != null) {
					resource->setTexture(texture);
				} else {
					logger->warn("Could not load texture [%s]", geometry->getMaterial()->getDiffuseTexture().c_str());
				}
			}

			if(!geometry->getMaterial()->getDiffuseTexture().empty()) {
				TextureResource *texture = (TextureResource *)getResourceManager()->load(geometry->getMaterial()->getDiffuseTexture(), MimeTypes::TEXTURE, request.getLabels(), request.getOptions());
				if(texture != null) {
					resource->setTexture(texture);
				} else {
					logger->warn("Could not load texture [%s]", geometry->getMaterial()->getDiffuseTexture().c_str());
				}
			}

			if(!geometry->getMaterial()->getBumptTexture().empty()) {
				TextureResource *texture = (TextureResource *)getResourceManager()->load(geometry->getMaterial()->getBumptTexture(), MimeTypes::TEXTURE, request.getLabels(), request.getOptions());
				if(texture != null) {
					resource->setNormalMap(texture);
				} else {
					logger->warn("Could not load texture [%s]", geometry->getMaterial()->getBumptTexture().c_str());
				}
			}

			if(!geometry->getMaterial()->getSpecularTexture().empty()) {
				TextureResource *texture = (TextureResource *)getResourceManager()->load(geometry->getMaterial()->getSpecularTexture(), MimeTypes::TEXTURE, request.getLabels(), request.getOptions());
				if(texture != null) {
					resource->setSpecularMap(texture);
				} else {
					logger->warn("Could not load texture [%s]", geometry->getMaterial()->getSpecularTexture().c_str());
				}
			}
		}

		return resource;
	}
};

#endif /* SRC_OPENGL_ADAPTERS_MESHRESOURCEADAPTER_H_ */
