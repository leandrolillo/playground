#include "ResourceAdapter.h"

//TODO: Maybe this should be expanded for handling logic here instead of in resource manager - for example checking request has mimetype and matches the output of the adapter, there's a uri, etc.
std::vector<Resource *> ResourceAdapter::load(ResourceLoadRequest &request) {
  if(this != null && !this->isValid()) {
    logger->error("Resource Adapter [%s] is invalid: [%s]", this->toString().c_str(), this->errors().c_str());
    return std::vector<Resource *>();
  }

  auto resources = doLoad(request);

  for (auto resource : resources) {
    if (resource->getUri().empty()) {
      resource->setUri(request.getUri());
    }

    if (resource->getMimeType().empty()) {
      resource->setMimeType(request.getOutputMimeType());
    }

    resource->setLabels(request.getLabels());
    logger->debug("Loaded [%s]", resource->toString().c_str());
  }

  return resources;
}
