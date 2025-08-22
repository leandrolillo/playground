#pragma once

#include "../../../playground/resources/ResourceAdapter.h"

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

	virtual void load(ResourceLoadRequest &request, ResourceLoadResponse &response) const override {
		if(mockedLoadResult != null) {
			response.addResource(mockedLoadResult);
		}
	}
};
