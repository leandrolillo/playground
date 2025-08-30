#pragma once

#include "ResourceAdapter.h"

class ResourceAdapterMock : public ResourceAdapter {
	Resource *mockedLoadResult = null;
public:
	ResourceAdapterMock(const std::set<String> &outputMimeTypes, String inputMimeType) {
		this->setOutputMimeTypes(outputMimeTypes);
		this->accepts(inputMimeType);
	}

	ResourceAdapterMock &withLoadResult(Resource *mockLoadResult) {
		this->mockedLoadResult = mockLoadResult;
		return *this;
	}

	virtual std::vector<Resource *> doload(ResourceLoadRequest &request, ResourceLoadResponse &response) const override {
	  std::vector<Resource *> result;

		if(mockedLoadResult != null) {
			result.push_back(mockedLoadResult);
		}

		return result;
	}
};
