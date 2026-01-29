#pragma once

#include "ResourceAdapter.h"

class ResourceAdapterMock : public ResourceAdapter {
	std::vector<Resource *>mockedLoadResult;
public:
	ResourceAdapterMock(ResourceManager &resourceManager, const std::set<String> &outputMimeTypes, String inputMimeType) : ResourceAdapter(resourceManager) {
	  logger = LoggerFactory::getLogger("tests/ResourceAdapterMock.h");
		this->setOutputMimeTypes(outputMimeTypes);
		this->accepts(inputMimeType);
	}

	ResourceAdapterMock &withLoadResult(Resource *mockLoadResult) {
		this->mockedLoadResult.push_back(mockLoadResult);
		return *this;
	}

	ResourceAdapterMock &withLoadResult(std::vector<Resource *>mockedLoadResults) {
	    mockedLoadResult.insert(mockedLoadResult.end(), mockedLoadResult.begin(), mockedLoadResult.end());
	    return *this;
	  }

  ResourceAdapterMock &clearResults() {
    this->mockedLoadResult.clear();
    return *this;
  }

	virtual std::vector<Resource *> doLoad(ResourceLoadRequest &request) const override {
		return mockedLoadResult;
	}
};
