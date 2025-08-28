/*
 * ResourceManagerMock.h
 *
 *  Created on: Jan 4, 2023
 *      Author: leandro
 */

#pragma once

#include "ResourceManager.h"

class ResourceManagerMock : public ResourceManager {
public:
	ResourceManagerMock(const String &rootFolder) : ResourceManager(rootFolder) {
	}

	unsigned int getAdaptersCount() {
		return this->resourceAdapters.size();
	}

	unsigned int getResourcesCount() {
		return this->resources.size();
	}

	unsigned int getResourcesCount(const String &mimeType) {
		unsigned int count = 0;

		for(const auto &resource : this->resources) {
			if(resource->getMimeType() == mimeType) {
				count++;
			}
		}
		return count;
	}
};
