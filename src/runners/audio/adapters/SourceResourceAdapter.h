/*
 * AudioResourceAdapter.h
 *
 *  Created on: 01/01/2013
 *      Author: Lean
 */

#ifndef AUDIORESOURCEADAPTER_H_
#define AUDIORESOURCEADAPTER_H_

#include "al.h"
#include <set>

#include "ResourceAdapter.h"
#include "BufferResource.h"
#include "SourceResource.h"

class SourceResourceAdapter: public ResourceAdapter {
	public:
		SourceResourceAdapter() {
			logger = LoggerFactory::getLogger("audio/SourceResourceAdapter");
			this->produces(MimeTypes::AUDIOSOURCE);
		}

		virtual void load(ResourceLoadRequest &request, ResourceLoadResponse &response) const override {
			ALenum error = 0;

			logger->debug("loading audio/source from [%s]", request.getFilePath().c_str());

			ResourceLoadRequest bufferRequest(request);
			BufferResource * buffer = (BufferResource *)getResourceManager()->load(
					bufferRequest.acceptMimeType(MimeTypes::AUDIOBUFFER).withAdditionalLabels(std::set<String> {ResourceManager::ResourceManager::EphemeralLabel})
			);
			if(buffer == null) {
				logger->error("Error creating source: could not load buffer for [%s]", request.getFilePath().c_str());
				return;
			}

			ALuint sourceId;
			alGenSources(1, &sourceId);
			if((error = alGetError()) != AL_NO_ERROR) {
				logger->error("Error creating source for [%s]: %d", request.getFilePath().c_str(), error);
				return;
			}

			alSourcei (sourceId, AL_BUFFER, (ALuint) buffer->getId());
			if((error = alGetError()) != AL_NO_ERROR) {
				logger->error("Error setting properties for source [%s]: %d", request.getFilePath().c_str(), error);
				return;
			}

			response.addResource(new Source(sourceId));
		}
		virtual void dispose(Resource *resource) const override {
			logger->debug("Deleting source [%ld]", resource->getId());

			unsigned int sourceId = resource->getId();

			alDeleteSources(1, &sourceId);
		}
};
#endif /* AUDIORESOURCEADAPTER_H_ */
