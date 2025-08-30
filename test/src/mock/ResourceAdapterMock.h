#pragma once

#include "ResourceAdapter.h"

class ResourceAdapterMock : public ResourceAdapter {
	Resource *mockedLoadResult = null;
public:
	ResourceAdapterMock(const std::set<String> &outputMimeTypes, String inputMimeType) {
	  logger = LoggerFactory::getLogger("tests/ResourceAdapterMock.h");
		this->setOutputMimeTypes(outputMimeTypes);
		this->accepts(inputMimeType);
	}

	ResourceAdapterMock &withLoadResult(Resource *mockLoadResult) {
		this->mockedLoadResult = mockLoadResult;
		return *this;
	}

	virtual std::vector<Resource *> doLoad(ResourceLoadRequest &request) const override {
	  std::vector<Resource *> result;

		if(mockedLoadResult != null) {
			result.push_back(mockedLoadResult);
		}

		return result;
	}
};
