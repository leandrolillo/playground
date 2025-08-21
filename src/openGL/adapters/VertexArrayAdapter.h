/*
 * VertexBufferAdapter.h
 *
 *  Created on: 07/02/2013
 *      Author: Lean
 */

#ifndef VERTEXBUFFERADAPTER_H_
#define VERTEXBUFFERADAPTER_H_
#include <adapters/OpenGLResourceAdapter.h>
#include <GeometryCollection.h>
#include <set>
#include<OpenGLUtilities.h>


class VertexArrayResourceAdapter: public OpenGLResourceAdapter {
public:
	VertexArrayResourceAdapter() {
		logger = LoggerFactory::getLogger("video/VertexArrayResourceAdapter");
		this->produces(MimeTypes::VERTEXARRAY);
	}

	/**
	 * Temporarily returns a single VERTEXARRAY but it also loads all other meshes in the file, leaving them available in the resource manager caché
	 */
	virtual void load(ResourceLoadRequest &request, ResourceLoadResponse &response) const override {
		GeometryCollection *geometryCollection = (GeometryCollection*) this->getResourceManager()->load(
				ResourceLoadRequest(request).acceptMimeType(MimeTypes::GEOMETRYCOLLECTION).withAdditionalLabels(std::set<String> { ResourceManager::EphemeralLabel })
		);
		if (geometryCollection == null || geometryCollection->getObjects().empty()) {
		    logger->error("Could not load geometry from %s with mimetype %s", request.getFilePath().c_str(), MimeTypes::GEOMETRYCOLLECTION.c_str());
		    return;
		}

		for(auto & geometry : geometryCollection->getObjects()) {
			response.addResource(OpenGLUtilites::generateVertexBuffer(geometry.second)); //make sure we add it to resource manager to avoid leaks
		}

		//response.addResource(vertexArrayCollection);
	}

	virtual void dispose(Resource *resource) const override {
		OpenGLUtilites::disposeVertexArray((VertexArrayResource *)resource);
	}
};
#endif /* VERTEXBUFFERADAPTER_H_ */
