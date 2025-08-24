/*
 * TextureResourceAdapter.h
 *
 *  Created on: 28/01/2013
 *      Author: Lean
 */

#ifndef TEXTURERESOURCEADAPTER_H_
#define TEXTURERESOURCEADAPTER_H_

#include <set>

#include "OpenGLResourceAdapter.h"
#include "ImageResource.h"
#include "TextureResource.h"

class TextureResourceAdapter: public OpenGLResourceAdapter {
	public:
		TextureResourceAdapter() {
			logger = LoggerFactory::getLogger("video/TextureResourceAdapter");
			this->produces(MimeTypes::TEXTURE);
			//this->setInputMimeType(MimeTypes::IMAGE); //TODO: if at some point use a tree search of chained adapters the input mimetype should be image. In the meantime leave it blank as a wildcard.
		}

		virtual void load(ResourceLoadRequest &request, ResourceLoadResponse &response) const override {
			ImageResource *imageResource = (ImageResource *)this->getResourceManager()->load(ResourceLoadRequest(request).acceptMimeType(MimeTypes::IMAGE).withAdditionalLabels(std::set<String> {ResourceManager::EphemeralLabel}));

			TextureResource *resource = null;

			if(imageResource != null) {
				unsigned int textureHandler = 0;
				glGetError();
				glGenTextures(1, &textureHandler);
				resource = new TextureResource(textureHandler);
				glBindTexture(GL_TEXTURE_2D, textureHandler);
				if(imageResource->getBpp() == 32)
					glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageResource->getAncho(), imageResource->getAlto(), 0, GL_BGRA, GL_UNSIGNED_BYTE, imageResource->getData());
				else
					glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, imageResource->getAncho(), imageResource->getAlto(), 0, GL_BGR, GL_UNSIGNED_BYTE, imageResource->getData());


				if(request.getOption("texture-filter") == "nearest") {
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
				} else {
					glGenerateMipmap(GL_TEXTURE_2D);
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
				}

				glBindTexture(GL_TEXTURE_2D, 0);

				String errorMessage;
                if (!(errorMessage = getGlError()).empty()) {
                    logger->error("Error loading texture [%s]: [%s]", request.getFilePath().c_str(), errorMessage.c_str());
                    dispose(resource);
                    return;
                }
			}

			response.addResource(resource);
		}
		virtual void dispose(Resource *resource) const override {
            logger->debug("Disposing of Texture Resource  %s", resource->toString().c_str());

			TextureResource *textureResource = (TextureResource *)resource;
			unsigned int textureHandler = textureResource->getId();
			glDeleteTextures(1, &textureHandler);
		}
};
#endif /* TEXTURERESOURCEADAPTER_H_ */
