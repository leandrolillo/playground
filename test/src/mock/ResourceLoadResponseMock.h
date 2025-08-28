#pragma once

#include "ResourceLoadResponse.h"

class ResourceLoadResponseMock : public ResourceLoadResponse{
private:
	Resource *lastAddedResource = null;
public:
	ResourceLoadResponseMock(ResourceLoadRequest &request, ResourceManager &resourceManager) : ResourceLoadResponse(request, resourceManager) {
	}

	Resource *addResource(Resource *resource) override {
	  ResourceLoadResponse::addResource(resource);

		this->lastAddedResource = resource;

		return lastAddedResource;
	}

	Resource *getLastAdded() {
		return this->lastAddedResource;
	}



};
