/*
 * AudioResourceAdapter.h
 *
 *  Created on: 01/01/2013
 *      Author: Lean
 */

#pragma once

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

  virtual void dispose(Resource *resource) const override {
    logger->debug("Deleting source [%ld]", resource->getId());

    unsigned int sourceId = resource->getId();

    alDeleteSources(1, &sourceId);
  }

protected:
  virtual std::vector<Resource *> doLoad(ResourceLoadRequest &request) const override {
    std::vector<Resource *>response;

    ALenum error = 0;

    logger->debug("loading audio/source from [%s]", request.getFilePath().c_str());

    ResourceLoadRequest bufferRequest(request);
    BufferResource *buffer = (BufferResource*) getResourceManager().load(
        bufferRequest.acceptMimeType(MimeTypes::AUDIOBUFFER).withAdditionalLabels(std::set<String> {
            ResourceManager::ResourceManager::EphemeralLabel })
            );
    if (buffer == null) {
      logger->error("Error creating source: could not load buffer for [%s]", request.getFilePath().c_str());
      return response;
    }

    ALuint sourceId;
    alGenSources(1, &sourceId);
    if ((error = alGetError()) != AL_NO_ERROR) {
      logger->error("Error creating source for [%s]: %d", request.getFilePath().c_str(), error);
      return response;
    }

    alSourcei(sourceId, AL_BUFFER, (ALuint) buffer->getId());
    if ((error = alGetError()) != AL_NO_ERROR) {
      logger->error("Error setting properties for source [%s]: %d", request.getFilePath().c_str(), error);
      return response;
    }

    response.push_back(new Source(sourceId));

    return response;
  }
};
